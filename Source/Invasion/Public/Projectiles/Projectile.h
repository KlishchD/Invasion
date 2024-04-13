#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Spell.h"
#include "Projectile.generated.h"

UCLASS()
class INVASION_API AProjectile : public AActor
{
	GENERATED_BODY()
public:	
	AProjectile();

	void Initialize(USpell* InSpell);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> EffectNiagaraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> ColliderComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY()
	USpell* Spell;

	UPROPERTY()
	float Speed = 0.0f;
};
