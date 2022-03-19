// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUCIKSTARTER_MP_CPP_API IDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**A version of React To Trigger that can be implemented in Blueprint only. Use node Aply Damge y event damage BP */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Damage Reaction")
	void Process_BPHealth();
	
	/**A version of React To Trigger that can be implemented in C++ or Blueprint. Use node Aply Damge y event damage BP */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Reaction")
		FString Process_BP_CPPDeath();


	//Only C++
	UFUNCTION()
	virtual FString ProcessHealth()=0;
	
	//Only C++
	UFUNCTION()
	virtual FString ProcessDeath()=0;

};
