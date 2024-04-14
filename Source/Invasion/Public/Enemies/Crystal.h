#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Crystal.generated.h"

class USpell;

UCLASS()
class INVASION_API ACrystal : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrystal();

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPointLightComponent> PointLightComponent;

	UPROPERTY(EditAnywhere)
	AActor* PortalActor;

	UPROPERTY(EditAnywhere)
	USpell* EffectiveSpell;
};
