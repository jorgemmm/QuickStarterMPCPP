// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseOnlineCharacter.h"

//Project Components
#include "QucikStarter_MP_CPP/Actors/QuickStartMPProjectile.h"
#include "QucikStarter_MP_CPP/Actors/QS_MP_Weapon.h"
#include "QucikStarter_MP_CPP/PlayerController/PlayerControllerBase.h"
#include "QucikStarter_MP_CPP/PlayerState/QSPlayerState.h"
#include "QucikStarter_MP_CPP/QucikStarter_MP_CPPGameMode.h"
#include "QucikStarter_MP_CPP/Interfaces/DamageInterface.h"


#include "QucikStarter_MP_CPP/Actors/QSDamageType.h"

//Engine Components and frameworks
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/DamageType.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

//Utils
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"


#include "Engine/EngineTypes.h"

#include "TimerManager.h"

// Sets default values
ABaseOnlineCharacter::ABaseOnlineCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/WeaponEffects/BloodImpact/P_blood_splash_02.P_blood_splash_02"));
	if (DefaultExplosionEffect.Succeeded())
	{
		DamageEffect = DefaultExplosionEffect.Object;
	}


	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;



	//Initialize Weapon class
	WeaponClass = AQS_MP_Weapon::StaticClass();

	//DamageType = UQSDamageType::StaticClass();
}

// Called when the game starts or when spawned
void ABaseOnlineCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;

		if (WeaponClass) {
			CurrentWeaponRef = GetWorld()->SpawnActor<AQS_MP_Weapon>(WeaponClass, GetActorLocation(), GetActorRotation(), spawnParameters);

		}
		//AQS_MP_Weapon* CurrentWeaponRef = GetWorld()->SpawnActor<AQS_MP_Weapon>(GetActorLocation(), GetActorRotation(), spawnParameters);


		CurrentWeaponRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));

		FString healthMessage = FString::Printf(TEXT("%s now has %s component attched."), *GetFName().ToString(), *CurrentWeaponRef->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, healthMessage);

		//OnTakeAnyDamage.AddDynamic(this, &ABaseOnlineCharacter::AfterTakeDamage);
	}
	
}

// Called every frame
void ABaseOnlineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseOnlineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Handle firing projectiles
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseOnlineCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseOnlineCharacter::StartFire);

}


/////////////////////////////////////////////////////////
//Health and Damage


void ABaseOnlineCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate(); //En servers
		if (OnCalculeDead()) {
			//En servers
			CurrentWeaponRef->Destroy();
			
		}
	}
}


