#include "AI/Services/BTService_FindOwner.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindOwner::UBTService_FindOwner()
{
	NodeName = "Find Owner";
}

void UBTService_FindOwner::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if(BlackboardComponent)
	{
		if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			BlackboardComponent->SetValueAsVector(OwnerActorKey.SelectedKeyName, Character->GetActorLocation());
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
