// Fill out your copyright notice in the Description page of Project Settings.


//#include "Actors/QSDamageType.h"
#include "QSDamageType.h"



UQSDamageType::UQSDamageType(const FObjectInitializer& ObjectInitializer)
{
	//Super::UQSDamageType(ObjectInitializer);


}
//
FString UQSDamageType::ProcessHealth()
{

	FString healthMessage = FString::Printf(TEXT("You are Receiving Damage"));//, CurrentHealth);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, healthMessage);
	return healthMessage;

  
}

FString UQSDamageType::ProcessDeath()
{

	 FString DeathMessage = FString::Printf(TEXT("You are Death!!!!!"));//, CurrentHealth);
	 return DeathMessage;
	 //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, DeathMessage);

}


/*This function will execute if no override founded on blueprint*/
FString UQSDamageType::Process_BP_CPPDeath_Implementation()
{
	//FString DeathMessage = FString::Printf(TEXT("You are Death!!!!!"));//, CurrentHealth);
	return FString::Printf(TEXT("You are Death!!!!!"));
																	   
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, DeathMessage);
}
