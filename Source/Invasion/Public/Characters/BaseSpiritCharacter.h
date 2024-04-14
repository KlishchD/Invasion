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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UBehaviorTree* GetMainBehaviorTree() const { return MainBehaviorTree; };

	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsAttacking();

	void SetIsAttacking(bool InbIsAttacking) { bIsAttacking = InbIsAttacking; }

	float GetSpiritDamage() const { return SpiritDamage; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> MainBehaviorTree;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	float FadeInTime = 3.0f;

	UPROPERTY(EditAnywhere)
	float FadeOutTime = 3.0f;

	UPROPERTY(EditAnywhere)
	float SpiritDamage = 10.0f;

	UPROPERTY()
	TObjectPtr<class ABaseSpiritAIController> BaseSpiritAIController;
	
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> Materials;
	
	float FadeInTimer = 0.0f;
	float FadeOutTimer = 0.0f;

	float CurrentHealth;
	
	bool bIsAlive = true;
	float Timer = 0.0f;

	bool bIsAttacking = false;
};
