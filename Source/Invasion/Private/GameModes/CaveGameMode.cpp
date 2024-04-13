// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CaveGameMode.h"

#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void ACaveGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> EnemySpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), EnemySpawnPoints);

	for (const AActor* SpawnPoint : EnemySpawnPoints)
	{
		GetWorld()->SpawnActor<ABaseEnemyCharacter>(EnemyCharacterClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
	}
}
