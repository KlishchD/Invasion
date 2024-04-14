// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UProgressBar* GetHealthBar() const { return HealthBar; }
	void SetText(const FText& InText) { HealthPoints->SetText(InText); }
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthPoints;
};
