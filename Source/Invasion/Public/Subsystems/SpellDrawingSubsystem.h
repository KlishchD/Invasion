#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Spell.h"
#include "SpellDrawingSubsystem.generated.h"

UCLASS()
class INVASION_API USpellDrawingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	void SetActiveSpell(USpell* Spell);

	UTextureRenderTarget2D* GetRuneExample() const;
	UTextureRenderTarget2D* GetRuneCanvas() const;

	float GetSpellCastStrength() const;

	void AddPoint(FVector2D Position);
	void ClearPoints();
protected:
	void InitializeTarget();
	void DrawRune();
	void DrawTile(FVector2D Point);

protected:
	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> RuneDisplayTarget;

	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> CanvasDisplayTarget;

	UPROPERTY()
	TObjectPtr<USpell> ActiveSpell;

	UPROPERTY()
	TArray<FVector2D> Points;

	float LineWidth = 50;
	float TileSize = 50;
	FVector2D AxisLine = FVector2D(1.0f, 0.0f);

	const float PointToLineThreshold = 0.01f;
	const float SegmentToPointsThreshold = 0.01f;
};
