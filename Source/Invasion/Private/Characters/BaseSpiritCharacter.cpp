#include "Characters/BaseSpiritCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


ABaseSpiritCharacter::ABaseSpiritCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}


void ABaseSpiritCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseSpiritCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
