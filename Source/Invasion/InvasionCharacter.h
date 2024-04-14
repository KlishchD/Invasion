// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/SpellComponent.h"
#include "Widgets/SpellRuneDisplayWidget.h"
#include "Widgets/StatusWidget.h"
#include "Characters/BaseSpiritCharacter.h"
#include "InvasionCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChnaged, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnManaChanged, float)

UCLASS(config=Game)
class AInvasionCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpellComponent> SpellComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SpellScrollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SummonSpiritAction;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellRuneDisplayWidget> SpellRuneDisplayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UStatusWidget> StatusWidgetClass;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere)
	float SpiritSpawnOffset = 500.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseSpiritCharacter> SpiritCharacterClass;

	UPROPERTY()
	TObjectPtr<USpellRuneDisplayWidget> SpellRuneDisplayWidget;
	
	UPROPERTY()
	TObjectPtr<UStatusWidget> StatusWidget;

	UPROPERTY()
	TObjectPtr<ABaseSpiritCharacter> SpiritCharacter;

	FOnHealthChnaged OnHealthChnaged;
	FOnManaChanged OnManaChanged;

	float Health;
	float Mana;
public:
	AInvasionCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ScrollSpell(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	USpellComponent* GetSpellComponent() const { return SpellComponent; }

	void StartSpellCasting(const FInputActionValue& Value);
	void CastSpell(float Strength);

	void SwitchToUIMode();
	void SwitchToGameMode();

	FOnHealthChnaged& GetOnHealthChnaged() { return OnHealthChnaged; }
	FOnManaChanged& GetOnManaChanged() { return OnManaChanged; }

	float GetHealth() const;
	float GetMana() const;

	float GetHealthNormalized() const;
	float GetManaNormalized() const;

	void SummonSpirit();
};

