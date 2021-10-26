// Copyright Epic Games, Inc. All Rights Reserved.

#include "WhatIsReplayGameMode.h"
#include "WhatIsReplayHUD.h"
#include "WhatIsReplayCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWhatIsReplayGameMode::AWhatIsReplayGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AWhatIsReplayHUD::StaticClass();
}
