// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "QSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class QUCIKSTARTER_MP_CPP_API AQSPlayerState : public APlayerState
{
	GENERATED_BODY()

	
	 AQSPlayerState(const FObjectInitializer& ObjectInitializer);
	

public:

	UPROPERTY(ReplicatedUsing = OnRep_KillPoints,VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
		int32 killPoints;

	UPROPERTY(ReplicatedUsing = OnRep_Deaths, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
		int32 DeathTimes;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
		FString InfoMessage;
	/** RepNotify for changes made to kill Points.*/
	UFUNCTION()
		void OnRep_KillPoints();
	
	/** RepNotify for changes made to kill Points.*/
	UFUNCTION()
		void OnRep_Deaths();

	UFUNCTION(BlueprintCallable)
		void ResetKillPoints();


	UFUNCTION(BlueprintCallable)
	void SetKillPoints(int32 DeltaKillPoints);

	UFUNCTION(BlueprintPure)
		int32 GetKillPoints() const;

	UFUNCTION(BlueprintCallable)
		void SetDeathTimes();
	
	UFUNCTION(BlueprintCallable)
		void SetInfoMessage(FString NewMessage);


	UFUNCTION(BlueprintPure)
		int32 GetDeathTimes() const;

	UFUNCTION(BlueprintPure)
		uint8 GetPawnPing() const;

	UFUNCTION(BlueprintPure)
		FString GetInfoMessage() const;
	
	//Definida en AplayerState 
	//void SetScore(const float NewScore);

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
