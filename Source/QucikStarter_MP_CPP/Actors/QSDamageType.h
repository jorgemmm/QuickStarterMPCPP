// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "QucikStarter_MP_CPP/Interfaces/DamageInterface.h"
#include "QSDamageType.generated.h"


//class IDamageInterface;

/**
 * 
 */
UCLASS()
class QUCIKSTARTER_MP_CPP_API UQSDamageType: public UDamageType,public IDamageInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	UQSDamageType(const FObjectInitializer& ObjectInitializer);


	FString ProcessHealth() override;

	FString ProcessDeath() override;

	FString Process_BP_CPPDeath_Implementation() override; /*Override on blueprint*/
				/*this code will call if no Blueprint override is found.*/

		
};
