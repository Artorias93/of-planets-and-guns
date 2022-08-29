#pragma once

#include <Engine/EngineTypes.h>
#include "Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"

#include "GameplayEffectTypes.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "OPAG_PlayerState.h"
#include "Characters/OPAG_CharacterBase.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "OPAG_Player.generated.h"

class UOPAG_AttributeSetBase;
class UTimelineComponent;
class UAnimInstance;
class UCurveFloat;
class AOPAG_ModuleBase;
class UCameraComponent;
class USpringArmComponent;
class UChildActorComponent;
class AOPAG_GunBase;
class AOPAG_ModularGun;
class UAnimBlueprintGeneratedClass;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Player : public AOPAG_CharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	const class UOPAG_AttributeSetPlayer* AttributeSetPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* DashTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* RecoilTimeline;

	UPROPERTY(Category = "OPAG | Character | Camera", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CurrentCamera;

	UPROPERTY(Category = "OPAG | Character | Timeline", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ADSCurveFloat;

	UPROPERTY(Category = "OPAG | Character | Timeline", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RecoilCurveFloat;

	UPROPERTY(Category = "OPAG | Character | Widgets", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrosshairClass;
	
public:
	AOPAG_Player();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	class UOPAG_InventorySystemComponent* InventorySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* PrimaryGunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* SecondaryGunComponent;

public:
	UPROPERTY(Category = "OPAG | Character | View", EditAnywhere, BlueprintReadWrite)
	FVector ViewOffset = FVector(.0f, .0f, -35.f);

public:
	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimBlueprintGeneratedClass* RifleAnimClass;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimBlueprintGeneratedClass* HandgunAnimClass;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* FireMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* ReloadMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* SwapRifleMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* SwapHandgunMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* InspectGunMontageToPlay;

public:
	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite)
	AOPAG_ModularGun* PrimaryGun;

	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite)
	AOPAG_GunBase* CurrentGun;

public:
	UPROPERTY(Category = "Player", EditAnywhere, BlueprintReadWrite)
	int PotionCount = 3;

	UPROPERTY(Category = "Player", EditAnywhere, BlueprintReadWrite)
	float TossForce = 15000.f;

public:
	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	bool bIsFirstDash = true;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	float DashSpeed = 10000.f;

	UPROPERTY(Category = "Dash", EditDefaultsOnly, BlueprintReadWrite)
	float DashLenght = 500.f;

public:
	UPROPERTY(Category = "LookupInteractable", EditAnywhere, BlueprintReadWrite)
	float LookUpMaxDistance = 100.f;

	UPROPERTY(Category = "LookupInteractable", EditAnywhere, BlueprintReadWrite)
	float LookAtMaxEnemyDistance = 9000.f;

	UPROPERTY(Category = "LookupInteractable", EditAnywhere, BlueprintReadWrite)
	float LookUpCapsuleRadius = 20.f;

	UPROPERTY(Category = "LookupInteractableTypes", EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractableTypesQuery;

public: 
	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> PlayerShieldAbility;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Stamina;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Grenade;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> HealthPotion;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Dash;

	UPROPERTY(Category = "Ability System", EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> ShieldAbility;

public:
	UPROPERTY(Category = "Debug", EditInstanceOnly, BlueprintReadWrite, meta = (ToolTip = "With this variable is possible to modify the main stats of current gun."))
	FOPAG_GunStats DebugCurretGunStats;

	UPROPERTY(Category = "Debug", EditInstanceOnly, BlueprintReadWrite, meta = (ToolTip = "With this variable is possible to modify the modules of primary gun."))
	FOPAG_GunModules DebugCurretGunModules;

	UPROPERTY(Category = "Debug", EditInstanceOnly, BlueprintReadWrite, meta = (ToolTip = "With this variable is possible to modify the primary gun's type."))
	TSubclassOf<AOPAG_ModularGun> DebugCurrentPrimaryGun;

	UPROPERTY(Category = "Debug", EditInstanceOnly, BlueprintReadWrite, meta = (ToolTip = "Modify the max player's life."))
	int DebugHealthMax = 0;

	UPROPERTY(Category = "Debug", EditInstanceOnly, BlueprintReadWrite, meta = (ToolTip = "Modify the max player's dash count."))
	int DebugDashMaxCount = 0;

	UPROPERTY(Category = "Debug", EditInstanceOnly, BlueprintReadWrite, meta = (ToolTip = "Modify the restore timer player's dash."))
	float DebugDashRestoreTime = 0.f;

private:
	const FName SocketRifleName = "SOCKET_Rifle";
	const FName SocketHandgunName = "SOCKET_Handgun";
	const FName SocketHandLeft = "SOCKET_Hand_L";
	const float LookUpDistanceToPlayer = 150.f;
	bool bIsDashAlreadyInitialized;
	bool bRunning = false;
	bool bIsInADS = false;
	bool bIsPreADS = false;
	bool bHasAlreadyLooked = false;
	bool bIsShieldBroken = false;
	float GunPreADSRoll = 0;
	AActor* InteractableObject = nullptr;
	AActor* PreviousLookedAtModule = nullptr;
	AActor* Shield = nullptr;

private:
	FOnTimelineFloat OnRecoilTimelineProgress;
	FOnTimelineEventStatic OnDashTimelineFinished;

private:
	FTimerHandle RestartShieldPlayerHandle;
	FTimerHandle ShieldPlayerCooldownHandle;

protected:
	virtual void BeginPlay() override;
	virtual void Death() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ReceiveDamage(const float Damage) override;

private:
	// Events system
	void BroadcastEvents();
	void BindEvents();

	// Init standard values
	void InitDefaultValues();
	void InitDashTimeline();
	void InitRecoilTimeline();
	void InitDashValues();

	UFUNCTION() void TeleportPlayerToStart(FTransform StartLocation);

	// Mathematics utility
	const FVector CalculateThrowForce() const;
	
	// Ability behavior
	void InitializeAbilities();

	// Guns behavior 
	void AttachDefaultGuns();
	const bool IsPrimaryGunActive() const;

	//Camera behavior
	void MakingAimCameraRotate();

	// Shooting
	const bool CheckCanShoot() const;
	virtual void GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance = 0) override;

	// LookAt intractable items
	void CalculateLookAtLocations(FVector& OutStartLocation, FVector& OutDirection, float MaxDistance);
	AActor* CheckLookAtInteractableItems();
	const FVector CalculateModuleSpawnLocation() const;

	void UpdateInteractableMessage();
	void UpdateComparison();

	// Lookat Enemy distance
	void LookAtEnemy();

	// Interact with items
	void InteractWithModule(AOPAG_ModuleBase* Module);
	void PickupModule(AOPAG_ModuleBase* Module);

	// Interact with guns
	void PickupGun(AOPAG_ModularGun* Gun);

	// Timer
	void ResetShieldTimer();
	void ShieldCooldownTimer();

public:
	// Movement player input
	UFUNCTION() void MoveForward(const FInputActionValue& Value);
	UFUNCTION() void MoveRight(const FInputActionValue& Value);
	UFUNCTION() void Lookup(const FInputActionValue& Value);
	UFUNCTION() void Turn(const FInputActionValue& Value);
	UFUNCTION() void Jumping();

	// Gun input
	UFUNCTION() void Shoot();
	UFUNCTION() void StopShoot();
	UFUNCTION() void SwapGuns();
	UFUNCTION() void ReloadGun();
	UFUNCTION() void IncrementPrimaryGunAmmo();
	UFUNCTION() void InspectGun();

	// Interact with items
	UFUNCTION() void InteractWithItem();
	UFUNCTION() void UsePotion();

	// Aim input
	UFUNCTION() void SwitchToADSCamera();
	UFUNCTION() void ResetFPSCamera();

	// Active ability input
	UFUNCTION() void ActiveDash();
	UFUNCTION() void ActiveGunAbility();

public:
	UFUNCTION() void DashTimelineFinished();
	UFUNCTION() void RecoilTimelineProgress(float Value);

public:
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void SavePlayerState();
	UFUNCTION() void ApplyPlayerState(const FOPAG_PlayerState& StateToApply);

public:
	UFUNCTION(BlueprintCallable) const bool GetIsADS() const;
	UFUNCTION(BlueprintCallable) const bool GetIsRunning() const;
	UFUNCTION(BlueprintCallable) void AttachMagazineToHandNotify();
	UFUNCTION(BlueprintCallable) void AttachMagazineToGunNotify();

	void HealthChanged(const FOnAttributeChangeData& Data);

	// Getter and setter for shield element
	void SetShield(AActor* NewShield);
	AActor* GetShield() const;

public:
	void RelocateComponets();
	FVector GetViewLocation() const;

public:
	FORCEINLINE UCameraComponent* GetCurrentCamera() const { return CurrentCamera; }
};
