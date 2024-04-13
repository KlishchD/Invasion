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

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> MainBehaviorTree;

};
