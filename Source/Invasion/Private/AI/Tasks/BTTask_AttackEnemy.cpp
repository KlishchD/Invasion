#include "AI/Tasks/BTTask_AttackEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseSpiritCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AttackEnemy::UBTTask_AttackEnemy()
{
	NodeName = "Attack Enemy";
}

EBTNodeResult::Type UBTTask_AttackEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ABaseSpiritCharacter* SpiritCharacter = Cast<ABaseSpiritCharacter>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		SpiritCharacter->SetIsAttacking(true);
		UGameplayStatics::ApplyDamage(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyActorKey.SelectedKeyName)),	SpiritCharacter->GetSpiritDamage(), OwnerComp.GetAIOwner(), nullptr, UDamageType::StaticClass());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
