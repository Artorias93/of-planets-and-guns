#include "Characters/Player/OPAG_PlayerController.h"

#include "Managers/OPAG_EventManagerSubsystem.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/OPAG_Player.h"
#include <EnhancedInputComponent.h>
#include "Managers/OPAG_GameInstance.h"

AOPAG_PlayerController::AOPAG_PlayerController()
{
	
}

void AOPAG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
}

void AOPAG_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	check(EIC);

	// All binding events for player's ability
	EIC->BindAction(InputActions.SwapGun, ETriggerEvent::Started, this, &AOPAG_PlayerController::SwapGuns);
	EIC->BindAction(InputActions.ReloadGun, ETriggerEvent::Started, this, &AOPAG_PlayerController::ReloadGun);
	EIC->BindAction(InputActions.Dash, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::ActiveDash);
	EIC->BindAction(InputActions.GunAbility, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::ActiveGunAbility);

	// All binding events for player's movements
	EIC->BindAction(InputActions.Move, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::MoveForward);
	EIC->BindAction(InputActions.Move, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::MoveRight);
	EIC->BindAction(InputActions.CameraMove, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Lookup);
	EIC->BindAction(InputActions.CameraMove, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Turn);
	EIC->BindAction(InputActions.Jump, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Jumping);
	EIC->BindAction(InputActions.Jump, ETriggerEvent::Canceled, this, &AOPAG_PlayerController::StopJumping);

	// All binding events for shoot
	EIC->BindAction(InputActions.Shoot, ETriggerEvent::Started, this, &AOPAG_PlayerController::Shoot);
	EIC->BindAction(InputActions.Shoot, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Shoot);

	// Binding event for interact with objects
	EIC->BindAction(InputActions.Interact, ETriggerEvent::Started, this, &AOPAG_PlayerController::InteractWithItem);

	// Binding event for interact with camera ads
	EIC->BindAction(InputActions.ADS, ETriggerEvent::Started, this, &AOPAG_PlayerController::SwitchToADSCamera);
	EIC->BindAction(InputActions.ADS, ETriggerEvent::Completed, this, &AOPAG_PlayerController::ResetFPSCamera);

	// binding event for health 
	EIC->BindAction(InputActions.UsePotion, ETriggerEvent::Started, this, &AOPAG_PlayerController::UsePotion);
}

void AOPAG_PlayerController::MoveForward(const FInputActionValue& Value)
{
	EMS->MoveForward.Broadcast(Value);
}

void AOPAG_PlayerController::MoveRight(const FInputActionValue& Value)
{
	EMS->MoveRight.Broadcast(Value);
}

void AOPAG_PlayerController::Lookup(const FInputActionValue& Value)
{
	EMS->Lookup.Broadcast(Value);
}

void AOPAG_PlayerController::Turn(const FInputActionValue& Value)
{
	EMS->Turn.Broadcast(Value);
}

void AOPAG_PlayerController::Jumping()
{
	EMS->Jumping.Broadcast();
}

void AOPAG_PlayerController::StopJumping()
{
	EMS->StopJumping.Broadcast();
}

void AOPAG_PlayerController::Shoot()
{
	EMS->Shoot.Broadcast();
}

void AOPAG_PlayerController::SwapGuns()
{
	EMS->SwapGuns.Broadcast();
}

void AOPAG_PlayerController::ReloadGun()
{
	EMS->ReloadGun.Broadcast();
}

void AOPAG_PlayerController::InteractWithItem()
{
	EMS->InteractWithItem.Broadcast();
}

void AOPAG_PlayerController::SwitchToADSCamera()
{
	EMS->SwitchToADSCamera.Broadcast();
}

void AOPAG_PlayerController::ResetFPSCamera()
{
	EMS->ResetFPSCamera.Broadcast();
}

void AOPAG_PlayerController::UsePotion()
{
	EMS->UsePotion.Broadcast();
}

void AOPAG_PlayerController::ActiveDash()
{
	EMS->ActiveDash.Broadcast();
}

void AOPAG_PlayerController::ActiveGunAbility()
{
	EMS->ActiveGunAbility.Broadcast();
}