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

	PopulateArrayWithTable();

	// All binding events for player's ability
	EIC->BindAction(InputActions[7]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::SwapGuns);
	EIC->BindAction(InputActions[8]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::ReloadGun);
	EIC->BindAction(InputActions[9]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::ActiveDash);
	EIC->BindAction(InputActions[10]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::ActiveGunAbility);

	// All binding events for player's movements
	EIC->BindAction(InputActions[0]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::MoveForward);
	EIC->BindAction(InputActions[0]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::MoveRight);
	EIC->BindAction(InputActions[1]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Lookup);
	EIC->BindAction(InputActions[1]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Turn);
	EIC->BindAction(InputActions[2]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Jumping);
	EIC->BindAction(InputActions[2]->InputAction, ETriggerEvent::Canceled, this, &AOPAG_PlayerController::StopJumping);

	// All binding events for shoot
	EIC->BindAction(InputActions[3]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::Shoot);
	EIC->BindAction(InputActions[3]->InputAction, ETriggerEvent::Triggered, this, &AOPAG_PlayerController::Shoot);
	EIC->BindAction(InputActions[3]->InputAction, ETriggerEvent::Completed, this, &AOPAG_PlayerController::StopShoot);

	// Binding event for interact with objects
	EIC->BindAction(InputActions[4]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::InteractWithItem);

	// Binding event for interact with camera ads
	EIC->BindAction(InputActions[5]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::SwitchToADSCamera);
	EIC->BindAction(InputActions[5]->InputAction, ETriggerEvent::Completed, this, &AOPAG_PlayerController::ResetFPSCamera);

	// binding event for health 
	EIC->BindAction(InputActions[6]->InputAction, ETriggerEvent::Started, this, &AOPAG_PlayerController::UsePotion);
}

void AOPAG_PlayerController::PopulateArrayWithTable()
{
	const FString Context;
	if (InputDataTable)
	{
		InputDataTable->GetAllRows(Context, InputActions);
	}
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

void AOPAG_PlayerController::StopShoot()
{
	EMS->StopShoot.Broadcast();
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