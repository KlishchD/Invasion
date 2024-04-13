#include "Projectiles/Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	EffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectComponent"));
	EffectNiagaraComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	ColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderComponent"));
	ColliderComponent->SetupAttachment(RootComponent);

	ColliderComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	ColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AProjectile::Initialize(USpell* InSpell)
{
	Spell = InSpell;

	EffectNiagaraComponent->SetAsset(Spell->Effect);
	EffectNiagaraComponent->Activate();

	AudioComponent->SetSound(Spell->FlyingSoundWave);
	AudioComponent->Play();

	Speed = Spell->Speed;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	FVector Offset = Speed * GetActorForwardVector();
	SetActorLocation(Location + Offset);
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ColliderComponent->OnComponentBeginOverlap.RemoveAll(this);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EffectNiagaraComponent->DeactivateImmediate();
	EffectNiagaraComponent->SetHiddenInGame(true);

	AudioComponent->Stop();

	ColliderComponent->OnComponentBeginOverlap.RemoveAll(this);
}

