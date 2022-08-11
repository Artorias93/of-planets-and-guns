#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include <Engine/DataTable.h>
#include "OPAG_PlayerController.generated.h"

class UOPAG_EventManagerSubsystem;
class UEnhancedInputComponent;
class UOPAG_GameInstance;
class UDataTable;

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_InputAction : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		UInputAction* InputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int InputActionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DisplayName;
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOPAG_PlayerController();

	UPROPERTY(Category = "Enhanced Input", EditDefaultsOnly, BlueprintReadWrite)
		UDataTable* InputDataTable;

	UPROPERTY(Category = "Enhanced Input", EditDefaultsOnly, BlueprintReadWrite)
		UInputMappingContext* InputMappingContext;

private:
	UOPAG_EventManagerSubsystem* EMS;
	TArray<FOPAG_InputAction*> InputActions;

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void PopulateArrayWithTable();


public:
	UFUNCTION() void MoveForward(const FInputActionValue& Value);

	UFUNCTION() void MoveRight(const FInputActionValue& Value);

	UFUNCTION() void Lookup(const FInputActionValue& Value);

	UFUNCTION() void Turn(const FInputActionValue& Value);

	UFUNCTION() void Jumping();

	UFUNCTION() void StopJumping();

	UFUNCTION() void Shoot();

	UFUNCTION() void StopShoot();

	UFUNCTION() void SwapGuns();

	UFUNCTION() void ReloadGun();

	UFUNCTION() void InteractWithItem();

	UFUNCTION() void SwitchToADSCamera();

	UFUNCTION() void ResetFPSCamera();

	UFUNCTION() void UsePotion();

	UFUNCTION() void ActiveDash();

	UFUNCTION() void ActiveGunAbility();
};
