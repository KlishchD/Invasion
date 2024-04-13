// Copyright Epic Games, Inc. All Rights Reserved.

#include "InvasionGameMode.h"
#include "InvasionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInvasionGameMode::AInvasionGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
