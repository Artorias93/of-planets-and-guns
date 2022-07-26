#pragma once

#include <Engine/EngineTypes.h>
#include "Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"

#include "GameplayEffectTypes.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/OPAG_CharacterBase.h"
#include "OPAG_Player.generated.h"


class AOPAG_AccessoryBase;
class UOPAG_AttributeSetBase;
class UKismetMathLibrary;
class UTimelineComponent;
class UCurveFloat;
class AOPAG_ModuleBase;
class USpringArmComponent;
class UCameraComponent;
class UChildActorComponent;
class UEnhancedInputComponent;
class UInputAction;
class AOPAG_GunBase;
class UDataTable;
class UKismetSystemLibrary;
class UAbilitySystemComponent;

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

	UPROPERTY(EditDefaultsOnly) UInputAction* Health;

	UPROPERTY(EditDefaultsOnly) UInputAction* Dash;

	UPROPERTY(EditDefaultsOnly) UInputAction* GunAbility;
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Player : public AOPAG_CharacterBase
{
	GENERATED_BODY()
	
public:
	AOPAG_Player();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* PrimaryGunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* SecondaryGunComponent;

public:
	UPROPERTY(Category = "Enhanced Input", EditDefaultsOnly, BlueprintReadWrite)
	FInputActions InputActions;

	UPROPERTY(Category = "Enhanced Input", EditDefaultsOnly, BlueprintReadWrite)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(Category = "Guns", VisibleAnywhere, BlueprintReadWrite)
	AOPAG_GunBase* PrimarytGun;

	UPROPERTY(Category = "Guns", VisibleAnywhere, BlueprintReadWrite)
	AOPAG_GunBase* CurrentGun;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* CurrentCamera;

	UPROPERTY(Category = "Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* FireMontageToPlay;

	UPROPERTY(Category = "Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* ReloadMontageToPlay;

	UPROPERTY(Category = "Timeline", VisibleDefaultsOnly, BlueprintReadWrite)
	UTimelineComponent* ADSTimeline;

	UPROPERTY(Category = "Timeline", VisibleDefaultsOnly, BlueprintReadWrite)
	UTimelineComponent* DashTimeline;

	UPROPERTY(Category = "Timeline", VisibleDefaultsOnly, BlueprintReadWrite)
	UTimelineComponent* RecoilTimeline;

	UPROPERTY(Category = "Timeline", EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ADSCurveFloat;

	UPROPERTY(Category = "Timeline", EditAnywhere, BlueprintReadWrite)
	UCurveFloat* RecoilCurveFloat;

	UPROPERTY(Category = "Widgets", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CrosshairClass;

public:
	UPROPERTY(Category = "Player", EditAnywhere, BlueprintReadWrite)
	int PotionCount = 3;

	UPROPERTY(Category = "Player", EditAnywhere, BlueprintReadWrite)
	float TossForce = 15000.f;

public:
	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	bool bIsFirstDash = true;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	int DashCount = 5;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	int DashMaxCount = 5;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	float DashSpeed = 10000.f;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	float DashLenght = 500.f;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	float DashRestoreTime = 5.f;

public:
	UPROPERTY(Category = "LookupInteractable", EditAnywhere, BlueprintReadWrite)
	float LookUpMaxDistance = 100.f;

	UPROPERTY(Category = "LookupInteractable", EditAnywhere, BlueprintReadWrite)
	float LookUpCapsuleRadius = 20.f;

	UPROPERTY(Category = "LookupInteractableTypes", EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractableTypesQuery;

public: 
	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Stamina;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Grenade;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> HealthPotion;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Dash;

public:
	UPROPERTY(Category = "Debug", EditDefaultsOnly, BlueprintReadWrite)
	int CurrentHealth = 0;

	UPROPERTY(Category = "Debug", EditDefaultsOnly, BlueprintReadWrite)
	int CurrentHealthMax = 0;

private:
	const FName GripSocketName = "GripPoint";
	const float LookUpDistanceToPlayer = 150.f;
	bool bIsInADS = false;
	bool bIsPreADS = false;
	float GunPreADSRoll = 0;
	AActor* InteractableObject = nullptr; // TODO: change with InteractableInterface

private:
	FOnTimelineFloat OnADSTimelineProgress;
	FOnTimelineFloat OnRecoilTimelineProgress;
	FOnTimelineEventStatic OnDashTimelineFinished;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// Init standard values
	void InitDefaultValues();
	void InitADSTimeline();
	void InitDashTimeline();
	void InitRecoilTimeline();

	// Mathematics utility
	const FVector CalculateThrowForce() const;
	
	// Ability behavior
	void InitializeAbilities();

	// Guns behavior 
	void AttachDefaultGuns();
	void SwapGuns();
	void ReloadGun();

	//Camera behavior
	void MakingAimCameraRotate();

	// Shooting
	const bool CheckCanShoot() const;
	virtual void GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance = 0) override;

	// LookAt intractable items
	void CalculateLookAtLocations(FVector& OutStartLocation, FVector& OutDirection); 
	AActor* CheckLookAtInteractableItems();
	const FVector CalculateModuleSpawnLocation() const;
	void ThrowScrap(UPrimitiveComponent* Target);

	// Interact with items
	void InteractWithModule(AOPAG_ModuleBase* Module);
	void PickupModule(AOPAG_ModuleBase* Module);

public:
	UFUNCTION() void MoveForward(const FInputActionValue& Value);

	UFUNCTION() void MoveRight(const FInputActionValue& Value);

	UFUNCTION() void Lookup(const FInputActionValue& Value);

	UFUNCTION() void Turn(const FInputActionValue& Value);

	UFUNCTION() void Jumping(const FInputActionValue& Value);

	UFUNCTION() void Shoot(const FInputActionValue& Value);

	UFUNCTION() void InteractWithItem(const FInputActionValue& Value);

	UFUNCTION() void SwitchToADSCamera(const FInputActionValue& Value);

	UFUNCTION() void ResetFPSCamera(const FInputActionValue& Value);

	UFUNCTION() void Health(const FInputActionValue& Value);

	UFUNCTION() void ActiveDash(const FInputActionValue& Value);

	UFUNCTION() void ActiveGunAbility(const FInputActionValue& Value);

public:
	UFUNCTION() void ADSTimelineProgress(float Value);

	UFUNCTION() void DashTimelineFinished();

	UFUNCTION() void RecoilTimelineProgress(float Value);

public:
	UFUNCTION(BlueprintCallable) const bool GetIsADS() const;

	void HealthChanged(const FOnAttributeChangeData& Data);
};
