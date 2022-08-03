#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "OPAG_PlayerController.generated.h"

class UOPAG_EventManagerSubsystem;
class UEnhancedInputComponent;
class UOPAG_GameInstance;

USTRUCT(BlueprintType)
struct FInputActions
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly) UInputAction* Move;

	UPROPERTY(EditDefaultsOnly) UInputAction* CameraMove;

	UPROPERTY(EditDefaultsOnly) UInputAction* Jump;

	UPROPERTY(EditDefaultsOnly) UInputAction* Shoot;

	UPROPERTY(EditDefaultsOnly) UInputAction* SwapGun;

	UPROPERTY(EditDefaultsOnly) UInputAction* ReloadGun;

	UPROPERTY(EditDefaultsOnly) UInputAction* Interact;

	UPROPERTY(EditDefaultsOnly) UInputAction* ADS;

	UPROPERTY(EditDefaultsOnly) UInputAction* UsePotion;

	UPROPERTY(EditDefaultsOnly) UInputAction* Dash;

	UPROPERTY(EditDefaultsOnly) UInputAction* GunAbility;
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOPAG_PlayerController();

	UPROPERTY(Category = "Enhanced Input", EditDefaultsOnly, BlueprintReadWrite)
		FInputActions InputActions;

	UPROPERTY(Category = "Enhanced Input", EditDefaultsOnly, BlueprintReadWrite)
		UInputMappingContext* InputMappingContext;

private:
	UOPAG_EventManagerSubsystem* EMS;

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UFUNCTION() void MoveForward(const FInputActionValue& Value);

	UFUNCTION() void MoveRight(const FInputActionValue& Value);

	UFUNCTION() void Lookup(const FInputActionValue& Value);

	UFUNCTION() void Turn(const FInputActionValue& Value);

	UFUNCTION() void Jumping();

	UFUNCTION() void StopJumping();

	UFUNCTION() void Shoot();

	UFUNCTION() void SwapGuns();

	UFUNCTION() void ReloadGun();

	UFUNCTION() void InteractWithItem();

	UFUNCTION() void SwitchToADSCamera();

	UFUNCTION() void ResetFPSCamera();

	UFUNCTION() void UsePotion();

	UFUNCTION() void ActiveDash();

	UFUNCTION() void ActiveGunAbility();
};
