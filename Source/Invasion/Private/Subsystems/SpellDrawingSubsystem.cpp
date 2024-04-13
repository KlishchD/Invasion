#include "Subsystems/SpellDrawingSubsystem.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"

void USpellDrawingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeTarget();
}

void USpellDrawingSubsystem::SetActiveSpell(USpell* Spell)
{
	ActiveSpell = Spell;

	if (Spell)
	{
		DrawRune();
	}
	else
	{
		ClearPoints();
	}
}

UTextureRenderTarget2D* USpellDrawingSubsystem::GetRuneExample() const
{
	return RuneDisplayTarget;
}

UTextureRenderTarget2D* USpellDrawingSubsystem::GetRuneCanvas() const
{
	return CanvasDisplayTarget;
}

void USpellDrawingSubsystem::DrawRune()
{
	if (!ActiveSpell)
	{
		return;
	}

	InitializeTarget();

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RuneDisplayTarget, Canvas, Size, Context);

	const TArray<FVector2D>& ActiveSpellPoints = ActiveSpell->RunePoints;
	for (int32 i = 1; i < ActiveSpellPoints.Num(); ++i)
	{
		FVector2D PreviousScreen = ActiveSpellPoints[i - 1] * Size;
		FVector2D CurrentScreen = ActiveSpellPoints[i] * Size;
		float LineSize = (CurrentScreen - PreviousScreen).Size();

		FVector2D Direction = (CurrentScreen - PreviousScreen).GetSafeNormal();
		float Angle = FMath::RadiansToDegrees(FMath::Acos(Direction.Dot(AxisLine)));

		Angle *= PreviousScreen > CurrentScreen ? -1 : 1;

		Canvas->K2_DrawMaterial(ActiveSpell->RuneMaterial.Get(), PreviousScreen, FVector2D(LineSize, LineWidth), FVector2D::ZeroVector, FVector2D::UnitVector, Angle, FVector2D::ZeroVector);
	}

	/* Tiled version ;)
	FVector2D AxisLine = FVector2D(1.0f, 0.0f);
	float Tile = 20;

	const TArray<FVector2D>& Points = Spell->RunePoints;
	for (int32 i = 1; i < Points.Num(); ++i)
	{
		FVector2D PreviousScreen = Points[i - 1] * Size;
		FVector2D CurrentScreen = Points[i] * Size;
		float LineSize = (CurrentScreen - PreviousScreen).Size();

		FVector2D Direction = (CurrentScreen - PreviousScreen).GetSafeNormal();
		float Angle = FMath::RadiansToDegrees(FMath::Acos(Direction.Dot(AxisLine)));

		Angle *= PreviousScreen > CurrentScreen ? -1 : 1;

		int32 Count = LineSize / Tile; 
		for (int32 j = 0; j < Count; ++j)
		{
			Canvas->K2_DrawMaterial(Spell->RuneMaterial.Get(), PreviousScreen + j * Direction * Tile, FVector2D(Tile, Tile), FVector2D::ZeroVector, FVector2D::UnitVector, Angle, FVector2D::ZeroVector);
		}
	}
	*/

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void USpellDrawingSubsystem::DrawTile(FVector2D Point)
{
	InitializeTarget();

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), CanvasDisplayTarget, Canvas, Size, Context);

	Canvas->K2_DrawMaterial(ActiveSpell->RuneMaterial.Get(), Point - FVector2D(TileSize / 2.0f), FVector2D(TileSize), FVector2D::ZeroVector);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void USpellDrawingSubsystem::InitializeTarget()
{
	FVector2D ViewportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	if (!RuneDisplayTarget && !ViewportSize.IsNearlyZero())
	{
		RuneDisplayTarget = NewObject<UTextureRenderTarget2D>(this);
		RuneDisplayTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
		RuneDisplayTarget->ClearColor = FLinearColor::Transparent;

		RuneDisplayTarget->InitAutoFormat(ViewportSize.X, ViewportSize.Y);
		RuneDisplayTarget->UpdateResourceImmediate(true);
	}

	if (!CanvasDisplayTarget && !ViewportSize.IsNearlyZero())
	{
		CanvasDisplayTarget = NewObject<UTextureRenderTarget2D>(this);
		CanvasDisplayTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
		CanvasDisplayTarget->ClearColor = FLinearColor::Transparent;

		CanvasDisplayTarget->InitAutoFormat(ViewportSize.X, ViewportSize.Y);
		CanvasDisplayTarget->UpdateResourceImmediate(true);
	}
}

float USpellDrawingSubsystem::GetSpellCastStrength() const
{
	FVector2D Size;
	Size.X = RuneDisplayTarget->GetSurfaceWidth();
	Size.Y = RuneDisplayTarget->GetSurfaceHeight();

	const TArray<FVector2D>& ActiveSpellPoints = ActiveSpell->RunePoints;
	
	for (const FVector2D& Point : Points)
	{
		FVector2D PointUV = Point / Size;
	
		float MinDistanceSqr = 10.0f;

		for (int32 i = 1; i < ActiveSpellPoints.Num(); ++i)
		{
			FVector2D ClosestPosition = FMath::ClosestPointOnSegment2D(PointUV, ActiveSpellPoints[i - 1], ActiveSpellPoints[i]);
			float DistanceSqr = FVector2D::DistSquared(PointUV, ClosestPosition);
			MinDistanceSqr = FMath::Min(MinDistanceSqr, DistanceSqr);
		}

		if (MinDistanceSqr > PointToLineThreshold)
		{
			return 0.0f;
		}
	}

	for (int32 i = 0; i < ActiveSpellPoints.Num(); ++i)
	{
		float MinDistanceSqr = 10.0f;

		for (const FVector2D& Point : Points)
		{
			FVector2D PointUV = Point / Size;
			float DistanceSqr = FVector2D::DistSquared(PointUV, ActiveSpellPoints[i]);
			MinDistanceSqr = FMath::Min(MinDistanceSqr, DistanceSqr);
		}

		if (MinDistanceSqr > SegmentToPointsThreshold)
		{
			return 0.0f;
		}
	}

	return 1.0f;
}

void USpellDrawingSubsystem::AddPoint(FVector2D Position)
{
	Points.Add(Position);
	DrawTile(Position);
}

void USpellDrawingSubsystem::ClearPoints()
{
	Points.Empty();

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RuneDisplayTarget, RuneDisplayTarget->ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), CanvasDisplayTarget, CanvasDisplayTarget->ClearColor);
}
