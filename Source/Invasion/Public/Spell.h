// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "Spell.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API USpell : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> Effect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<USoundWave> FlyingSoundWave;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	float Offset;

	UPROPERTY(EditDefaultsOnly, Category = "Runes")
	TArray<FVector2D> RunePoints;
	
	UPROPERTY(EditDefaultsOnly, Category = "Runes")
	TObjectPtr<UMaterialInterface> ExplanationRuneMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Runes")
	TObjectPtr<UMaterialInterface> DrawRuneMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float ActiveRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float ManaCost;

	UPROPERTY(EditDefaultsOnly, Category = "Element")
	FLinearColor ElementColor;
};
