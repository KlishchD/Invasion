// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Projectiles/Projectile.h"
#include "Spell.h"
#include "SpellComponent.generated.h"

UCLASS()
class INVASION_API USpellComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USpellComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CastSpell();

	USpell* GetActiveSpell() const;
	void SetActiveSpell(int32 Index);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<USpell>> Spells;

	UPROPERTY()
	int32 ActiveSpellIndex = 0;
};
