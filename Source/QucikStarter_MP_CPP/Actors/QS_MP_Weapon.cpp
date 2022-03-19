// Fill out your copyright notice in the Description page of Project Settings.


//#include "Actors/QS_MP_Weapon.h"
#include "QS_MP_Weapon.h"

#include "QuickStartMPProjectile.h"

#include "QucikStarter_MP_CPP/QucikStarter_MP_CPPCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "TimerManager.h"


// Sets default values
AQS_MP_Weapon::AQS_MP_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	
	//SetReplicates(true);
	//SetReplicatedMovement(true);
	
	//Initialize projectile class 
	ProjectileClass = AQuickStartMPProjectile::StaticClass();
	
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;
	
}

// Called when the game starts or when spawned
void AQS_MP_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQS_MP_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQS_MP_Weapon::LineTraceFire()
{

	AActor* MyOwnwer = GetOwner();
	if (MyOwnwer)
	{
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOwnwer->GetActorEyesViewPoint(EyeLocation, EyeRotator);

		FVector ShotDirection = EyeRotator.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 1000);

	}
}

void AQS_MP_Weapon::StartFire()
{
	if (!bIsFiringWeapon)
	{

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AQS_MP_Weapon::StopFire, FireRate, false);
		HandleFire();
	}
}

void AQS_MP_Weapon::StopFire()
{
	bIsFiringWeapon = false;
}

void AQS_MP_Weapon::HandleFire_Implementation()
{
	
	AActor* MyOwnwer = GetOwner();

	
	if (MyOwnwer && ProjectileClass)
	 {
		//AQucikStarter_MP_CPPCharacter* PawnOwner = Cast<AQucikStarter_MP_CPPCharacter>(MyOwnwer);
		
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOwnwer->GetActorEyesViewPoint(EyeLocation, EyeRotator);
		
			
		FVector MuzzleLocation = GetWeaponMesh()->GetSocketLocation(MuzzleSocketName);		
		FRotator MuzzleRotation = GetWeaponMesh()->GetSocketRotation(MuzzleSocketName);
		
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //TL
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;
		
			AQuickStartMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AQuickStartMPProjectile>(
				ProjectileClass, MuzzleLocation,EyeRotator,
				spawnParameters);

	}
	


}

