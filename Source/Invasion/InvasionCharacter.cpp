// Copyright Epic Games, Inc. All Rights Reserved.

#include "InvasionCharacter.h"
#include "InvasionProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInvasionCharacter

AInvasionCharacter::AInvasionCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	SpellComponent = CreateDefaultSubobject<USpellComponent>(TEXT("SpellComponent"));
	SpellComponent->SetupAttachment(GetCapsuleComponent());
}

void AInvasionCharacter::BeginPlay()
{
	Health = MaxHealth;
	Mana = MaxMana;

	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	StatusWidget = CreateWidget<UStatusWidget>(GetWorld(), StatusWidgetClass);
	StatusWidget->AddToViewport();
}

//////////////////////////////////////////////////////////////////////////// Input

void AInvasionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInvasionCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInvasionCharacter::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AInvasionCharacter::StartSpellCasting);
		
		EnhancedInputComponent->BindAction(SpellScrollAction, ETriggerEvent::Triggered, this, &AInvasionCharacter::ScrollSpell);

		EnhancedInputComponent->BindAction(SummonSpiritAction, ETriggerEvent::Triggered, this, &AInvasionCharacter::SummonSpirit);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AInvasionCharacter::SwitchToUIMode()
{
	APlayerController* PC = GetController<APlayerController>();
	PC->SetShowMouseCursor(true);
	PC->SetInputMode(FInputModeUIOnly());
}

void AInvasionCharacter::SwitchToGameMode()
{
	APlayerController* PC = GetController<APlayerController>();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
}

void AInvasionCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AInvasionCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AInvasionCharacter::StartSpellCasting(const FInputActionValue& Value)
{
	SpellRuneDisplayWidget = CreateWidget<USpellRuneDisplayWidget>(GetWorld(), SpellRuneDisplayWidgetClass);
	SpellRuneDisplayWidget->AddToViewport();

	SwitchToUIMode();
}

void AInvasionCharacter::CastSpell(float Strenght)
{
	AddManaOffset(-SpellComponent->GetActiveSpell()->ManaCost * Strenght);

	SpellComponent->CastSpell(Strenght);

	SwitchToGameMode();
}

void AInvasionCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AInvasionCharacter::GetHasRifle()
{
	return bHasRifle;
}

float AInvasionCharacter::GetHealth() const
{
	return Health;
}

float AInvasionCharacter::GetMana() const
{
	return Mana;
}

float AInvasionCharacter::GetHealthNormalized() const
{
	return Health / MaxHealth;
}

float AInvasionCharacter::GetManaNormalized() const
{
	return Mana / MaxMana;
}

void AInvasionCharacter::ScrollSpell(const FInputActionValue& Value)
{
	SpellComponent->MoveActiveSpellIndex(Value.Get<float>() > 0 ? 1 : -1);
}

void AInvasionCharacter::SummonSpirit()
{
	if (!SpiritCharacter || !SpiritCharacter->IsAlive())
	{
		AddManaOffset(-SpiritSpawnManaCost);

		FActorSpawnParameters parameters = FActorSpawnParameters();
		parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpiritCharacter = GetWorld()->SpawnActor<ABaseSpiritCharacter>(SpiritCharacterClass, GetActorLocation() + GetActorForwardVector() * SpiritSpawnOffset, (GetActorForwardVector()).ToOrientationRotator(), parameters);
	}
}

float AInvasionCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	AddHealthOffset(-DamageAmount);

	return DamageAmount;
}

void AInvasionCharacter::AddHealthOffset(float Offset)
{
	Health += Offset;
	OnHealthChnaged.Broadcast(Health);
}

void AInvasionCharacter::AddManaOffset(float Offset)
{
	Mana += Offset;
	OnManaChanged.Broadcast(Mana);
}
