#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseSpiritCharacter.generated.h"

UCLASS()
class INVASION_API ABaseSpiritCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseSpiritCharacter();
	virtual void Tick(float DeltaTime) override;

	UBehaviorTree* GetMainBehaviorTree() const { return MainBehaviorTree; };

protected:
	virtual void BeginPlay() override;

	void FollowOwner();

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> MainBehaviorTree;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	UPROPERTY()
	TObjectPtr<class ABaseSpiritAIController> BaseSpiritAIController;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	float CurrentHealth;
	
	UPROPERTY(EditDefaultsOnly)
	float DistanceToFollow = 200.0f;

	bool bIsAlive = true;
	float Timer = 0.0f;

};
