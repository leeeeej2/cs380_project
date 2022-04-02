// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacingLineGameMode.h"
#include "RacingLinePlayerController.h"
#include "RacingLineCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARacingLineGameMode::ARacingLineGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARacingLinePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}