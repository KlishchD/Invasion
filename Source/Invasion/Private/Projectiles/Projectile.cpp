#include "Projectiles/Projectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	EffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectComponent"));
	EffectNiagaraComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AProjectile::Initialize(USpell* InSpell, float InStrength)
{
	Spell = InSpell;

	EffectNiagaraComponent->SetAsset(Spell->Effect);
	EffectNiagaraComponent->Activate();

	AudioComponent->SetSound(Spell->FlyingSoundWave);
	AudioComponent->Play();

	Speed = Spell->Speed;

	Strength = InStrength;

	SetActorTickEnabled(true);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	FVector Offset = Speed * GetActorForwardVector();
	SetActorLocation(Location + Offset);

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(this, Location, Location + Offset, Spell->ActiveRadius, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, { GetOwner<APlayerController>()->GetPawn() }, EDrawDebugTrace::None, HitResults, true);

	for (FHitResult& HitResult : HitResults)
	{
		if (AActor* Actor = HitResult.GetActor())
		{
			UGameplayStatics::ApplyDamage(Actor, Spell->Damage * Strength, GetOwner<AController>(), this, UDamageType::StaticClass());
		}
	}

	if (HitResults.Num())
	{
		EffectNiagaraComponent->SetHiddenInGame(true);

		AudioComponent->Stop();

		SetActorTickEnabled(false);
	}
}

USpell* AProjectile::GetSpell() const
{
	return Spell;
}
