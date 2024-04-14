#include "AI/Services/BTService_FindEnemy.h"
#include "AI/BaseSpiritAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FindEnemy::UBTService_FindEnemy()
{
	NodeName = TEXT("Find Enemy");
}

void UBTService_FindEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		ABaseSpiritAIController* Controller = Cast<ABaseSpiritAIController>(OwnerComp.GetAIOwner());
		if(UBaseAIPerceptionComponent* BaseAIPerceptionComponent = Controller->GetBaseAIPerceptionComponent())
		{
			BlackboardComponent->SetValueAsObject(EnemyActorKey.SelectedKeyName, BaseAIPerceptionComponent->GetClosestEnemy());
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