//Este evento se produce en el servidor
float ABaseOnlineCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const &DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	bool bIsImplemented = DamageEvent.DamageTypeClass->GetClass()->ImplementsInterface(UDamageInterface::StaticClass());


	FDamageEvent damageEvent = DamageEvent;

	//bool bIsImplemented = DamageEvent.DamageTypeClass->Implements<UDamageInterface>();

	/*if(bIsImplemented)
	{
		UE_LOG(LogTemp, Warning, TEXT("El objeto damageType implementa la insterfaz de daño personalizada!!!!!!!!!!!!!"));
	}
	else
	{
		bIsImplemented = UDamageType::StaticClass()->Implements<UDamageInterface>();

		if (bIsImplemented)
		{
			UE_LOG(LogTemp, Warning, TEXT("El objeto damageType implementa la insterfaz de daño personalizada!!!!!!!!!!!!!"));
		}
	}*/
	
	

   /* Super:TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);
	
	
	TSubclassOf<UDamageType> DamageType;
	UQSDamageType*  MyDamage = GetWorld()->SpawnActor<class UQSDamageType>(  DamageType, spawnParameters); //Bad 
	
	*/
	/*UQSDamageType* MyDamage;
	if (DamageEvent.DamageTypeClass->GetDefaultObject()) {
		UE_LOG(LogTemp, Error, TEXT("DamageEvent.DamageTypeClass is not Null"));
		MyDamage = Cast<class UQSDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
	}
	else
	{

		TSubclassOf<class UDamageType> DamageTypeclass;
		MyDamage = Cast<class UQSDamageType>(DamageTypeclass->GetDefaultObject());
	}*/
	
	//TSubclassOf< UDamageType> DamageType = DamageEvent.DamageTypeClass;
	
	

	//UQSDamageType* MyDamage= Cast<UQSDamageType>(DamageEvent.DamageTypeClass); //No coje El puntero
	UQSDamageType* MyDamage = Cast<UQSDamageType>(damageEvent.DamageTypeClass->GetDefaultObject());
	
	
	if(MyDamage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Se inició el puntero de al damageType personalizado!!!!!!!!!!!!!"));
		UE_LOG(LogTemp, Warning, TEXT("Se inició el puntero de al damageType personalizado!!!!!!!!!!!!!"));
		

		//FString HealthMessage = MyDamage->ProcessHealth();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, MyDamage->ProcessHealth());
		
		//const IDamageInterface* DamageInterface = Cast<IDamageInterface>(MyDamage);		
		
		//FString healthMessage = MyDamage->Process_BP_CPPDeath();  //ProcessHealth();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, *MyDamage->ProcessHealth());
		

		if (CurrentHealth <= 0)
		{
			return 0;
		}

		float damageApplied = CurrentHealth - DamageTaken;


		SetCurrentHealth(damageApplied);
		if (CurrentHealth <= 0)
		{
			MyDamage->ProcessDeath();
			CurrentWeaponRef->Destroy();

			APlayerControllerBase* PCInstigator = Cast<APlayerControllerBase>(EventInstigator);
			if(PCInstigator)
			{
			    //
				AQSPlayerState* QSPlayerState = Cast<AQSPlayerState>(PCInstigator->PlayerState);
				if (QSPlayerState)
				{
					UE_LOG(LogTemp, Warning, TEXT("Kill points  is Counted!!!!"));
					QSPlayerState->SetKillPoints(1);
					QSPlayerState->SetInfoMessage(TEXT("Kill points  is Counted!!!!"));
				}
					

			}
			
			{
				
				AQSPlayerState* QSPlayerState = Cast<AQSPlayerState>(GetPlayerState());
				if (QSPlayerState)
					QSPlayerState->SetDeathTimes();


			}

			Destroy();

		}
		return damageApplied;
	
	}	
	else
	{

		UE_LOG(LogTemp, Error, TEXT("No se inició el puntero de al damageType personalizado"));
		return 0;

	}
	
	//Otro Tipo de daño Por ejemplo fuego
	//UQSDamageType_2* MyDamage_2 = Cast<UQSDamageType_2>(DamageEvent.DamageTypeClass->GetDefaultObject());
	/*if (MyDamage_2)
	{
	//To do My damage_2
	  
	 //return damageApplied;
	}*/
	
	
	
	
	
}






//////////////////////////////////////////////////////////////////////////
// Replicated Properties


//Este Evento se replica en cada cliente
//void ABaseOnlineCharacter::Destroyed()
//{
	//if (GetLocalRole() == ROLE_Authority)//Server- Specific Funcionality
 	//if (GetLocalRole() < ROLE_Authority)// Simulated Proxies Client-specific functionality	
	//Client-specific functionality Conected 
	//if (IsLocallyControlled()) 
	//{
	//	
	//	APlayerControllerBase* PC = Cast<APlayerControllerBase>(GetController());
	//	if (!PC)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("No player controller loaded in BaseCharacter"));
	//		return;
	//	}
	//	UE_LOG(LogTemp, Error, TEXT("A player controller loaded in BaseCharacter!!!!!!!!!!!!!!"));
	//	FTimerHandle PlayerSpawnHandle;
	//	float DelayToSpawn = 3.0;
	//	GetWorldTimerManager().ClearTimer(PlayerSpawnHandle);
	//	GetWorldTimerManager().SetTimer(
	//		PlayerSpawnHandle,
	//		PC,//this,
	//		&APlayerControllerBase::ResPawn,//&ABaseOnlineCharacter::Respawn,
	//		DelayToSpawn,
	//		false//,0.f
	//		
	//	);
	//}
//}

//Deprecated 
//void ABaseOnlineCharacter::Respawn()
//{
//	//if (GetLocalRole() == ROLE_Authority)//Server only - Specific Funcionality
//	//if (GetLocalRole() < ROLE_Authority)//Simulated Proxies  specific functionality	( Bullets, Pickups, IA´s)
//	if (IsLocallyControlled()) //Autonomous Proxies only Client-specific functionality Conected 
//	{
//		/*APlayerControllerBase* PC = Cast<APlayerControllerBase>(GetController());
//
//		if (!PC)
//		{
//			UE_LOG(LogTemp, Error, TEXT("No player controller loaded in BaseCharacter"));
//			return;
//		}
//
//		PC->ResPawn();*/
//	}
//	
//	
//	
//	
//}

