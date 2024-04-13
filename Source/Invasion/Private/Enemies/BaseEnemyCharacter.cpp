// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemyCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphereComponent"));
	OverlapSphereComponent->InitSphereRadius(200.0f);
	OverlapSphereComponent->SetupAttachment(RootComponent);

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ThisClass::OnActorPerceived);
	
	OverlapSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponRadiusOverlap);
	OverlapSphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnEndWeaponRadiusOverlap);
	EnemyAIController = Cast<ABaseEnemyAIController>(GetController());
}

void ABaseEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PerceptionComponent->OnTargetPerceptionInfoUpdated.RemoveAll(this);
	
	OverlapSphereComponent->OnComponentBeginOverlap.RemoveAll(this);
	OverlapSphereComponent->OnComponentEndOverlap.RemoveAll(this);
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
	if (MainCharacter)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(EnemyAIController->GetIsInWeaponRadius(), true);
	}
}

void ABaseEnemyCharacter::OnEndWeaponRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(EnemyAIController->GetIsInWeaponRadius(), false);
	}
}

// Called every frame
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

void ABaseEnemyCharacter::Walk()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABaseEnemyCharacter::Run()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ABaseEnemyCharacter::StopMovement()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
}

ETeamAttitude::Type ABaseEnemyCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<AInvasionCharacter>(&Other);
	return OtherTeamAgent && OtherTeamAgent->GetGenericTeamId()== PlayersTeam ? ETeamAttitude::Hostile : ETeamAttitude::Neutral;
}

