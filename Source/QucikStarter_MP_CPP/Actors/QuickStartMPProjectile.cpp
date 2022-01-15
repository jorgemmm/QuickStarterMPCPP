// Fill out your copyright notice in the Description page of Project Settings.


//#include "QucikStarter_MP_CPP/Actors/QuickStartMPProjectile.h
#include "QuickStartMPProjectile.h"

#include "QucikStarter_MP_CPP/QucikStarter_MP_CPPCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

#include "DrawDebugHelpers.h"



// Sets default values
AQuickStartMPProjectile::AQuickStartMPProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;


	//Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(37.5f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;
	
	//Registering the Projectile Impact function on a Hit event.
	//make sure any new actions that the player can perform are triggered by Server functions.
	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &AQuickStartMPProjectile::OnProjectileImpact);
	}

	//Definition for the Mesh that will serve as our visual representation.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//Set the Static Mesh and its position/scale if we successfully found a mesh asset to use.
	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
		//StaticMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
		StaticMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}


	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultImpactEffect(TEXT("/Game/StarterContent/Particles/P_Sparks.P_Sparks"));
	if (DefaultImpactEffect.Succeeded())
	{
		ImpactEffect = DefaultExplosionEffect.Object;
	}

	//Definition for the Projectile Movement Component.
	//Replication pre-built
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	
	Damage = 10.0f;

}

// Called when the game starts or when spawned
void AQuickStartMPProjectile::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AQuickStartMPProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/** Esta Función está registrada para que se ejecute en el servidor*/
void AQuickStartMPProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
		return;

	if (OtherActor)
	{
		//UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
		
		
		
		FVector spawnLocation = OtherActor->GetActorLocation();	
		


		AQucikStarter_MP_CPPCharacter*  Enemy  =   Cast<AQucikStarter_MP_CPPCharacter>(OtherActor);
		
		if (Enemy)
			FinalEffect = ImpactEffect;
		else
			FinalEffect = ExplosionEffect;
		

		//FVector End		
		// draw collision sphere
		// create a collision sphere		
				

		
		float DamageRadius = 100.f;
		TArray<AActor*> IgnoreElems;
		
		

		FVector Start = GetActorLocation();
		
		FVector End = GetActorLocation();
		//FCollisionQueryParams CollisionParams;

		FHitResult outHit = Hit;
		
		
		bool isHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			Start,
			End,
			DamageRadius,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			IgnoreElems,
			EDrawDebugTrace::ForDuration,
			outHit,
			true,
			FLinearColor::Green, //Trace
			FLinearColor::Red,   //Hit
			5.0     //DrawTime

		);

	if (isHit) {

		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(DamageRadius);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, false,5.f);


		//Colocar aquí daño radial

		UGameplayStatics::ApplyRadialDamage(
			GetWorld(),
			Damage,
		    outHit.Location,
			DamageRadius,
			DamageType,
			IgnoreElems,
			this,
			GetInstigator()->Controller,
			true,
			ECC_Pawn);

	}
		
		
	}

	//Call in server
	Destroy(); 
}



/**Call in clients*/
void AQuickStartMPProjectile::Destroyed()
{
	
	FVector spawnLocation = GetActorLocation();
	
	UGameplayStatics::SpawnEmitterAtLocation(this, 
		ExplosionEffect,
		spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

