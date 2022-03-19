// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QucikStarter_MP_CPPGameMode.generated.h"

class APlayerController;

UCLASS(minimalapi)
class AQucikStarter_MP_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQucikStarter_MP_CPPGameMode();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	void GetPlayerStart();


public:

	TArray<AActor*> PlayerStarts;

	UFUNCTION(Server, Reliable)
		void HandleRespawnPlayer(APlayerController* PController);

};



