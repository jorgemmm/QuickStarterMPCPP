// Fill out your copyright notice in the Description page of Project Settings.


//#include "Actors/QS_MP_Weapon.h"
#include "QS_MP_Weapon.h"

#include "Components/SkeletalMeshComponent.h"

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

void AQS_MP_Weapon::Fire()
{
	AActor* MyOwnwer = GetOwner();
	if (MyOwnwer)
	{
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOwnwer->GetActorEyesViewPoint(EyeLocation, EyeRotator);

	}
}

