// Fill out your copyright notice in the Description page of Project Settings.


//#include "PlayerState/QSPlayerState.h"
#include "QSPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

#include "DrawDebugHelpers.h"

/*
#include <iostream>

using namespace std;

class Account {

public:
	//Atributes
	std::string Name;
	double Balance =30000.f;

	//Methods
	bool WithDraws(double amt) {
		Balance -= amt;
	}
	bool Deposit(double amt) {
		Balance += amt;
	}

};*/

const int32 MAX_VAL = 2147483647;

AQSPlayerState::AQSPlayerState(const FObjectInitializer& ObjectInitializer)
{
	
	/*Account* Jordi_Account = new Account;

	Jordi_Account->Name = "Jordi";*/

	InfoMessage = TEXT("Damage Message");
}



void AQSPlayerState::OnRep_KillPoints()
{
	//Info que quiero que sereplique en resto de clientes desde el servidor
	if (GetLocalRole() < ROLE_Authority)
	{
		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(100.f);
		DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Red, false,.2f);
	}

}

void AQSPlayerState::OnRep_Deaths()
{
	//Info que quiero que sereplique en resto de clientes desde el servidor
	if (GetLocalRole() < ROLE_Authority)
	{
		//FCollisionShape MyColSphere = FCollisionShape::MakeSphere(100.f);
		//DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Red, false, .2f);
	}

}

/** Almacenamos puntución y reseteamos killpoints*/
void AQSPlayerState::ResetKillPoints()
{
	//Sólo en clientes conectados
	if(GetLocalRole()<ROLE_Authority)
	{
	}
		const float score = .1f * GetKillPoints();
		SetScore(score);
		killPoints = 0;
	
	
}

/** Actualizamos Killpoints*/
void AQSPlayerState::SetKillPoints(int32 DeltaKillPoints)
{
	
	killPoints = FMath::Clamp( killPoints + DeltaKillPoints , 0, MAX_VAL);
	
	//killPoints += DeltaKillPoints;

}
/** Obtenermos Killpoints*/
int32 AQSPlayerState::GetKillPoints() const
{
	return killPoints;
}

void AQSPlayerState::SetDeathTimes()
{

	DeathTimes = FMath::Clamp(++DeathTimes, 0, MAX_VAL);
}

void AQSPlayerState::SetInfoMessage(FString NewMessage)
{
	InfoMessage = NewMessage;
}

int32 AQSPlayerState::GetDeathTimes() const
{
	return DeathTimes;
}

uint8 AQSPlayerState::GetPawnPing() const
{
	return GetPing();
}

FString AQSPlayerState::GetInfoMessage() const
{
	return InfoMessage;
}




void AQSPlayerState::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current killPoints 
	
	DOREPLIFETIME(AQSPlayerState, killPoints);
	DOREPLIFETIME(AQSPlayerState, DeathTimes);
	DOREPLIFETIME(AQSPlayerState, InfoMessage);
	
	/*DOREPLIFETIME(AQSPlayerState, bDead);
	DOREPLIFETIME(AQSPlayerState, CurrentWeaponRef);*/

}



