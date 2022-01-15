// Copyright Epic Games, Inc. All Rights Reserved.

#include "QucikStarter_MP_CPPGameMode.h"
#include "QucikStarter_MP_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQucikStarter_MP_CPPGameMode::AQucikStarter_MP_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
