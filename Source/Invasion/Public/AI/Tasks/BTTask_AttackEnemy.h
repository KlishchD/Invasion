#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackEnemy.generated.h"

UCLASS()
class INVASION_API UBTTask_AttackEnemy : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackEnemy();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyActorKey;
};
