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
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* PrimaryGunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* SecondaryGunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UOPAG_InventorySystemComponent* InventorySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	const class UOPAG_AttributeSetPlayer* AttributeSetPlayer;
	
public:
	AOPAG_Player();

private:
	UPROPERTY(Category = "OPAG | Character | Physics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TossForce = 15000.f;

private:
	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimBlueprintGeneratedClass* RifleAnimClass;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimBlueprintGeneratedClass* HandgunAnimClass;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwapRifleMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwapHandgunMontageToPlay;

	UPROPERTY(Category = "OPAG | Character | Animation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* InspectGunMontageToPlay;

private:
	UPROPERTY(Category = "OPAG | Character | Potion", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int PotionCount = 3;

private:
	UPROPERTY(Category = "OPAG | Character | Dash", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsFirstDash = true;

	UPROPERTY(Category = "OPAG | Character | Dash", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DashSpeed = 10000.f;

	UPROPERTY(Category = "OPAG | Character | Dash", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DashLenght = 500.f;

private:
	UPROPERTY(Category = "OPAG | Character | LookupInteractable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LookUpMaxDistance = 100.f;

	UPROPERTY(Category = "OPAG | Character | LookupInteractable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LookAtMaxEnemyDistance = 9000.f;

	UPROPERTY(Category = "OPAG | Character | LookupInteractable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LookUpCapsuleRadius = 20.f;

	UPROPERTY(Category = "OPAG | Character | LookupInteractable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractableTypesQuery;

private: 
	UPROPERTY(Category = "OPAG | Character | Ability System", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> PlayerShieldAbility;

	UPROPERTY(Category = "OPAG | Character | Ability System", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> Grenade;

	UPROPERTY(Category = "OPAG | Character | Ability System", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> HealthPotion;

	UPROPERTY(Category = "OPAG | Character | Ability System", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> Dash;

private:
	UPROPERTY(Category = "OPAG | Character | Debug", EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ToolTip = "With this variable is possible to modify the main stats of current gun."))
	FOPAG_GunStats DebugCurretGunStats;

	UPROPERTY(Category = "OPAG | Character | Debug", EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ToolTip = "With this variable is possible to modify the modules of primary gun."))
	FOPAG_GunModules DebugCurretGunModules;

	UPROPERTY(Category = "OPAG | Character | Debug", EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ToolTip = "With this variable is possible to modify the primary gun's type."))
	TSubclassOf<AOPAG_ModularGun> DebugCurrentPrimaryGun;

	UPROPERTY(Category = "OPAG | Character | Debug", EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ToolTip = "Modify the max player's life."))
	int DebugHealthMax = 0;

	UPROPERTY(Category = "OPAG | Character | Debug", EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ToolTip = "Modify the max player's dash count."))
	int DebugDashMaxCount = 0;

	UPROPERTY(Category = "OPAG | Character | Debug", EditInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ToolTip = "Modify the restore timer player's dash."))
	float DebugDashRestoreTime = 0.f;

private:
	UPROPERTY(Category = "OPAG | Character | Camera", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CurrentCamera;

	UPROPERTY(Category = "OPAG | Character | Camera", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector ViewOffset = FVector(.0f, .0f, -35.f);

	UPROPERTY(Category = "OPAG | Character | Camera", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector MeshRifleOffset = FVector(.0f, .0f, -10.f);

	UPROPERTY(Category = "OPAG | Character | Camera", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector MeshHandOffset = FVector(.0f, .0f, 0.f);

	UPROPERTY(Category = "OPAG | Character | Camera", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LookUpDistanceToPlayer = 150.f;

	UPROPERTY(Category = "OPAG | Character | Camera", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DefaultHipSensitivity = 1.f;
	UPROPERTY(Category = "OPAG | Character | Camera", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DefaultADSSensitivity = 0.5f;

private:
	UPROPERTY(Category = "OPAG | Character | Timeline", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* DashTimeline;

	UPROPERTY(Category = "OPAG | Character | Timeline", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* RecoilTimeline;

	UPROPERTY(Category = "OPAG | Character | Timeline", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ADSCurveFloat;

	UPROPERTY(Category = "OPAG | Character | Timeline", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RecoilCurveFloat;

private:
	UPROPERTY(Category = "OPAG | Character | Widgets", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrosshairClass;

private:
	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AOPAG_ModularGun* PrimaryGun;

	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AOPAG_GunBase* SecondaryGun;

	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AOPAG_GunBase* CurrentGun;

	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RecoilYawMin = -.01f;

	UPROPERTY(Category = "OPAG | Character | Guns", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RecoilYawMax = .01f;

private:
	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> RecoilTimelineDirection;

private:
	// All socket names
	const FName SocketCameraName = "SOCKET_Camera";
	const FName SocketRifleName = "SOCKET_Rifle";
	const FName SocketHandgunName = "SOCKET_Handgun";
	const FName SocketHandLeft = "SOCKET_Hand_L";

private:
	// Boolean checks
	bool bIsDashAlreadyInitialized;
	bool bInspecting = false;
	bool bRunning = false;
	bool bIsInADS = false;
	bool bIsPreADS = false;
	bool bHasAlreadyLooked = false;
	bool bIsShieldBroken = false;

	float HipSensitivity;
	float ADSSensitivity;

	// Other objects
	AActor* InteractableObject = nullptr;
	AActor* PreviousLookedAtModule = nullptr;
	AActor* Shield = nullptr;

	// Fire timers
	float LastFireTime = .0f;
private:
	FOnTimelineFloat OnRecoilTimelineProgress;
	FOnTimelineEventStatic OnRecoilTimelineFinished;
	FOnTimelineEventStatic OnDashTimelineFinished;

private:
	FTimerHandle ShootingHanlde;
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
	virtual void ReceiveDamage(const float Damage, AActor* const Causer) override;
	void HealthChanged(const FOnAttributeChangeData& Data);

private:
	// Events system
	void BroadcastEvents();
	void BindEvents();

	// Init standard values
	void InitDefaultValues();
	void InitDashTimeline();
	void InitRecoilTimeline();
	void InitDashValues();

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
	virtual void GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance = 0) override;

	// LookAt intractable items
	void CalculateLookAtLocations(FVector& OutStartLocation, FVector& OutDirection, float MaxDistance);
	AActor* CheckLookAtInteractableItems();
	const FVector CalculateModuleSpawnLocation() const;
	void UpdateInteractableMessage();
	void UpdateComparison();

	// Look-at Enemy distance
	void LookAtEnemy();

	// Interact with items
	void InteractWithModule(AOPAG_ModuleBase* Module);
	void PickupModule(AOPAG_ModuleBase* Module);

	// Interact with guns
	void PickupGun(AOPAG_ModularGun* Gun);

	// Timer
	void ResetShieldTimer();
	void ShieldCooldownTimer();

	// Animation corrections
	void RelocateComponets();
	FVector GetViewLocation() const;

private:
	UFUNCTION() void TeleportPlayerToStart(FTransform StartLocation);

	// Movement player input
	UFUNCTION() void MoveForward(const FInputActionValue& Value);
	UFUNCTION() void MoveRight(const FInputActionValue& Value);
	UFUNCTION() void Lookup(const FInputActionValue& Value);
	UFUNCTION() void Turn(const FInputActionValue& Value);
	UFUNCTION() void Jumping();

	// Shooting input
	UFUNCTION() void StartShooting();
	UFUNCTION() void KeepShooting();
	UFUNCTION() void StopShooting();
	UFUNCTION() void Shoot();

	// Swap guns
	UFUNCTION() void SwapGuns();

	// Reload guns
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

	// Active and remove passive ability 
	UFUNCTION() void ActivePassiveGunAbility(TSubclassOf<UGameplayAbility> PassiveAbility);
	UFUNCTION() void RemovePassiveGunAbility(FGameplayTagContainer PassiveAbilityTagContainer);
private:
	// Timeline behavior | finished and progress
	UFUNCTION() void DashTimelineFinished();
	UFUNCTION() void RecoilTimelineProgress(float Value);
	UFUNCTION() void RecoilTimelineFinished();

public:
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void SavePlayerState();
	UFUNCTION() void ApplyPlayerState(const FOPAG_PlayerState& StateToApply);

public:
	// Callable in animation
	UFUNCTION(BlueprintCallable) const bool GetIsADS() const;
	UFUNCTION(BlueprintCallable) const bool GetIsRunning() const;
	UFUNCTION(BlueprintCallable) void AttachMagazineToHandNotify();
	UFUNCTION(BlueprintCallable) void AttachMagazineToGunNotify();
	UFUNCTION(BlueprintCosmetic) void CompleteInspectNotify();

public:
	// Getter and setter for shield element
	void SetShield(AActor* NewShield);
	AActor* GetShield() const;

	// Getter dash
	float GetDashLenght() const;
	float GetDashSpeed() const;

public:
	FORCEINLINE UCameraComponent* GetCurrentPlayerCamera() const { return CurrentCamera; }
	FORCEINLINE UOPAG_InventorySystemComponent* GetInventorySystemComponent() const { return InventorySystemComp; }
	FORCEINLINE AOPAG_GunBase* GetCurrentGun() const { return CurrentGun; }
	FORCEINLINE AOPAG_ModularGun* GetPrimaryGun() const { return PrimaryGun; }

};