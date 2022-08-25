#include "Characters/Player/OPAG_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ChildActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Engine/DataTable.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/OPAG_AttributeSetBase.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Characters/Player/OPAG_AttributeSetPlayer.h"
#include <AbilitySystemBlueprintLibrary.h>

AOPAG_Player::AOPAG_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_Player::OnOverlapBegin);

	InventorySystemComp = CreateDefaultSubobject<UOPAG_InventorySystemComponent>("InventorySystemComp");

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	if (Camera)
	{
		Camera->SetRelativeLocation(FVector(0, 0, 64.f));
		Camera->bUsePawnControlRotation = true;
		Camera->SetupAttachment(RootComponent);
	}

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	if (Mesh1P)
	{
		Mesh1P->SetOnlyOwnerSee(true);
		Mesh1P->bCastDynamicShadow = false;
		Mesh1P->CastShadow = false;
		Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
		Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
		Mesh1P->SetupAttachment(Camera);
	}

	PrimaryGunComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PrimaryGun"));
	if (PrimaryGunComponent)
	{
		PrimaryGunComponent->SetupAttachment(RootComponent);
	}

	SecondaryGunComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("SecondaryGun"));
	if (SecondaryGunComponent)
	{
		SecondaryGunComponent->SetupAttachment(RootComponent);
	}

	ADSTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ADSTimeline"));
	DashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DashTimeline"));
	RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilTimeline"));
}

void AOPAG_Player::BeginPlay()
{
	Super::BeginPlay();

	// Set Crosshair widget
	UUserWidget* Crosshair = CreateWidget<UUserWidget>(GetWorld(), CrosshairClass);
	Crosshair->AddToViewport();
	
	// Init player health
	if (BaseAttributeSet && AbilitySystemComp)
	{
		AttributeSetPlayer = Cast<UOPAG_AttributeSetPlayer>(BaseAttributeSet);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &AOPAG_Player::HealthChanged);
	}

	GetCharacterMovement()->DisableMovement();

	InitializeAbilities();
	InitDefaultValues();
	InitADSTimeline();
	InitDashTimeline();
	InitRecoilTimeline();

	// Attach primary and secondary guns on player
	AttachDefaultGuns();

	// Binds and broad all player's events
	BindEvents();
	BroadcastEvents();
}

void AOPAG_Player::Death()
{
	Super::Death();
	SavePlayerState();
	EMS->PlayerDied.Broadcast();
}

void AOPAG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInADS)
	{
		MakingAimCameraRotate();
	}
	// Initialize the object seen by the player
	InteractableObject = CheckLookAtInteractableItems();

	UpdateComparison();

	if (Shield != nullptr)
	{
		Shield->SetActorLocation(GetActorLocation());
	}
}

void AOPAG_Player::ReceiveDamage(const float Damage)
{
	check(AttributeSetPlayer);

	if (AttributeSetPlayer->GetShieldCapacity() <= 0 || bIsShieldBroken)
	{
		Super::ReceiveDamage(Damage);

		// Restart Shield
		GetWorld()->GetTimerManager().ClearTimer(RestartShieldPlayerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ShieldPlayerCooldownHandle);
		GetWorld()->GetTimerManager().SetTimer(ShieldPlayerCooldownHandle, this, &AOPAG_Player::ShieldCooldownTimer, AttributeSetPlayer->GetShieldCooldown(), false);
		bIsShieldBroken = true;
		UE_LOG(LogTemp, Warning, TEXT("Start Shield Recharge Cooldown"));
		return;
	}
	if (AbilitySystemComp && BaseAttributeSet)
	{
		AbilitySystemComp->SetNumericAttributeBase(BaseAttributeSet->GetDamageTakenAmountAttribute(), Damage);

		//TODO: Rename BasicShot in AbilityTakeDamage
		AbilitySystemComp->TryActivateAbilityByClass(PlayerShieldAbility);

		const FGameplayTag HitGameplayTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.shield.hitEvent"));
		FGameplayEventData HitEventData = FGameplayEventData();
		HitEventData.Target = this;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitGameplayTag, HitEventData);
		UE_LOG(LogTemp, Warning, TEXT("Shield Capacity: %f"), AttributeSetPlayer->GetShieldCapacity());
	}
}

