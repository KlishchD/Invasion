// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	float GetPatrolRadius() const { return PatrolRadius; }
	const FName& GetPatrolTargetKey() const { return PatrolTargetKey; }
	const FName& GetIsInLineOfSightKey() const { return IsInLineOfSightKey; }
	const FName& GetDetectedCharacter() const { return DetectedCharacterKey; }
	const FName& GetLastKnownLocationKey() const { return LastKnowsLocationKey; }
	const FName& GetIsInWeaponRadius() const { return IsInWeaponRadius; }
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PatrolRadius;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName PatrolTargetKey;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName IsInLineOfSightKey;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName DetectedCharacterKey;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName LastKnowsLocationKey;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName IsInWeaponRadius;
};
