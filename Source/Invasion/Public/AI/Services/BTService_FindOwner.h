#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindOwner.generated.h"

UCLASS()
class INVASION_API UBTService_FindOwner : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindOwner();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector OwnerActorKey;
	
};