#if WITH_EDITOR
void AOPAG_Player::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Health
	static const FName NameCurrentHealthMax = GET_MEMBER_NAME_CHECKED(AOPAG_Player, DebugHealthMax);

	// Guns and Modules
	static const FName NameCurrentGunStats = GET_MEMBER_NAME_CHECKED(AOPAG_Player, DebugCurretGunStats);
	static const FName NameCurrentGunModules = GET_MEMBER_NAME_CHECKED(AOPAG_Player, DebugCurretGunModules);
	static const FName NameCurrentPrimaryGun = GET_MEMBER_NAME_CHECKED(AOPAG_Player, DebugCurrentPrimaryGun);

	// Dash
	static const FName NameDashMaxCount = GET_MEMBER_NAME_CHECKED(AOPAG_Player, DebugDashMaxCount);
	static const FName NameDashRestoreTime = GET_MEMBER_NAME_CHECKED(AOPAG_Player, DebugDashRestoreTime);

	if (PropertyChangedEvent.Property)
	{
		const FName PropName = PropertyChangedEvent.MemberProperty->GetFName();
		if (PropName == NameCurrentHealthMax && BaseAttributeSet)
		{
			SetHealthMax(DebugHealthMax);
		}
		if (PropName == NameCurrentGunStats)
		{
			CurrentGun->Stats = DebugCurretGunStats;
		}
		if (PropName == NameCurrentGunModules)
		{
			PrimaryGun->CreateAndAttachAllModules(DebugCurretGunModules);
			DebugCurretGunStats = CurrentGun->Stats;
		}
		if (PropName == NameCurrentPrimaryGun)
		{
			PrimaryGunComponent->SetChildActorClass(DebugCurrentPrimaryGun);
			if (PrimaryGunComponent->GetChildActor())
			{
				AActor* Target = PrimaryGunComponent->GetChildActor();
				Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
				PrimaryGun = Cast<AOPAG_ModularGun>(Target);
				CurrentGun = PrimaryGun;
			}
		}
		if (PropName == NameDashMaxCount)
		{
			AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetDashCountAttribute(), DebugDashMaxCount);
		}
		if (PropName == NameDashRestoreTime)
		{
			AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetDashRestoreTimeAttribute(), DebugDashRestoreTime);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AOPAG_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOPAG_Player::TeleportPlayerToStart(FTransform StartLocation)
{
	TeleportTo(StartLocation.GetLocation(), FRotator::ZeroRotator);
	GetController()->SetControlRotation(StartLocation.Rotator());
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
}

#pragma region BindsAndBroadcast
void AOPAG_Player::BroadcastEvents()
{
	EMS->TotalAmmoChanged.Broadcast(CurrentGun->AmmoMax);
	EMS->CurrentAmmoChanged.Broadcast(CurrentGun->AmmoCurrent);
	EMS->DashAmountChanged.Broadcast(AttributeSetPlayer->GetDashCount());
	EMS->RequestPlayerState.Broadcast();

	if (AttributeSetPlayer)
	{
		for (int i = 0; i < AttributeSetPlayer->GetDashMaxCount(); i++) 
		{
			EMS->DashAmountIncrement.Broadcast();
		}
	}
}

void AOPAG_Player::BindEvents()
{
	EMS->RequestSavePlayerState.AddDynamic(this, &AOPAG_Player::SavePlayerState);
	EMS->ApplyPlayerState.AddDynamic(this, &AOPAG_Player::ApplyPlayerState);

	EMS->PlacePlayerAtStart.AddDynamic(this, &AOPAG_Player::TeleportPlayerToStart);

	// Enhanced
	EMS->MoveForward.AddDynamic(this, &AOPAG_Player::MoveForward);
	EMS->MoveRight.AddDynamic(this, &AOPAG_Player::MoveRight);
	EMS->Lookup.AddDynamic(this, &AOPAG_Player::Lookup);
	EMS->Turn.AddDynamic(this, &AOPAG_Player::Turn);
	EMS->Jumping.AddDynamic(this, &AOPAG_Player::Jumping);
	EMS->StopJumping.AddDynamic(this, &AOPAG_Player::StopJumping);
	EMS->Shoot.AddDynamic(this, &AOPAG_Player::Shoot);
	EMS->StopShoot.AddDynamic(this, &AOPAG_Player::StopShoot);
	EMS->SwapGuns.AddDynamic(this, &AOPAG_Player::SwapGuns);
	EMS->ReloadGun.AddDynamic(this, &AOPAG_Player::ReloadGun);
	EMS->InteractWithItem.AddDynamic(this, &AOPAG_Player::InteractWithItem);
	EMS->SwitchToADSCamera.AddDynamic(this, &AOPAG_Player::SwitchToADSCamera);
	EMS->ResetFPSCamera.AddDynamic(this, &AOPAG_Player::ResetFPSCamera);
	EMS->UsePotion.AddDynamic(this, &AOPAG_Player::UsePotion);
	EMS->ActiveDash.AddDynamic(this, &AOPAG_Player::ActiveDash);
	EMS->ActiveGunAbility.AddDynamic(this, &AOPAG_Player::ActiveGunAbility);

	EMS->AmmoPickedUp.AddDynamic(this, &AOPAG_Player::IncrementPrimaryGunAmmo);
}
#pragma endregion BindsAndBroadcast

