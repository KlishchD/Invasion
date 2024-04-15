// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemyCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Widgets/HealthBar.h"

// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMeshComponent"));
	WeaponStaticMeshComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	WeaponOverlapSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponOverlapSphereComponent"));
	WeaponOverlapSphereComponent->InitSphereRadius(100.f);
	WeaponOverlapSphereComponent->SetupAttachment(RootComponent);

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ThisClass::OnActorPerceived);

	WeaponOverlapSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponRadiusOverlap);
	WeaponOverlapSphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnEndWeaponRadiusOverlap);
	WeaponStaticMeshComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponOverlap);
	
	EnemyAIController = Cast<ABaseEnemyAIController>(GetController());

	AnimInstance = CastChecked<UEnemyAnimInstance>( GetMesh() ? GetMesh()->GetAnimInstance() : nullptr );

	if (HealthBar && HealthBar->GetWidget())
	{
		HealthBarWidget = Cast<UHealthBar>(HealthBar->GetWidget());

		if (HealthBarWidget)
		{
			HealthBarWidget->GetHealthBar()->SetFillColorAndOpacity(HealthBarWidgetColor);
		}
	}
}

void ABaseEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PerceptionComponent->OnTargetPerceptionInfoUpdated.RemoveAll(this);

	WeaponStaticMeshComponent->OnComponentBeginOverlap.RemoveAll(this);
	WeaponOverlapSphereComponent->OnComponentBeginOverlap.RemoveAll(this);
	WeaponOverlapSphereComponent->OnComponentEndOverlap.RemoveAll(this);
}

void ABaseEnemyCharacter::OnActorPerceived(const FActorPerceptionUpdateInfo& ActorPerceptionInfo)
{
	bIsInLineOfSight = ActorPerceptionInfo.Stimulus.WasSuccessfullySensed();

	if (EnemyAIController)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(EnemyAIController->GetIsInLineOfSightKey(), bIsInLineOfSight);
	
		if (bIsInLineOfSight)
		{
			PerceivedActor = ActorPerceptionInfo.Target.Get();
			MainCharacter = Cast<AInvasionCharacter>(PerceivedActor);
			TargetLostElapsed = 0.f;

			EnemyAIController->GetBlackboardComponent()->SetValueAsObject(EnemyAIController->GetDetectedCharacter(), PerceivedActor);
			EnemyAIController->GetBlackboardComponent()->SetValueAsVector(EnemyAIController->GetLastKnownLocationKey(), PerceivedActor->GetActorLocation());
		}
	}
}

void ABaseEnemyCharacter::OnWeaponRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AInvasionCharacter>(OtherActor))
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(EnemyAIController->GetIsInWeaponRadius(), true);
	}
}

void ABaseEnemyCharacter::OnEndWeaponRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AInvasionCharacter>(OtherActor))
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(EnemyAIController->GetIsInWeaponRadius(), false);
	}
}

void ABaseEnemyCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AInvasionCharacter>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
	}
}

float ABaseEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	if (HealthBarWidget)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		HealthBarWidget->GetHealthBar()->SetPercent(HealthPercent);
		HealthBarWidget->SetText(FText::AsNumber(CurrentHealth));
	}

	if (CurrentHealth <= 0.f)
	{
		EnemyAIController->UnPossess();
		HealthBar->SetHiddenInGame(true);
		SetActorEnableCollision(false);
		bIsDead = true;

		if (Cast<AInvasionCharacter>(DamageCauser))
		{
			MainCharacter->AddManaOffset(ManaReward);
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyAIController)
	{		
		if (bIsInLineOfSight && (LastLocationElapsed += DeltaTime) >= LastLocationUpdateTime)
		{
			LastLocationElapsed = 0.f;
			EnemyAIController->GetBlackboardComponent()->SetValueAsVector(EnemyAIController->GetLastKnownLocationKey(), PerceivedActor->GetActorLocation());
		}

		if (!bIsInLineOfSight && PerceivedActor && (TargetLostElapsed += DeltaTime) >= TargetLostTime)
		{
			PerceivedActor = nullptr;
			TargetLostElapsed = 0.f;
			EnemyAIController->GetBlackboardComponent()->SetValueAsObject(EnemyAIController->GetDetectedCharacter(), PerceivedActor);
		}
	}
}

float ABaseEnemyCharacter::OnAttack()
{
	StopMovement();
	AnimInstance->OnAttack();
	return AnimInstance->GetAttackMontageLength();
}

void ABaseEnemyCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABaseEnemyCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ABaseEnemyCharacter::StopMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
}

ETeamAttitude::Type ABaseEnemyCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<AInvasionCharacter>(&Other);
	return OtherTeamAgent && OtherTeamAgent->GetGenericTeamId()== PlayersTeam ? ETeamAttitude::Hostile : ETeamAttitude::Neutral;
}

