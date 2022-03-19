// Copyright Epic Games, Inc. All Rights Reserved.

#include "QucikStarter_MP_CPPGameMode.h"
#include "QucikStarter_MP_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "GameFramework/PlayerController.h"
//#include "QucikStarter_MP_CPP/Characters/QucikStarter_MP_CPPCharacter.h"
#include "QucikStarter_MP_CPPCharacter.h"

#include "GameFramework/PlayerStart.h"

AQucikStarter_MP_CPPGameMode::AQucikStarter_MP_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AQucikStarter_MP_CPPGameMode::BeginPlay()
{
	GetPlayerStart();

}

void AQucikStarter_MP_CPPGameMode::GetPlayerStart()
{
	// Get references and game win/lose conditions.
	TSubclassOf<APlayerStart> ClassToFind;
	ClassToFind = APlayerStart::StaticClass();

	//TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, OUT PlayerStarts);
	
	
	
	


}

/** Esta función llamada por el player controller una vez muera el player*/
void AQucikStarter_MP_CPPGameMode::HandleRespawnPlayer_Implementation(APlayerController* PC)
{

	if (!PC || PlayerStarts.Num() <=0)
	{
		UE_LOG(LogTemp, Error, TEXT("No player Controller loaded, or Player Start no placed"))
		return;
	}
	//FMath::RandInit(32);

	int length = PlayerStarts.Num();
	int nextPS = FMath::RandRange(0, length - 1);
	
	FVector  Location= PlayerStarts[nextPS]->GetActorLocation(); 
	FRotator Rotator = PlayerStarts[nextPS]->GetActorRotation();
	
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //TL
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;
	
	AQucikStarter_MP_CPPCharacter* DefaultPawn = GetWorld()->SpawnActor<AQucikStarter_MP_CPPCharacter>(
		DefaultPawnClass,// CharacterClass//ProjectileClass,
		Location, Rotator,	spawnParameters);

	if(!DefaultPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("New player Pawn doesn´t Spawn something wrong happen in Game Mode HandleRespawnPlayer"))
		return;
	}

	PC->Possess(DefaultPawn);

	

}