#pragma region Init
void AOPAG_Player::InitDefaultValues()
{
	// Set secondary with hidden because the primary guns is start gun
	if (SecondaryGunComponent->GetChildActor())
	{
		SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
	}
	if (PrimaryGunComponent->GetChildActor())
	{
		CurrentGun = Cast<AOPAG_GunBase>(PrimaryGunComponent->GetChildActor());
		PrimaryGun = Cast<AOPAG_ModularGun>(PrimaryGunComponent->GetChildActor());
		DebugCurretGunStats = CurrentGun->Stats;
		DebugCurretGunModules = PrimaryGun->GetModulesStruct();
	}
	if (Camera)
	{
		CurrentCamera = Camera;
	}
	if (AttributeSetPlayer != nullptr)
	{
		DebugHealthMax = AttributeSetPlayer->GetMaxHealth();
		DebugDashMaxCount = AttributeSetPlayer->GetDashMaxCount();
		DebugDashRestoreTime = AttributeSetPlayer->GetDashRestoreTime();
	}

	bIsDashAlreadyInitialized = false;
}

void AOPAG_Player::InitADSTimeline()
{
	checkf(ADSCurveFloat, TEXT("ADS curve float is required for the ADS to rotate properly."));
	if (ADSCurveFloat)
	{
		ADSTimeline->SetLooping(false);
		ADSTimeline->SetTimelineLength(0);

		OnADSTimelineProgress.BindUFunction(this, FName("ADSTimelineProgress"));
		ADSTimeline->AddInterpFloat(ADSCurveFloat, OnADSTimelineProgress);
	}
}

void AOPAG_Player::InitDashTimeline()
{
	DashTimeline->SetLooping(false);
	DashTimeline->SetTimelineLength(5.f);
	DashTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	OnDashTimelineFinished.BindUFunction(this, FName("DashTimelineFinished"));
	DashTimeline->SetTimelineFinishedFunc(OnDashTimelineFinished);
}

void AOPAG_Player::InitRecoilTimeline()
{
	if (RecoilCurveFloat)
	{
		RecoilTimeline->SetLooping(false);
		RecoilTimeline->SetTimelineLength(0);

		OnRecoilTimelineProgress.BindUFunction(this, FName("RecoilTimelineProgress"));
		RecoilTimeline->AddInterpFloat(RecoilCurveFloat, OnRecoilTimelineProgress);
	}
}

void AOPAG_Player::InitDashValues()
{
	if (bIsDashAlreadyInitialized)
		return;

	bIsDashAlreadyInitialized = true;

	if (AttributeSetPlayer->GetDashCount() < AttributeSetPlayer->GetDashMaxCount() && bIsFirstDash)
	{
		bIsFirstDash = false;
		DashTimeline->PlayFromStart();
	}

	for (int i = 0; i < AttributeSetPlayer->GetDashCount(); i++) {
		EMS->DashAmountIncrement.Broadcast();
	}

	EMS->DashAmountChanged.Broadcast(AttributeSetPlayer->GetDashCount());
}
#pragma endregion Init

#pragma region MathematicUtility
const FVector AOPAG_Player::CalculateThrowForce() const
{
	const FVector ForwardWithForce = AActor::GetActorForwardVector() * TossForce;
	const FVector UpWithForce = AActor::GetActorUpVector() * TossForce;
	const FVector Force = ForwardWithForce + UpWithForce;
	return Force;
}
#pragma endregion MathematicUtility

#pragma region Ability
void AOPAG_Player::InitializeAbilities()
{
	InitializeAbility(PlayerShieldAbility, 0);

	// Initialize and active stamina ability for health behavior
	InitializeAbility(Stamina, 0);
	AbilitySystemComp->TryActivateAbilityByClass(Stamina, true);

	// Initialize grenade ability (player launch the grenade with appropriate accessory)
	InitializeAbility(Grenade, 0);

	// Initialize dash ability
	InitializeAbility(Dash, 0);

	// This ability allow you to cure the player
	InitializeAbility(HealthPotion, 0);

	// This ability allow you to shield the player
	InitializeAbility(ShieldAbility, 0);
}

