#include "AI/BaseSpiritAIController.h"

#include "Characters/BaseSpiritCharacter.h"

void ABaseSpiritAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	 if(ABaseSpiritCharacter* BaseSpiritCharacter = Cast<ABaseSpiritCharacter>(InPawn))
	 {
	 	RunBehaviorTree(BaseSpiritCharacter->GetMainBehaviorTree());
	 }
}