//Se replica en los clientes (Mejor usar RepNotyfy)
void ABaseOnlineCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();  //En  todos los Clients
}


void ABaseOnlineCharacter::OnHealthUpdate()
{
	//Client-specific functionality

	if (IsLocallyControlled())
	{
		/*APlayerControllerBase* PC = Cast<APlayerControllerBase>(GetController());
		if (!PC) UE_LOG(LogTemp, Error, TEXT("No player controller loaded in BaseCharacter"));
		*/	
	

		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, healthMessage);		
		
			//if(PC)PC->ShowInfoInWBP(healthMessage);
			AQSPlayerState* QSPlayerState = Cast<AQSPlayerState>(GetPlayerState());
			if(!QSPlayerState)
				UE_LOG(LogTemp, Error, TEXT("No QSPlayerState loaded in BaseCharacter"));
			if(QSPlayerState)
				QSPlayerState->SetInfoMessage(healthMessage);
		
			
		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, deathMessage);
			
				
			//Coloc info widget			
				/*if(PC) 
					PC->ShowInfoInWBP(deathMessage);*/	


		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, healthMessage);

		AQSPlayerState* QSPlayerState = Cast<AQSPlayerState>(GetPlayerState());
		if (!QSPlayerState)
			UE_LOG(LogTemp, Error, TEXT("No QSPlayerState loaded in BaseCharacter"));
		if (QSPlayerState)
			QSPlayerState->SetInfoMessage(healthMessage);


	}

	
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this,
		DamageEffect,
		spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}


//Se replica en los clientes (Mejor usar RepNotyfy)
void ABaseOnlineCharacter::OnRep_bDead()
{
	//To Do  when character is dead
	OnCalculeDead();



}


bool ABaseOnlineCharacter::OnCalculeDead()
{
	bDead = false;
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		if (CurrentHealth <= 0)
		{
			FVector spawnLocation = GetActorLocation();

			UGameplayStatics::SpawnEmitterAtLocation(this,
				DamageEffect,
				spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

			bDead = true;
			//Destroy();    //Esta funcionalidad se replica La dejamos al fimal en take damage 
			                //No ponerla 2 veces para no llamr 2 veces al evento Destroyed



			

		}
	}
	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString DeadMessage;
		if (bDead)
		{
			DeadMessage = FString::Printf(TEXT("Character is Dead: %b"), *GetFName().ToString(), bDead);

			//Respawn();
		}
		else
			DeadMessage= FString::Printf(TEXT("Character is Alive: %b"), *GetFName().ToString(), bDead);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, DeadMessage);
	}

	//Functions that occur on all machines. 
	/*
		Any special functionality that should occur as a result of damage or death should be placed here.
	*/
	
	

	return bDead;
	/*GetWorldTimerManager().SetTimer(DestroyTimerHandle, this,
		Destroy, 2.0f, true, 5.0f);*/
}



void ABaseOnlineCharacter::OnHealthDamageFx()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this,
		DamageEffect,
		spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}


void ABaseOnlineCharacter::StartFire()
{
	if (CurrentWeaponRef)
	{
		CurrentWeaponRef->StartFire();

	}

	return;


	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;

		UWorld* World = GetWorld();
		//World->GetTimerManager().SetTimer(FiringTimer, this, &AQucikStarter_MP_CPPCharacter::StopFire, FireRate, false);
		//HandleFire();
	}
}

void ABaseOnlineCharacter::StopFire()
{
	if (CurrentWeaponRef)
	{
		CurrentWeaponRef->StopFire();

	}

	return;

	//Deprecated 
	bIsFiringWeapon = false;
}



void ABaseOnlineCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health and currentWeapon.
	DOREPLIFETIME(ABaseOnlineCharacter, CurrentHealth);
	DOREPLIFETIME(ABaseOnlineCharacter, bDead);
	DOREPLIFETIME(ABaseOnlineCharacter, CurrentWeaponRef);

}


