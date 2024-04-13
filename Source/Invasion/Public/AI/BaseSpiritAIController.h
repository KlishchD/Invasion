#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseSpiritAIController.generated.h"


UCLASS()
class INVASION_API ABaseSpiritAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
};
