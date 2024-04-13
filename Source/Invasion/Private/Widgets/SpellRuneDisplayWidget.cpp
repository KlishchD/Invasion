#include "Widgets/SpellRuneDisplayWidget.h"
#include "Invasion/InvasionCharacter.h"

void USpellRuneDisplayWidget::NativeOnInitialized()
{
	SpellDrawingSubsystem = GetGameInstance()->GetSubsystem<USpellDrawingSubsystem>();
}

void USpellRuneDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AInvasionCharacter* Character = GetOwningPlayerPawn<AInvasionCharacter>();
	USpell* Spell = Character->GetSpellComponent()->GetActiveSpell();
	
	SpellDrawingSubsystem->SetActiveSpell(Spell);

	RuneImageDynamicMaterialInstance = UMaterialInstanceDynamic::Create(RuneImageMaterial, this);
	RuneImageDynamicMaterialInstance->SetTextureParameterValue(TEXT("Background"), SpellDrawingSubsystem->GetRuneExample());
	RuneImageDynamicMaterialInstance->SetTextureParameterValue(TEXT("Foreground"), SpellDrawingSubsystem->GetRuneCanvas());

	Image->SetBrushFromMaterial(RuneImageDynamicMaterialInstance);

	SetFocus();

	bIsDrawing = false;
}

FReply USpellRuneDisplayWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		bIsDrawing = true;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply USpellRuneDisplayWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		bIsDrawing = false;
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply USpellRuneDisplayWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDrawing)
	{
		FVector2D LeftTop = InGeometry.GetAbsolutePositionAtCoordinates(FVector2D::ZeroVector);
		SpellDrawingSubsystem->AddPoint(InMouseEvent.GetScreenSpacePosition() - LeftTop);
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply USpellRuneDisplayWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == FinishKey)
	{
		CastSpell();

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USpellRuneDisplayWidget::CastSpell()
{
	AInvasionCharacter* Character = GetOwningPlayerPawn<AInvasionCharacter>();

	Character->CastSpell(SpellDrawingSubsystem->GetSpellCastStrength());

	RemoveFromViewport();

	SpellDrawingSubsystem->SetActiveSpell(nullptr);

	Character->SwitchToGameMode();
}
