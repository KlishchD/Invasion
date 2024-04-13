// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CaveGameMode.h"

#include "EnemyPoint/EnemyTargetPoint.h"
#include "Kismet/GameplayStatics.h"

void ACaveGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> EnemySpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyTargetPoint::StaticClass(), EnemySpawnPoints);

	for (const AActor* SpawnPoint : EnemySpawnPoints)
	{
		if (const AEnemyTargetPoint* SpawnEnemyTargetPoint = Cast<AEnemyTargetPoint>(SpawnPoint); SpawnEnemyTargetPoint->EnemyToSpawn)
		{
			GetWorld()->SpawnActor<ABaseEnemyCharacter>(SpawnEnemyTargetPoint->EnemyToSpawn, SpawnEnemyTargetPoint->GetTransform());
		}
	}
}
