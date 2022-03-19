// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QS_MP_Weapon.generated.h"

class USkeletalMeshComponent;
class UArrowComponent;

class AQuickStartMPProjectile;

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


	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }


	void LineTraceFire();

	/** Delay between shots in seconds. Used to control fire rate for our test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** If true, we are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
		TSubclassOf<AQuickStartMPProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	  FName MuzzleSocketName = TEXT("Muzzle");


	


	

	void StartFire();

	void StopFire();

	/** Server function for spawning projectiles.
	Deprecated: code in Weapon
	*/

	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;




};
