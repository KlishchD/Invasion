// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimInstance.h"
#include "AI/BaseEnemyAIController.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveLinearColor.h"
#include "GameFramework/Character.h"
#include "Invasion/InvasionCharacter.h"
#include "Widgets/HealthBar.h"
#include "BaseEnemyCharacter.generated.h"

struct FActorPerceptionUpdateInfo;
class UAIPerceptionComponent;

UENUM()
enum class EEnemyType
{
	Melee,
	Ranged
};

UCLASS()
class INVASION_API ABaseEnemyCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnActorPerceived(const FActorPerceptionUpdateInfo& ActorPerceptionInfo);

	UFUNCTION()
	void OnWeaponRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndWeaponRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float OnAttack();
	
	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Run();

	UFUNCTION(BlueprintCallable)
	void StopMovement();

	UFUNCTION(BlueprintNativeEvent)
	void OnDead();

	bool GetIsDead() const { return bIsDead; }

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAcess = "true"))
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap Components")
	TObjectPtr<USphereComponent> WeaponOverlapSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> WeaponStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaReward = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor HealthBarWidgetColor;
	
	UPROPERTY()
	TObjectPtr<ABaseEnemyAIController> EnemyAIController;

	UPROPERTY()
	TObjectPtr<UEnemyAnimInstance> AnimInstance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<AActor> PerceivedActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<AInvasionCharacter> MainCharacter;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float RunSpeed = 600.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float WalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 PlayersTeam = 255;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsInLineOfSight = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetLostTime;
	
private:
	float TargetLostElapsed = 0.f;
	float LastLocationElapsed = 0.f;
	float LastLocationUpdateTime = 0.5f;
};
