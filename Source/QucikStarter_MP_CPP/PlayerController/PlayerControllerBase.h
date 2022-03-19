// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class QUCIKSTARTER_MP_CPP_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()


	//APlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Crear un userwidget personalizadfo con unca prpiedad text enlazada
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		TSubclassOf<UUserWidget> DefaultInfoLevelWBP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		UUserWidget* LevelInfoWBP;
		//OwnWidget* LevelInfoWBP;

	    


public:

	
	void SetPlayerEnabledState(bool SetPlayerEnabled);	
	

	void ResPawn();


	UFUNCTION(Server,Unreliable)//, WithValidation)
	void RequestRespawn(int value);


	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;


	int32 i =2;


	UFUNCTION(BlueprintPure, Category = "Health")
	bool GetPawnIsAlive();

	UFUNCTION()
		void ShowInfoInWBP(FString NewText);

	FString InfoText;
};


