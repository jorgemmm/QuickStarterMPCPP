// Copyright Epic Games, Inc. All Rights Reserved.

#include "QucikStarter_MP_CPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


//#include "QuickStartMPProjectile.h"

//////////////////////////////////////////////////////////////////////////
// AQucikStarter_MP_CPPCharacter

AQucikStarter_MP_CPPCharacter::AQucikStarter_MP_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	
	
		
	//Initialize projectile class
	//ProjectileClass = AQuickStartMPProjectile::StaticClass();	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AQucikStarter_MP_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AQucikStarter_MP_CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQucikStarter_MP_CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AQucikStarter_MP_CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AQucikStarter_MP_CPPCharacter::LookUpAtRate);

	// Handle firing projectiles Coded in BaseOnline
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AQucikStarter_MP_CPPCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AQucikStarter_MP_CPPCharacter::StartFire);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AQucikStarter_MP_CPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AQucikStarter_MP_CPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AQucikStarter_MP_CPPCharacter::OnResetVR);
}


void AQucikStarter_MP_CPPCharacter::BeginPlay()
{
	Super::BeginPlay();
		
		/*if (GetLocalRole() == ROLE_Authority)
		{
	
			//FActorSpawnParameters spawnParameters;
			//spawnParameters.Instigator = GetInstigator();
			//spawnParameters.Owner = this;

			//if (WeaponClass) {
			//	CurrentWeaponRef = GetWorld()->SpawnActor<AQS_MP_Weapon>(WeaponClass, GetActorLocation(), GetActorRotation(), spawnParameters);
			//	
			//}
			////AQS_MP_Weapon* CurrentWeaponRef = GetWorld()->SpawnActor<AQS_MP_Weapon>(GetActorLocation(), GetActorRotation(), spawnParameters);


			//CurrentWeaponRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));

			//FString healthMessage = FString::Printf(TEXT("%s now has %s component attched."), *GetFName().ToString(), *CurrentWeaponRef->GetName());
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, healthMessage);

		


		}*/

}

void AQucikStarter_MP_CPPCharacter::OnResetVR()
{
	// If QucikStarter_MP_CPP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in QucikStarter_MP_CPP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AQucikStarter_MP_CPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AQucikStarter_MP_CPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AQucikStarter_MP_CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AQucikStarter_MP_CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AQucikStarter_MP_CPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AQucikStarter_MP_CPPCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AQucikStarter_MP_CPPCharacter::StartFire()
{
	Super::StartFire();
}

void AQucikStarter_MP_CPPCharacter::StopFire()
{
	Super::StopFire();
}



//Old fire implementation
/**

void AQucikStarter_MP_CPPCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetControlRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AQuickStartMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AQuickStartMPProjectile>(spawnLocation, spawnRotation, spawnParameters);
}

*/