// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QS_MP_Weapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class QUCIKSTARTER_MP_CPP_API AQS_MP_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQS_MP_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* WeaponMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();


	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

};
