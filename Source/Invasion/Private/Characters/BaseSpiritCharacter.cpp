#include "Characters/BaseSpiritCharacter.h"

#include "AIController.h"
#include "AI/BaseSpiritAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/AITask_MoveTo.h"


ABaseSpiritCharacter::ABaseSpiritCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


void ABaseSpiritCharacter::BeginPlay()
{
	Super::BeginPlay();
	BaseSpiritAIController = Cast<ABaseSpiritAIController>(GetController());
	CurrentHealth = MaxHealth;
}

void ABaseSpiritCharacter::FollowOwner()
{
	BaseSpiritAIController->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABaseSpiritCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(FVector::Distance(BaseSpiritAIController->GetPawn()->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()) > DistanceToFollow)
	{
		//FollowOwner();
	}

	if(bIsAlive == true)
	{
		if(CurrentHealth <= 0.0f)
		{
			bIsAlive = false;
			GetCharacterMovement()->DisableMovement();
			PlayAnimMontage(DeathAnimMontage);
			SetLifeSpan(5.0f);
		}
		else
		{
			Timer+=DeltaTime;
			if(Timer >= 1.0f)
			{
				CurrentHealth--;
				Timer = 0.0f;
				UE_LOG(LogTemp, Error, TEXT("CurrentHealth: %f"), CurrentHealth);
			}
		}
	}
}
