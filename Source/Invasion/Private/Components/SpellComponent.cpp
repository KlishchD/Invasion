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

void USpellComponent::CastSpell()
{
	if (AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass))
	{
		USpell* Spell = GetActiveSpell();

		Projectile->SetActorLocation(GetComponentLocation() + GetForwardVector() * Spell->Offset);
		Projectile->SetActorRotation(GetComponentRotation());
		Projectile->Initialize(Spell);
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
	}
}

