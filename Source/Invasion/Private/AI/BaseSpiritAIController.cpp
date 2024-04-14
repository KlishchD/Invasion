#include "AI/BaseSpiritAIController.h"

#include "AI/Components/BaseAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseSpiritCharacter.h"

ABaseSpiritAIController::ABaseSpiritAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UBaseAIPerceptionComponent>(TEXT("PerceptionComponent"));

	SetPerceptionComponent(*AIPerceptionComponent);
}

void ABaseSpiritAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(ABaseSpiritCharacter* BaseSpiritCharacter = Cast<ABaseSpiritCharacter>(InPawn))
	{
		RunBehaviorTree(BaseSpiritCharacter->GetMainBehaviorTree());
	}
}

void ABaseSpiritAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetFocus(GetFocusOnActor());
}

AActor* ABaseSpiritAIController::GetFocusOnActor()
{
	if(GetBlackboardComponent())
	{
		return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
	}
	return nullptr;
}