void AOPAG_Player::ActiveDash()
{
	if (AbilitySystemComp && DashTimeline && AttributeSetPlayer->GetDashCount() > 0)
	{
		const bool bSuccess = AbilitySystemComp->TryActivateAbilityByClass(Dash, true);
		if (bSuccess)
		{
			AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetShieldCapacityAttribute(), AttributeSetPlayer->GetDashCount() - 1);

			EMS->DashAmountChanged.Broadcast(AttributeSetPlayer->GetDashCount());

			EMS->DashAmountDecrement.Broadcast();

			if (bIsFirstDash)
			{
				bIsFirstDash = false;
				DashTimeline->PlayFromStart();
			}
		}
	}
}

void AOPAG_Player::ActiveGunAbility()
{
	const bool bIsPrimaryGun = IsPrimaryGunActive();
	if (AbilitySystemComp && bIsPrimaryGun && PrimaryGun->Accessory->GetChildActor())
	{
		AOPAG_AccessoryBase* Module = Cast<AOPAG_AccessoryBase>(PrimaryGun->Accessory->GetChildActor());
		if (Module->Ability)
		{
			AbilitySystemComp->TryActivateAbilityByClass(Module->Ability, true);
		}
	}
}
#pragma endregion Ability

#pragma region Guns
// The method is invoke in begin play for attach all guns with player's mesh
void AOPAG_Player::AttachDefaultGuns()
{
	if (PrimaryGunComponent->GetChildActor())
	{
		AActor* Target = PrimaryGunComponent->GetChildActor();
		Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
	}
	if (SecondaryGunComponent->GetChildActor())
	{
		AActor* Target = SecondaryGunComponent->GetChildActor();
		Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
	}
}

void AOPAG_Player::SwapGuns()
{
	if (PrimaryGunComponent->GetChildActor() && SecondaryGunComponent->GetChildActor())
	{
		AOPAG_GunBase* SecondaryGun = Cast<AOPAG_GunBase>(SecondaryGunComponent->GetChildActor());
		const bool bIsPrimaryGun = IsPrimaryGunActive();
		if (bIsPrimaryGun)
		{
			PrimaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
			SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(false);
			CurrentGun = SecondaryGun;
		}
		else
		{
			PrimaryGunComponent->GetChildActor()->SetActorHiddenInGame(false);
			SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
			CurrentGun = PrimaryGun;
		}

		DebugCurretGunStats = CurrentGun->Stats;
		EMS->TotalAmmoChanged.Broadcast(CurrentGun->AmmoMax);
		EMS->CurrentAmmoChanged.Broadcast(CurrentGun->AmmoCurrent);
	}
}

void AOPAG_Player::ReloadGun()
{
	const bool CheckIsReloading = CurrentGun->bIsReloading;
	const bool bIsPrimaryGun = IsPrimaryGunActive();
	if (bIsPrimaryGun && !CheckIsReloading && PrimaryGun->AmmoCurrent < PrimaryGun->Stats.ClipSize)
	{
		PrimaryGun->Reload();
		Mesh1P->GetAnimInstance()->Montage_Play(ReloadMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
	}
}

void AOPAG_Player::IncrementPrimaryGunAmmo()
{
	if (PrimaryGun) 
	{
		PrimaryGun->IncrementAmmo();
	}
}

const bool AOPAG_Player::IsPrimaryGunActive() const
{
	return CurrentGun == PrimaryGun;
}
#pragma endregion Guns

#pragma region Shooting Behavior
// This method is link with input fire 
// Calculate standard direction for shoot and invoke shoot behavior with gun and star fire's animation
void AOPAG_Player::Shoot()
{
	if (CheckCanShoot())
	{
		FVector StartLocation;
		FVector Direction;
		GetShootingDirection(StartLocation, Direction);
		CurrentGun->Shoot(StartLocation, Direction, this);
		//TODO: Should this be on the gun?
		MakeNoise(1, this);
		Mesh1P->GetAnimInstance()->Montage_Play(FireMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
		RecoilTimeline->PlayFromStart();

		EMS->TotalAmmoChanged.Broadcast(CurrentGun->AmmoMax);
		EMS->CurrentAmmoChanged.Broadcast(CurrentGun->AmmoCurrent);
	}
}

void AOPAG_Player::StopShoot()
{
	CurrentGun->ResetReductionSpreadRadiusShooting();
}

// Check if guns can shoot (control if gun is shooting, if player isn't reloading gun and gun has enough ammo)
const bool AOPAG_Player::CheckCanShoot() const
{
	const bool bCanShoot = CurrentGun->bCanShoot && !CurrentGun->bIsReloading && CurrentGun->AmmoCurrent > 0;
	return bCanShoot;
}

// Override the default method GetShootingDirection 
// The start location is equal to the camera's world location
// The direction is equal to camera's rotation with vector x
void AOPAG_Player::GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance)
{
	OutStartLocation = CurrentCamera->GetComponentLocation();
	OutDirection = CurrentCamera->GetComponentRotation().Vector();
}
#pragma endregion Shooting Behavior

#pragma region Movement Input
void AOPAG_Player::MoveForward(const FInputActionValue& Value)
{
	AxisY = Value[1];
	if (AxisY != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), AxisY);
	}
}

