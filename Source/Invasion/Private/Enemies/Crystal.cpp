#include "Enemies/Crystal.h"
#include "Projectiles/Projectile.h"

ACrystal::ACrystal()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(RootComponent);
}

void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
	{
		UMaterialInstanceDynamic* Material = MeshComponent->CreateAndSetMaterialInstanceDynamic(i);
		Material->SetVectorParameterValue(TEXT("ElementColor"), EffectiveSpell->ElementColor);
	}

	PointLightComponent->SetLightColor(EffectiveSpell->ElementColor);
}

float ACrystal::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (AProjectile* Projectile = Cast<AProjectile>(DamageCauser))
	{
		if (Projectile->GetSpell() == EffectiveSpell)
		{
			if (PortalActor)
			{
				PortalActor->SetActorHiddenInGame(false);
			}

			MeshComponent->SetHiddenInGame(true);
			MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PointLightComponent->SetIntensity(0.0f);
		}
	}

	return 0.0f;
}

