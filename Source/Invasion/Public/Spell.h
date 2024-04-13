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
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Effect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundWave> FlyingSoundWave;

	UPROPERTY(EditDefaultsOnly)
	float Speed;

	UPROPERTY(EditDefaultsOnly)
	float Offset;
};