void AOPAG_Player::MoveRight(const FInputActionValue& Value)
{
	AxisX = Value[0];
	if (AxisX != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), AxisX);
	}
}

void AOPAG_Player::Lookup(const FInputActionValue& Value)
{
	const float LookupScale = Value[1];
	if (LookupScale != 0.0f)
	{
		AddControllerPitchInput(LookupScale);
	}
}

void AOPAG_Player::Turn(const FInputActionValue& Value)
{
	const float TurnScale = Value[0];
	if (TurnScale != 0.0f)
	{
		AddControllerYawInput(TurnScale);
	}
}

void AOPAG_Player::Jumping()
{
	if (JumpCurrentCount < JumpMaxCount)
	{
		ACharacter::Jump();
	}
}
#pragma endregion Movement Input

#pragma region Camera
void AOPAG_Player::MakingAimCameraRotate()
{
	const FQuat ControlRotation = GetControlRotation().Quaternion();
	Camera->SetWorldRotation(ControlRotation, false, nullptr, ETeleportType::None);
}

void AOPAG_Player::SwitchToADSCamera()
{
	// Set ADS and initialize new camera view
	AActor* NewViewTarget = nullptr;
	bIsInADS = true;
	const bool bIsPrimaryGun = IsPrimaryGunActive();
	if (bIsPrimaryGun && PrimaryGun->Sight->GetChildActor())
	{
		NewViewTarget = PrimaryGun->Sight->GetChildActor();
		CurrentCamera = PrimaryGun->GetSightCamera();
	}
	else
	{
		NewViewTarget = CurrentGun;
		CurrentCamera = CurrentGun->GetADSCamera();
	}

	// Set new camera view 
	// If gun has the sight, so is possible to active sight camera else active ads camera 
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(NewViewTarget, .2f, EViewTargetBlendFunction::VTBlend_Linear, 0, false);

	// ADS hand animation play
	if (!bIsPreADS)
	{
		GunPreADSRoll = CurrentGun->GetRootComponent()->GetComponentRotation().Pitch;
		bIsPreADS = true;
	}
	if (ADSTimeline)
	{
		ADSTimeline->Play();
	}
}

