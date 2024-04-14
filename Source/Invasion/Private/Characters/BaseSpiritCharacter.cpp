#include "Characters/BaseSpiritCharacter.h"
#include "AIController.h"
#include "AI/BaseSpiritAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/AITask_MoveTo.h"
#include "Components/CapsuleComponent.h"

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

	USkeletalMeshComponent* MeshComponent = GetMesh();
	for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
	{
		UMaterialInstanceDynamic* Material = MeshComponent->CreateAndSetMaterialInstanceDynamic(i);
		Material->SetScalarParameterValue(TEXT("Opacity"), 0.0f);
		Materials.Add(Material);
	}

	FadeInTimer = FadeInTime;
}

void ABaseSpiritCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlive)
	{
		if (FadeInTimer >= 0)
		{
			float Opacity = (FadeInTime - FadeInTimer) / FadeInTime;
			for (UMaterialInstanceDynamic* Material : Materials)
			{
				Material->SetScalarParameterValue(TEXT("Opacity"), Opacity);
			}

			FadeInTimer -= DeltaTime;
		}

		if(CurrentHealth <= 0.0f)
		{
			FadeOutTimer = FadeOutTime;

			bIsAlive = false;
			GetCharacterMovement()->DisableMovement();
			PlayAnimMontage(DeathAnimMontage);
		}
		else
		{
			Timer += DeltaTime;
			
			if (Timer >= 1.0f)
			{
				CurrentHealth--;
				Timer = 0.0f;
				UE_LOG(LogTemp, Error, TEXT("CurrentHealth: %f"), CurrentHealth);
			}
		}
	}
	else
	{
		if (FadeOutTimer >= 0)
		{
			float Opacity = FadeOutTimer / FadeOutTime;
			for (UMaterialInstanceDynamic* Material : Materials)
			{
				Material->SetScalarParameterValue(TEXT("Opacity"), Opacity);
			}

			FadeOutTimer -= DeltaTime;

			if (FadeOutTimer <= 0)
			{
				for (UMaterialInstanceDynamic* Material : Materials)
				{
					Material->SetScalarParameterValue(TEXT("Opacity"), 0.0f);
				}
			}
		}
		else
		{
			SetLifeSpan(5.0f);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

bool ABaseSpiritCharacter::IsAlive() const
{
	return bIsAlive;
}

bool ABaseSpiritCharacter::IsAttacking()
{
	return bIsAttacking;
}
