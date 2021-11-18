// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameGameMode.h"
#include "SurvivalGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASurvivalGameGameMode::ASurvivalGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
