// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemyCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "CaveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API ACaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABaseEnemyCharacter> EnemyCharacterClass;
};
