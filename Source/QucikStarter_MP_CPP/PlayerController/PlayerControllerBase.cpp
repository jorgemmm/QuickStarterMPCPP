// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerControllerBase.h"
#include "QucikStarter_MP_CPP/QucikStarter_MP_CPPGameMode.h"
#include "QucikStarter_MP_CPP/QucikStarter_MP_CPPCharacter.h"
#include "QucikStarter_MP_CPP/PlayerState/QSPlayerState.h"

#include "GameFramework/Pawn.h"

#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"


//
//APlayerControllerBase::APlayerControllerBase(const FObjectInitializer& ObjectInitializer)
//{
//	Super::APlayerController();
//
//}

// Called when the game starts or when spawned
void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultInfoLevelWBP)
	{
		LevelInfoWBP = CreateWidget<UUserWidget>(GetWorld(), DefaultInfoLevelWBP);
		LevelInfoWBP->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No default InfoLevel Widget in Player Controller Base "));
	}


	/*FTimerHandle PlayerSpawnHandle;
	float DelayToSpawn = .5;

	GetWorldTimerManager().ClearTimer(PlayerSpawnHandle);
	GetWorldTimerManager().SetTimer(
		PlayerSpawnHandle,
		this,
		&APlayerControllerBase::ResPawn,
		DelayToSpawn, true, .5f);*/

}

void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled)
{
	if (SetPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
		bShowMouseCursor = true;
	}
	else
	{
		GetPawn()->DisableInput(this);
		bShowMouseCursor = false;
	}

}


void APlayerControllerBase::RequestRespawn_Implementation(int value)
{
	 UE_LOG(LogTemp, Warning, TEXT("Trying respawn a new Pawn"));
	//ServerRestartPlayer();
	//return;
	if (i <= 0)
	{
		return;
	}
	AQSPlayerState* QSPlayerState = Cast<AQSPlayerState>(GetPawn());
	if (QSPlayerState) 
		QSPlayerState->SetDeathTimes();
	
	AQucikStarter_MP_CPPGameMode* QSGameMode = Cast<AQucikStarter_MP_CPPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!QSGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("No game mode loaded in Player controller"));
		return;
	}



	QSGameMode->HandleRespawnPlayer(this);

}

//
//bool APlayerControllerBase::RequestRespawn_Validation(int value)
//{
//	return value >= 1;
//}

void APlayerControllerBase::ResPawn()
{
	//GetPawn()->Destroyed();

	/*
	if(IsLocalController())
	
	{
		UE_LOG(LogTemp, Warning, TEXT("Aplayercontroller is still defined in map"));
		
		
	}
	
	*/


	i = 1;
	if (!GetPawn() || GetPawn()->IsPendingKill())
	{
		/*PlayerState;
		AQSPlayerState* QSPlayerState = Cast<AQSPlayerState>(PlayerState);

		if (QSPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("Death Pawn is Counted!!!!"));
			QSPlayerState->SetDeathTimes();
		}*/

		UE_LOG(LogTemp, Warning, TEXT("Pawn is destroyed!!!!!!"));
		RequestRespawn(i);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn is still loaded in World"));
		
	}


	
	//--i;
	
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent(	);

	InputComponent->BindAction("Respawn", IE_Released, this, &APlayerControllerBase::ResPawn);


}
	


bool APlayerControllerBase::GetPawnIsAlive()
{
	
	if ( GetPawn() )
		return true;
	else
		return false;

	
	//bool bDead = false;
	////Client-specific functionality And no Simulated proxies only _Client connected
	//if( this->IsLocalController())
	//{
	//}
	//	AQucikStarter_MP_CPPCharacter* DefaultPawn = Cast<AQucikStarter_MP_CPPCharacter>( GetPawn() );
	//
	//	if(!DefaultPawn)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Pawn is not loaded in World"));
	//		return bDead;
	//	}
	//	bDead= DefaultPawn->bDead;
	//	return bDead;
	//
	//
	//return bDead;
	

}


void APlayerControllerBase::ShowInfoInWBP(FString NewText)
{
	//DefaultInfoLevelWBP->
	//OwnWidget->Text = NewText
}
