// Fill out your copyright notice in the Description page of Project Settings.


//#include "QucikStarter_MP_CPP/Actors/QuickStartMPProjectile.h
#include "QuickStartMPProjectile.h"

#include "QucikStarter_MP_CPP/QucikStarter_MP_CPPCharacter.h"
#include "QucikStarter_MP_CPP/Actors/QSDamageType.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"

#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/PointLightComponent.h"


#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

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

	ProjectileDazzler = CreateDefaultSubobject<UPointLightComponent>(TEXT("ProjectileDazzler"));
	ProjectileDazzler->SetSourceLength(10.f);

	ProjectileDazzler->SetupAttachment(RootComponent);

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
		ImpactEffect = DefaultImpactEffect.Object;
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

	bpawnImpacted = false;

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


/** Esta Funci?n est? registrada para que se ejecute en el servidor*/
void AQuickStartMPProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		Destroy();
		return;
	}
	
	
	
	if (OtherActor)
	{
		//UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
		if(GetLocalRole()==ROLE_Authority)
		{
			AQucikStarter_MP_CPPCharacter* enemy = Cast<AQucikStarter_MP_CPPCharacter>(OtherActor);
			if (enemy)
			{
				//Esto se puede ahorrar
				//Pues se puede poner como RepNotify en Health
				bpawnImpacted = true;
				//MulticastRPCHandleFX(enemy->GetActorLocation());

			}
		}
		
				
		
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
			EDrawDebugTrace::None,
			outHit,
			true,
			FLinearColor::Green, //Trace
			FLinearColor::Red,   //Hit
			.2f     //DrawTime

		);

	if (isHit) {

		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(DamageRadius);
		//DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, false,.2f);


		//Colocar aqu? da?o radial

		UGameplayStatics::ApplyRadialDamage(
			GetWorld(),
			Damage,
		    outHit.Location,
			DamageRadius,
			UQSDamageType::StaticClass(),
			IgnoreElems,
			this,
			GetInstigator()->Controller,
			true,
			ECC_Pawn);



	}
		
		
	}

	// Pon un timer para no solapar las 2 particles
	//Call from server Clients
	Destroy(); 
}



void AQuickStartMPProjectile::MulticastRPCHandleFX_Implementation(FVector Location)
{
	//if (GetRemoteRole() < ROLE_Authority) //Redundante pues solo se ejecuta en clientes
	//{
	//}
		UGameplayStatics::SpawnEmitterAtLocation(this,
			ImpactEffect,
			Location, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

		FString ImpactMessage = FString::Printf(TEXT("You have a hit!!."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ImpactMessage);

	
	

}

/**Call in clients
Esta funci?n se replica a todos los clientes conectados Se replica 
*/
void AQuickStartMPProjectile::Destroyed()
{

	FVector spawnLocation = GetActorLocation();
	//FVector spawnScale  =
		
		if (!bpawnImpacted)
		{
			

			/*UGameplayStatics::SpawnEmitterAtLocation(this,
				ExplosionEffect,
				spawnLocation,
				FRotator::ZeroRotator,
				FVector(0.2,0.2,0.2),
				
				true, EPSCPoolMethod::AutoRelease);*/

			FString ImpactMessage = FString::Printf(TEXT("You hit to enemy!!!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ImpactMessage);
		}
		
		FString ImpactMessage = FString::Printf(TEXT("You don?t have a hit!!."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ImpactMessage);
		bpawnImpacted = false;
}



void AQuickStartMPProjectile::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health and currentWeapon.
	DOREPLIFETIME(AQuickStartMPProjectile, bpawnImpacted);
	

}

