#include "AI/Components/BaseAIPerceptionComponent.h"

#include "AI/BaseSpiritAIController.h"
#include "Characters/BaseSpiritCharacter.h"
#include "Enemies/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Sight.h"

AActor* UBaseAIPerceptionComponent::GetClosestEnemy()
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	if(PerceiveActors.Num() == 0) return nullptr;

	ABaseSpiritAIController* AIController = Cast<ABaseSpiritAIController>(GetOwner());
	if(!IsValid(AIController)) return nullptr;

	ABaseSpiritCharacter* Character = Cast<ABaseSpiritCharacter>(AIController->GetPawn());
	if(!IsValid(Character)) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestCharacter = nullptr;

	for(AActor* PerceiveActor : PerceiveActors)
	{
		if(PerceiveActor != UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) && !Cast<ABaseEnemyCharacter>(PerceiveActor)->GetIsDead())
		{
			float CurrentDistance = (PerceiveActor->GetActorLocation() - Character->GetActorLocation()).Size();
			if(CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestCharacter = PerceiveActor;
			}
		}
	}
	if(!IsValid(BestCharacter))
	{
		Character->SetIsAttacking(false);
	}
	return BestCharacter;
	
}
