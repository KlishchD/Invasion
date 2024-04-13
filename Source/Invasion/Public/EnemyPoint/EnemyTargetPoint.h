// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemyCharacter.h"
#include "Engine/TargetPoint.h"
#include "EnemyTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AEnemyTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseEnemyCharacter> EnemyToSpawn;
};
