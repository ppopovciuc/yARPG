// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ARP.h"
#include "ARPGameMode.h"
#include "ARPPlayerController.h"
#include "ARPCharacter.h"

AARPGameMode::AARPGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AARPPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}