#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Subsystems/SpellDrawingSubsystem.h"
#include "SpellRuneDisplayWidget.generated.h"

UCLASS()
class INVASION_API USpellRuneDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	void CastSpell();

protected:
	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> RuneImageMaterial;

	UPROPERTY(EditAnywhere)
	FKey FinishKey;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> RuneImageDynamicMaterialInstance;

	UPROPERTY()
	TObjectPtr<USpellDrawingSubsystem> SpellDrawingSubsystem;

	UPROPERTY()
	bool bIsDrawing = false;
};
