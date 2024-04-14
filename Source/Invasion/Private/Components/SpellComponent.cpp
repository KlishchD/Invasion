#include "Components/SpellComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

USpellComponent::USpellComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpellComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USpellComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpellComponent::CastSpell(float Strength)
{
	if (!FMath::IsNearlyZero(Strength))
	{
		FActorSpawnParameters Parameters;
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Parameters.Owner = GetOwner<APawn>()->GetController();

		USpell* Spell = GetActiveSpell();

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetComponentLocation() + GetForwardVector() * Spell->Offset, GetComponentRotation(), Parameters);
		if (Projectile)
		{
			Projectile->Initialize(Spell, Strength);
		}
	}
}

USpell* USpellComponent::GetActiveSpell() const
{
	return Spells[ActiveSpellIndex];
}

void USpellComponent::SetActiveSpell(int32 Index)
{
	if (Spells.IsValidIndex(Index))
	{
		ActiveSpellIndex = Index;
		OnActiveSpellChanged.Broadcast(Spells[ActiveSpellIndex]);
	}
}

void USpellComponent::MoveActiveSpellIndex(int32 Offset)
{
	int32 NewActiveSpellIndex = (ActiveSpellIndex + Offset + Spells.Num()) % Spells.Num();
	SetActiveSpell(NewActiveSpellIndex);
}