void AOPAG_Player::ResetFPSCamera()
{
	bIsInADS = false;
	CurrentCamera = Camera;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(this, .2f, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
	
	if (ADSTimeline)
	{
		ADSTimeline->Reverse();
	}
}

#pragma endregion Camera

#pragma region LookAtInteractable
void AOPAG_Player::InteractWithItem()
{
	if (InteractableObject && 
		InteractableObject->GetClass()->ImplementsInterface(UOPAG_InteractableInterface::StaticClass()))
	{
		if (IOPAG_InteractableInterface::Execute_CanInteract(InteractableObject))
		{
			if (AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(InteractableObject))
			{
				InteractWithModule(Module);
			}
			else if (AOPAG_ModularGun* Gun = Cast<AOPAG_ModularGun>(InteractableObject))
			{
				PickupGun(Gun);
			}
			else
			{
				IOPAG_InteractableInterface::Execute_Interact(InteractableObject);
			}
		}
		if (IOPAG_InteractableInterface::Execute_CanBuy(InteractableObject))
		{
			InventorySystemComp->GoldAmount -= IOPAG_InteractableInterface::Execute_GetPriceTotal(InteractableObject);
			EMS->GoldChanged.Broadcast(InventorySystemComp->GoldAmount);

			if (AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(InteractableObject))
			{
				EMS->DisableModuleTextInVendor.Broadcast(Module);
				InteractWithModule(Module);
			}
			else if (AOPAG_ModularGun* Gun = Cast<AOPAG_ModularGun>(InteractableObject))
			{
				EMS->DisableGunTextInVendor.Broadcast(Gun);
				PickupGun(Gun);
			}
		}
	}
}

void AOPAG_Player::InteractWithModule(AOPAG_ModuleBase* Module)
{
	const int EquippableType = Module->EquippableFireTypes;
	const int FireType = (int)CurrentGun->FireType;
	const int FireMask = 1 << FireType;
	if (FireMask & EquippableType)
	{
		PickupModule(Module);
	}
}

void AOPAG_Player::PickupModule(AOPAG_ModuleBase* Module)
{
	if (PrimaryGun)
	{
		// Initialize new module params
		const FOPAG_GunStats NewModuleStats = Module->Stats;
		const EModuleRarity NewModuleRarity = Module->Rarity;
		const int NewModuleEquippable = Module->EquippableFireTypes;
		const TSubclassOf<AOPAG_ModuleBase> NewModuleClass = Module->GetClass();

		// Create old module params
		FOPAG_GunStats OldModuleStats;
		EModuleRarity OldModuleRarity;
		int OldModuleEquippable;
		TSubclassOf<AOPAG_ModuleBase> OldModuleClass;
		
		const bool bResult = PrimaryGun->AddNewModule(NewModuleClass, NewModuleRarity, NewModuleStats, NewModuleEquippable, OldModuleClass, OldModuleRarity, OldModuleStats, OldModuleEquippable);
		if (bResult)
		{
			Module->Destroy();

			// Control if gun equip the module
			if (OldModuleClass)
			{
				// Spawn old module in scene
				const FVector ModuleSpawnLocation = CalculateModuleSpawnLocation();
				FActorSpawnParameters SpawnParams;
				AOPAG_ModuleBase* OldModule = GetWorld()->SpawnActor<AOPAG_ModuleBase>(OldModuleClass, ModuleSpawnLocation, FRotator::ZeroRotator, SpawnParams);
				OldModule->SetParameters(OldModuleRarity, OldModuleStats, OldModuleEquippable);
				OldModule->ApplyForce_Implementation(CalculateThrowForce());
			}
		}
	}
}

void AOPAG_Player::PickupGun(AOPAG_ModularGun* Gun)
{
	//New Gun Modules
	const int NewAmmoCurrent = Gun->AmmoCurrent;
	const int NewAmmoMax = Gun->AmmoMax;
	const TSubclassOf<AOPAG_ModularGun> NewGunClass = Gun->GetClass();
	const FOPAG_GunModules NewGunModules = Gun->GetModulesStruct();

	//Old Gun Modules
	const int OldAmmoCurrent = PrimaryGun->AmmoCurrent;
	const int OldAmmoMax = PrimaryGun->AmmoMax;
	const TSubclassOf<AOPAG_ModularGun> OldGunClass = PrimaryGun->GetClass();
	const FOPAG_GunModules OldGunModules = PrimaryGun->GetModulesStruct();

	if (NewGunClass == nullptr || OldGunClass == nullptr)
	{
		return;
	}
	
	// Destroy the pickup's gun
	Gun->Destroy();

	// Attach new gun
	PrimaryGunComponent->SetChildActorClass(nullptr);
	PrimaryGunComponent->SetChildActorClass(NewGunClass);
	if (PrimaryGunComponent->GetChildActor())
	{
		AActor* Target = PrimaryGunComponent->GetChildActor();
		Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
		PrimaryGun = Cast<AOPAG_ModularGun>(Target);
		PrimaryGun->SetAmmoParams(NewAmmoCurrent, NewAmmoMax);
		PrimaryGun->CreateAndAttachAllModules(NewGunModules);
		CurrentGun = PrimaryGun;
		DebugCurretGunStats = CurrentGun->Stats;
	}

	// Spawn the old primary gun in scene
	const FVector GunSpawnLocation = CalculateModuleSpawnLocation();
	AOPAG_ModularGun* OldGun = GetWorld()->SpawnActor<AOPAG_ModularGun>(OldGunClass, GunSpawnLocation, FRotator::ZeroRotator);
	OldGun->SetAmmoParams(OldAmmoCurrent, OldAmmoMax);
	OldGun->CreateAndAttachAllModules(OldGunModules);
	OldGun->DetachAllModules(CalculateThrowForce());
	OldGun->ApplyForce_Implementation(CalculateThrowForce());
}

const FVector AOPAG_Player::CalculateModuleSpawnLocation() const
{
	const FVector SpawnLocation = AActor::GetActorLocation() + AActor::GetActorForwardVector() * 100.f;
	return SpawnLocation;
}

void AOPAG_Player::CalculateLookAtLocations(FVector& OutStartLocation, FVector& OutDirection)
{
	const FVector CameraRotationToForward = FRotationMatrix(Camera->GetComponentRotation()).GetScaledAxis(EAxis::X);
	const FVector CameraLocation = Camera->GetComponentLocation();

	OutStartLocation = (CameraRotationToForward * LookUpDistanceToPlayer) + CameraLocation;
	OutDirection = OutStartLocation + (CameraRotationToForward * LookUpMaxDistance);
}

AActor* AOPAG_Player::CheckLookAtInteractableItems()
{
	// Calculate main location for tracing
	FVector CameraStartLocation;
	FVector CameraDirection;
	CalculateLookAtLocations(CameraStartLocation, CameraDirection);

	// Calculate Tracing
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	this->GetAllChildActors(ActorsToIgnore);
	ActorsToIgnore.Add(this);
	bool bHasHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), Camera->GetComponentLocation(), CameraDirection, LookUpCapsuleRadius, 0, InteractableTypesQuery,
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.f);
	if (bHasHit && OutHits.Num() > 0)
	{
		for (int i = 0; i < OutHits.Num(); ++i) {
			const bool bActorImplementsInterface = OutHits[i].GetActor()->GetClass()->
			ImplementsInterface(UOPAG_InteractableInterface::StaticClass());
			if (bActorImplementsInterface)
			{	
				return OutHits[i].GetActor();
			}
		}
	}
	return nullptr;
}

