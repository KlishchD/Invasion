#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindEnemy.generated.h"

UCLASS()
class INVASION_API UBTService_FindEnemy : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_FindEnemy();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyActorKey;
};
