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
		if(IsValid(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyActorKey.SelectedKeyName))))
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyActorKey.SelectedKeyName))->GetName())
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NOT WORK"))
		}
		SpiritCharacter->SetIsAttacking(true);
		UGameplayStatics::ApplyDamage(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyActorKey.SelectedKeyName)),	SpiritCharacter->GetSpiritDamage(), OwnerComp.GetAIOwner(), nullptr, UDamageType::StaticClass());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