void AOPAG_Player::UpdateComparison() {

	if (InteractableObject == PreviousLookedAtModule)
		return;

	if (InteractableObject == nullptr) {
		EMS->UnlookedAtModule.Broadcast();
		EMS->UnlookedAtGun.Broadcast();

		PreviousLookedAtModule = InteractableObject;
	}

	else if (InteractableObject != nullptr) {
		AOPAG_ModularGun* Gun = Cast<AOPAG_ModularGun>(InteractableObject);
		if (Gun && IOPAG_InteractableInterface::Execute_CanInteract(Gun)) {

			const FOPAG_GunStats OtherGunStats = Gun->Stats;
			const FString OtherGunName = (Gun->Name).ToString();

			EMS->LookedAtGun.Broadcast(PrimaryGun->Stats, OtherGunStats, OtherGunName);

			PreviousLookedAtModule = InteractableObject;
		}

		AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(InteractableObject);
		if (Module && IOPAG_InteractableInterface::Execute_CanInteract(Module)) {

			FOPAG_GunStats CurrentModuleStats;
			AOPAG_ModuleBase* CurrentModule = PrimaryGun->GetModuleBySocketType(Module->Type);

			if (CurrentModule) {
				CurrentModuleStats = CurrentModule->Stats;
			}

			EMS->LookedAtModule.Broadcast(PrimaryGun->Stats, CurrentModuleStats, Module->Stats,
				Module->Name, Module->Rarity);

			PreviousLookedAtModule = InteractableObject;
		}
	}

}
#pragma endregion LookAtInteractable

#pragma region Others Input
void AOPAG_Player::UsePotion()
{
	if (InventorySystemComp->PotionsAmount > 0 && AbilitySystemComp)
	{
		const bool bSuccess = AbilitySystemComp->TryActivateAbilityByClass(HealthPotion, true);
		if (bSuccess)
		{
			InventorySystemComp->PotionsAmount--;
			EMS->PotionsChanged.Broadcast(InventorySystemComp->PotionsAmount);
		}
	}
}
#pragma endregion Others Input

#pragma region GET
const bool AOPAG_Player::GetIsADS() const
{
	return bIsInADS;
}
#pragma endregion GET

#pragma region Timeline
void AOPAG_Player::ADSTimelineProgress(float Value)
{
	const FRotator GunRotation = CurrentGun->GetRootComponent()->GetComponentRotation();
	const float BlendPitch = UKismetMathLibrary::Lerp(GunPreADSRoll, 0, Value);
	const FRotator BlendRotation(BlendPitch, GunRotation.Yaw, GunRotation.Roll);
	CurrentGun->GetRootComponent()->SetWorldRotation(BlendRotation);
}

void AOPAG_Player::DashTimelineFinished()
{
	DashTimeline->SetTimelineLength(AttributeSetPlayer->GetDashRestoreTime());
	AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetShieldCapacityAttribute(), AttributeSetPlayer->GetDashCount() + 1);
	EMS->DashAmountChanged.Broadcast(AttributeSetPlayer->GetDashCount());
	EMS->DashAmountIncrement.Broadcast();

	if (AttributeSetPlayer->GetDashCount() < AttributeSetPlayer->GetDashMaxCount())
	{
		DashTimeline->PlayFromStart();
	}
	else
	{
		bIsFirstDash = true;
		DashTimeline->SetNewTime(0);
	}
}

void AOPAG_Player::RecoilTimelineProgress(float Value)
{
	const float GunRecoilInSeconds = CurrentGun->CalculateRateInSeconds(CurrentGun->Stats.RecoilRate);
	const float RecoilPitch = GunRecoilInSeconds * Value;
	AddControllerPitchInput(RecoilPitch);
}

void AOPAG_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOPAG_Drop* object = Cast<AOPAG_Drop>(OtherActor);
	if (object) {
		InventorySystemComp->AnalyzeItem(object);
	}
}

