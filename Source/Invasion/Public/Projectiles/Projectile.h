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

	void Initialize(USpell* InSpell, float InStrength);
	virtual void Tick(float DeltaTime) override;

	USpell* GetSpell() const;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> EffectNiagaraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY()
	USpell* Spell;

	UPROPERTY()
	float Speed = 0.0f;

	UPROPERTY()
	float Strength = 1.0f;
};
