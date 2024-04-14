#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/BaseAIPerceptionComponent.h"
#include "BaseSpiritAIController.generated.h"


UCLASS()
class INVASION_API ABaseSpiritAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseSpiritAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	UBaseAIPerceptionComponent* GetBaseAIPerceptionComponent() const { return AIPerceptionComponent; }

protected:
	AActor* GetFocusOnActor();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBaseAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly)
	FName FocusOnKeyName = "EnemyActor";
};