void AOPAG_Player::SavePlayerState()
{
	FOPAG_PlayerState NewState;

	NewState.LoadType = PlayerStateLoadType::PREVIOUS_STAGE;
	NewState.Health = GetHealth();
	NewState.DashCount = AttributeSetPlayer->GetDashCount();
	NewState.GoldAmount = InventorySystemComp->GoldAmount;
	NewState.BoltsAmount = InventorySystemComp->BoltsAmount;
	NewState.PotionsAmount = InventorySystemComp->PotionsAmount;
	NewState.bIsPrimaryActive = IsPrimaryGunActive();


	PrimaryGun->GetCurrentState(NewState.PrimaryGun);

	EMS->SavePlayerState.Broadcast(NewState);
}

void AOPAG_Player::ApplyPlayerState(const FOPAG_PlayerState& StateToApply)
{
	if (StateToApply.LoadType == PlayerStateLoadType::NOT_LOADED) return;
	if (StateToApply.LoadType == PlayerStateLoadType::PREVIOUS_STAGE)
	{
		SetHealth(StateToApply.Health);
		AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetShieldCapacityAttribute(), StateToApply.DashCount);
		EMS->DashAmountChanged.Broadcast(AttributeSetPlayer->GetDashCount());
		bIsDashAlreadyInitialized = false;
		InitDashValues();	
	
		InventorySystemComp->GoldAmount = StateToApply.GoldAmount;
		InventorySystemComp->PotionsAmount = StateToApply.PotionsAmount;

		if (StateToApply.PrimaryGun.GunClass)
		{
			PrimaryGunComponent->SetChildActorClass(StateToApply.PrimaryGun.GunClass);
			if (PrimaryGunComponent->GetChildActor())
			{
				SwapGuns();

				PrimaryGun = Cast<AOPAG_ModularGun>(PrimaryGunComponent->GetChildActor());
				PrimaryGun->CreateAndAttachAllModules(StateToApply.PrimaryGun.AttachedModules);

				AttachDefaultGuns();

				if (StateToApply.LoadType == PlayerStateLoadType::PREVIOUS_STAGE)
				{
					PrimaryGun->AmmoCurrent = StateToApply.PrimaryGun.AmmoCurrent;
					PrimaryGun->AmmoMax = StateToApply.PrimaryGun.AmmoMax;
				}
				if (StateToApply.bIsPrimaryActive) SwapGuns();

				EMS->CurrentAmmoChanged.Broadcast(CurrentGun->AmmoCurrent);
				EMS->TotalAmmoChanged.Broadcast(CurrentGun->AmmoMax);

			}
		}
	}

	InventorySystemComp->BoltsAmount = StateToApply.BoltsAmount;

	
}
#pragma endregion Timeline

#pragma region Health
void AOPAG_Player::HealthChanged(const FOnAttributeChangeData& Data)
{
	//UE_LOG(LogTemp, Warning, TEXT("New Health: %f"), Data.NewValue);
	//UE_LOG(LogTemp, Warning, TEXT("Max Health: %f"), GetHealthMax());

	EMS->HealthChanged.Broadcast(Data.NewValue, GetHealthMax());
}
#pragma endregion

#pragma region Shield
void AOPAG_Player::SetShield(AActor* NewShield)
{
	Shield = NewShield;
}

AActor* AOPAG_Player::GetShield() const
{
	return Shield;
}
#pragma endregion Shield

#pragma region Timer
void AOPAG_Player::ResetShieldTimer()
{
	if (AbilitySystemComp != nullptr && AttributeSetPlayer != nullptr)
	{
		if (AttributeSetPlayer->GetShieldCapacity() >= AttributeSetPlayer->GetShieldCapacityMax())
		{
			GetWorld()->GetTimerManager().ClearTimer(RestartShieldPlayerHandle);
			bIsShieldBroken = false;
			return;
		}

		const float NewShieldCapicityValue = FMath::Clamp(AttributeSetPlayer->GetShieldCapacity() + AttributeSetPlayer->GetShieldRechargeRate(), 0.f, AttributeSetPlayer->GetShieldCapacityMax());
		AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetShieldCapacityAttribute(), NewShieldCapicityValue);
		UE_LOG(LogTemp, Warning, TEXT("Current Shield Capacity: %f"), AttributeSetPlayer->GetShieldCapacity());
	}
}

void AOPAG_Player::ShieldCooldownTimer()
{
	if (AttributeSetPlayer != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(RestartShieldPlayerHandle, this, &AOPAG_Player::ResetShieldTimer, 1.f, true);
		UE_LOG(LogTemp, Warning, TEXT("Complete Shield Recharge Cooldown"));
	}
}
#pragma endregion Timer
