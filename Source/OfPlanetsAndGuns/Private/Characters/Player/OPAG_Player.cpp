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
#include "Environment/OPAG_Zipline.h"
#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Telemetry/OPAG_PlayerTelemetryComponent.h"
#include "Items/OPAG_Drop.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Weapons/Modules/OPAG_MagazineBase.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Characters/Player/OPAG_AttributeSetPlayer.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include <Animation/AnimBlueprintGeneratedClass.h>

#include "NiagaraComponent.h"
#include "Abilities/OPAG_HealthBuffAbility.h"
#include "Characters/OPAG_CharacterBase.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"

#include "DisplayDebugHelpers.h"

AOPAG_Player::AOPAG_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_Player::OnOverlapBegin);

	InventorySystemComp = CreateDefaultSubobject<UOPAG_InventorySystemComponent>("InventorySystemComp");
	PlayerTelemetryComp = CreateDefaultSubobject<UOPAG_PlayerTelemetryComponent>("PlayerTelemetryComp");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArm != nullptr)
	{
		SpringArm->TargetArmLength = .0f;
		SpringArm->bDoCollisionTest = false;
		SpringArm->SetupAttachment(GetMesh());
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	if (Camera)
	{
		Camera->bUsePawnControlRotation = true;
		Camera->SetupAttachment(SpringArm);
	}

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	if (Mesh1P)
	{
		Mesh1P->SetOnlyOwnerSee(true);
		Mesh1P->bCastDynamicShadow = false;
		Mesh1P->CastShadow = false;
		Mesh1P->SetupAttachment(Camera);
		//if this is set to true then child actor cameras won't be able to see the mesh
		Mesh1P->SetOnlyOwnerSee(false);
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

	DashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DashTimeline"));
	RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilTimeline"));
	RecoilReverseTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilReverseTimeline"));

	DashNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashNiagara"));
	if(DashNiagaraComponent)
	{
		DashNiagaraComponent->SetupAttachment(Camera);
	}

	
}

void AOPAG_Player::BeginPlay()
{
	Super::BeginPlay();

	// Set ViewLocation
	const FVector ViewLocation = GetViewLocation();
	SpringArm->SetRelativeLocation(ViewLocation);

	// Set Cross hair widget
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
	InitDashTimeline();
	InitRecoilTimeline();

	// Attach primary and secondary guns on player
	AttachDefaultGuns();

	// Binds and broad all player's events
	BindEvents();
	BroadcastEvents();

	// 
	InitDashValues();
	InitUICounters();
}

void AOPAG_Player::Death()
{
	Super::Death();
	SMS->PlayCharacterSounds(this, ECharacterSoundAction::Player_Death);
	SavePlayerState();
	EMS->PlayerDied.Broadcast();
}

void AOPAG_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This is important to reset the camera view
	RelocateComponets();

	// Update the field of view so we can do cool changes. We get this value from the Animation Blueprint transitions.
	const float FieldOfViewCurveFloat = Mesh1P->GetAnimInstance()->GetCurveValue("Field Of View");
	Camera->SetFieldOfView(FieldOfViewCurveFloat);

	const FTransform SocketTransform = Mesh1P->GetSocketTransform(SocketCameraName, RTS_Component);
	Camera->SetRelativeRotation(FRotator(-SocketTransform.Rotator().Roll, SocketTransform.Rotator().Pitch,
		SocketTransform.Rotator().Yaw));

	const FVector NewLocationSpring = UKismetMathLibrary::VInterpTo(SpringArm->GetRelativeLocation(),
		GetViewLocation(), DeltaTime, 15.f);
	SpringArm->SetRelativeLocation(NewLocationSpring);

	// Update aim behavior
	if (bIsTryingToADSDuringReload)
	{
		SwitchToADSCamera();
	}
	if (bIsInADS)
	{
		MakingAimCameraRotate();
	}

	// Initialize the object seen by the player
	InteractableObject = CheckLookAtInteractableItems();

	// Module view
	UpdateInteractableMessage();
	UpdateComparison();

	// Check if player sees enemy (for health bar)
	LookAtEnemy();
}

void AOPAG_Player::ReceiveDamage(const float Damage, AActor* const Causer)
{
	check(AttributeSetPlayer);

	if (AttributeSetPlayer->GetShieldCapacity() < AttributeSetPlayer->GetShieldCapacityMax())
	{
		// Restart Shield
		GetWorld()->GetTimerManager().ClearTimer(RestartShieldPlayerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ShieldPlayerCooldownHandle);
		GetWorld()->GetTimerManager().SetTimer(ShieldPlayerCooldownHandle, this, &AOPAG_Player::ShieldCooldownTimer, AttributeSetPlayer->GetShieldCooldown(), false);

		//UE_LOG(LogTemp, Warning, TEXT("Start Shield Recharge Cooldown"));
	}

	//Shows damage indicator widget
	EMS->DamagedBy.Broadcast(Causer);

	if (AttributeSetPlayer->GetShieldCapacity() <= 0 || bIsShieldBroken)
	{
		Super::ReceiveDamage(Damage, Causer);
		
		if (!bIsShieldBroken) {
			bIsShieldBroken = true;
			SMS->PlayCharacterSounds(this, ECharacterSoundAction::Player_Shield_Broken);
		}
		SMS->PlayCharacterSounds(this, ECharacterSoundAction::Player_Damage);
		//UE_LOG(LogTemp, Warning, TEXT("Start Shield Recharge Cooldown"));
		return;
	}
	if (AbilitySystemComp && BaseAttributeSet)
	{		
		AbilitySystemComp->SetNumericAttributeBase(BaseAttributeSet->GetDamageTakenAmountAttribute(), Damage);

		AbilitySystemComp->TryActivateAbilityByClass(PlayerShieldAbility);

		const FGameplayTag HitGameplayTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.shield.hitEvent"));
		FGameplayEventData HitEventData = FGameplayEventData();
		HitEventData.Target = this;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitGameplayTag, HitEventData);
		//UE_LOG(LogTemp, Warning, TEXT("Shield Capacity: %f"), AttributeSetPlayer->GetShieldCapacity());
		SMS->PlayCharacterSounds(this, ECharacterSoundAction::Player_Shield_Hit);
		EMS->ShieldChanged.Broadcast(AttributeSetPlayer->GetShieldCapacity(), 
			AttributeSetPlayer->GetShieldCapacityMax(),
			bIsShieldBroken);
	}
}

void AOPAG_Player::DisplayDebug(UCanvas* InCanvas, const FDebugDisplayInfo& InDebugDisplay, float& InYL, float& InYPos)
{
	Super::DisplayDebug(InCanvas, InDebugDisplay, InYL, InYPos);

	static FName NAME_Telemetry = FName(TEXT("PlayerTelemetry"));

	if (InDebugDisplay.IsDisplayOn(NAME_Telemetry))
	{
		if (PlayerTelemetryComp != nullptr)
		{
			PlayerTelemetryComp->DrawTelemetry(InCanvas, InYL, InYPos);
		}
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
			CurrentGun->SetStatsTotal(DebugCurretGunStats);
		}
		if (PropName == NameCurrentGunModules)
		{
			PrimaryGun->CreateAndAttachAllModules(DebugCurretGunModules);
			DebugCurretGunStats = CurrentGun->GetStatsTotal();
		}
		if (PropName == NameCurrentPrimaryGun)
		{
			PrimaryGunComponent->SetChildActorClass(DebugCurrentPrimaryGun);
			if (PrimaryGunComponent->GetChildActor())
			{
				AActor* Target = PrimaryGunComponent->GetChildActor();
				Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketRifleName);
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
	EMS->TotalAmmoChanged.Broadcast(CurrentGun->GetAmmoMax());
	EMS->CurrentAmmoChanged.Broadcast(CurrentGun->GetAmmoCurrent());
	EMS->RequestPlayerState.Broadcast();
}

void AOPAG_Player::BindEvents()
{
	EMS->RequestSavePlayerState.AddDynamic(this, &AOPAG_Player::SavePlayerState);
	EMS->ApplyPlayerState.AddDynamic(this, &AOPAG_Player::ApplyPlayerState);

	EMS->PlacePlayerAtStart.AddDynamic(this, &AOPAG_Player::TeleportPlayerToStart);

	EMS->PauseGame.AddDynamic(this, &AOPAG_Player::OnPauseGame);

	// Buffer
	EMS->ActivePassiveAbilityEffect.AddDynamic(this, &AOPAG_Player::ActivePassiveGunAbility);
	EMS->RemoveAbilityEffect.AddDynamic(this, &AOPAG_Player::RemovePassiveGunAbility);

	// Enhanced
	EMS->MoveForward.AddDynamic(this, &AOPAG_Player::MoveForward);
	EMS->MoveRight.AddDynamic(this, &AOPAG_Player::MoveRight);
	EMS->Lookup.AddDynamic(this, &AOPAG_Player::Lookup);
	EMS->Turn.AddDynamic(this, &AOPAG_Player::Turn);
	EMS->Jumping.AddDynamic(this, &AOPAG_Player::Jumping);
	EMS->StopJumping.AddDynamic(this, &AOPAG_Player::StopJumping);
	EMS->StartShoot.AddDynamic(this, &AOPAG_Player::StartShooting);
	EMS->KeepShoot.AddDynamic(this, &AOPAG_Player::KeepShooting);
	EMS->StopShoot.AddDynamic(this, &AOPAG_Player::StopShooting);
	EMS->SwapGuns.AddDynamic(this, &AOPAG_Player::SwapGuns);
	EMS->ReloadGun.AddDynamic(this, &AOPAG_Player::ReloadGun);
	EMS->InteractWithItem.AddDynamic(this, &AOPAG_Player::InteractWithItem);
	EMS->SwitchToADSCamera.AddDynamic(this, &AOPAG_Player::SwitchToADSCamera);
	EMS->ResetFPSCamera.AddDynamic(this, &AOPAG_Player::ResetFPSCamera);
	EMS->UsePotion.AddDynamic(this, &AOPAG_Player::UsePotion);
	EMS->ActiveDash.AddDynamic(this, &AOPAG_Player::ActiveDash);
	EMS->ActiveGunAbility.AddDynamic(this, &AOPAG_Player::ActiveGunAbility);

	EMS->AmmoPickedUp.AddDynamic(this, &AOPAG_Player::IncrementPrimaryGunAmmo);
	EMS->InspectGun.AddDynamic(this, &AOPAG_Player::InspectGun);
}
#pragma endregion BindsAndBroadcast

#pragma region Init
void AOPAG_Player::InitDefaultValues()
{
	// Set secondary gun hidden because the primary guns is the starting gun
	if (SecondaryGunComponent->GetChildActor())
	{
		SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
	}
	if (PrimaryGunComponent->GetChildActor())
	{
		CurrentGun = Cast<AOPAG_GunBase>(PrimaryGunComponent->GetChildActor());
		PrimaryGun = Cast<AOPAG_ModularGun>(PrimaryGunComponent->GetChildActor());
		
		// Update debug stats and modules struct
		#if WITH_EDITOR
		DebugCurretGunStats = CurrentGun->GetStatsTotal();
		DebugCurretGunModules = PrimaryGun->GetModulesStruct();
		#endif
	}
	if (Camera)
	{
		CurrentCamera = Camera;
	}
	HipSensitivity = DefaultHipSensitivity;
	ADSSensitivity = DefaultADSSensitivity;

	if (AttributeSetPlayer != nullptr)
	{
		DebugHealthMax = AttributeSetPlayer->GetMaxHealth();
		DebugDashMaxCount = AttributeSetPlayer->GetDashMaxCount();
		DebugDashRestoreTime = AttributeSetPlayer->GetDashRestoreTime();
	}

	bIsDashAlreadyInitialized = false;
	bAreCountersAlreadyInitialized = false;
}

void AOPAG_Player::InitDashTimeline()
{
	DashTimeline->SetLooping(false);
	DashTimeline->SetTimelineLength(DashRestoreTime);
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

		RecoilTimeline->SetPropertySetObject(this);
		RecoilTimeline->SetDirectionPropertyName(FName("RecoilTimelineDirection"));

		OnRecoilTimelineProgress.BindUFunction(this, FName("RecoilTimelineProgress"));
		RecoilTimeline->AddInterpFloat(RecoilCurveFloat, OnRecoilTimelineProgress, FName("Recoil"));

		OnRecoilTimelineFinished.BindUFunction(this, FName("RecoilTimelineFinished"));
		RecoilTimeline->SetTimelineFinishedFunc(OnRecoilTimelineFinished);
	}
	if (RecoilReverseCurveFloat)
	{
		RecoilReverseTimeline->SetLooping(false);
		RecoilReverseTimeline->SetTimelineLength(RecoilReverseLength);

		RecoilReverseTimeline->SetPropertySetObject(this);
		RecoilReverseTimeline->SetDirectionPropertyName(FName("RecoilTimelineDirection"));

		OnRecoilReverseTimelineProgress.BindUFunction(this, FName("RecoilReverseTimelineProgress"));
		RecoilReverseTimeline->AddInterpFloat(RecoilReverseCurveFloat, OnRecoilReverseTimelineProgress, FName("RecoilReverse"));

		OnRecoilReverseTimelineFinished.BindUFunction(this, FName("RecoilReverseTimelineFinished"));
		RecoilReverseTimeline->SetTimelineFinishedFunc(OnRecoilReverseTimelineFinished);
	}
}

void AOPAG_Player::InitDashValues()
{
	if (bIsDashAlreadyInitialized)
		return;

	bIsDashAlreadyInitialized = true;

	if (!AttributeSetPlayer) return;

	if (AttributeSetPlayer->GetDashCount() < AttributeSetPlayer->GetDashMaxCount() && bIsFirstDash)
	{
		bIsFirstDash = false;
		DashTimeline->PlayFromStart();
	}
}

void AOPAG_Player::InitUICounters()
{
	if (bAreCountersAlreadyInitialized)
		return;

	bAreCountersAlreadyInitialized = true;

	int i;
	for (i = 0; i < AttributeSetPlayer->GetDashCount(); ++i) {
		EMS->DashAmountIncrement.Broadcast();
	}

	EMS->PotionsAmountChanged.Broadcast(InventorySystemComp->PotionsAmount);
	EMS->BoltsAmountChanged.Broadcast(InventorySystemComp->BoltsAmount);
}
#pragma endregion Init

#pragma region MathematicUtility
FVector AOPAG_Player::CalculateThrowForce() const
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

	// Initialize grenade ability (player launches a grenade with the appropriate accessory)
	InitializeAbility(Grenade, 0);

	// Initialize dash ability
	InitializeAbility(Dash, 0);

	// This ability allows you to cure the player
	InitializeAbility(HealthPotion, 0);
}

void AOPAG_Player::ActiveDash()
{
	if (AbilitySystemComp && DashTimeline && AttributeSetPlayer->GetDashCount() > 0)
	{
		const bool bSuccess = AbilitySystemComp->TryActivateAbilityByClass(Dash, true);
		if (bSuccess)
		{
			//Speed lines particle effect spawn
			DashNiagaraComponent->Activate();
			
			SMS->PlayCharacterSounds(this, ECharacterSoundAction::Movement_Dash);
			AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetDashCountAttribute(), AttributeSetPlayer->GetDashCount() - 1);

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
	if (AbilitySystemComp && bIsPrimaryGun && PrimaryGun->HasAccessory() && InventorySystemComp->BoltsAmount > 0)
	{
		TSubclassOf<UGameplayAbility> Ability = PrimaryGun->GetActiveAbility();
		if(Ability == nullptr)	return;

		// Initialize ability with ability system component
		InitializeAbility(Ability, 0);

		const bool bResult = AbilitySystemComp->TryActivateAbilityByClass(Ability, true);
		if (bResult)
		{
			InventorySystemComp->BoltsAmount--;
			EMS->BoltsAmountChanged.Broadcast(InventorySystemComp->BoltsAmount);
			//EMS->BoltsAmountDecrement.Broadcast();
		}
	}
}

void AOPAG_Player::ActivePassiveGunAbility(TSubclassOf<UGameplayAbility> PassiveAbility)
{
	if (PassiveAbility == nullptr)	return;

	InitializeAbility(PassiveAbility, 0);
	AbilitySystemComp->TryActivateAbilityByClass(PassiveAbility, true);
}

void AOPAG_Player::RemovePassiveGunAbility(FGameplayTagContainer PassiveAbilityTagContainer)
{
	if(AbilitySystemComp == nullptr)	return;

	AbilitySystemComp->RemoveActiveEffectsWithGrantedTags(PassiveAbilityTagContainer);
}
#pragma endregion Ability

#pragma region Guns
// Method invoked in begin play to attach all guns to the player's mesh
void AOPAG_Player::AttachDefaultGuns()
{
	if (PrimaryGunComponent->GetChildActor())
	{
		AActor* Target = PrimaryGunComponent->GetChildActor();
		Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketRifleName);
	}
	if (SecondaryGunComponent->GetChildActor())
	{
		AActor* Target = SecondaryGunComponent->GetChildActor();
		Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketHandgunName);
	}
}

void AOPAG_Player::SwapGuns()
{
	if (bInspecting)	StopInspectGun();

	// Clear reload, update the last fire time and re-attach the clip on gun
	if (!CurrentGun->CanReloading())
	{
		CurrentGun->StopReloading();
		AttachMagazineToGunNotify();

		// Update last fire time
		LastFireTime = GetGameTimeSinceCreation();
	}

	if (PrimaryGunComponent->GetChildActor() && SecondaryGunComponent->GetChildActor())
	{
		SecondaryGun = Cast<AOPAG_GunBase>(SecondaryGunComponent->GetChildActor());
		UpdateCamera(false);
		const bool bIsPrimaryGun = IsPrimaryGunActive();
		if (bIsPrimaryGun)
		{
			PrimaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
			SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(false);
			CurrentGun = SecondaryGun;
			// Post animation change
			Mesh1P->SetAnimInstanceClass(HandgunAnimClass);
			//Mesh1P->SetRelativeLocation(MeshHandOffset);
			Mesh1P->GetAnimInstance()->Montage_Play(SwapHandgunMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
			SMS->PlayWeaponSounds(CurrentGun, EWeaponSoundAction::Pickup);
			
		}
		else
		{
			PrimaryGunComponent->GetChildActor()->SetActorHiddenInGame(false);
			SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
			CurrentGun = PrimaryGun;
			
			// Post animation change
			Mesh1P->SetAnimInstanceClass(RifleAnimClass);
			//Mesh1P->SetRelativeLocation(MeshRifleOffset);
			Mesh1P->GetAnimInstance()->Montage_Play(SwapRifleMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
			SMS->PlayWeaponSounds(CurrentGun, EWeaponSoundAction::Pickup);
		}
		UpdateCamera(bIsInADS);

		DebugCurretGunStats = CurrentGun->GetStatsTotal();
		EMS->TotalAmmoChanged.Broadcast(CurrentGun->GetAmmoMax());
		EMS->CurrentAmmoChanged.Broadcast(CurrentGun->GetAmmoCurrent());

		// Switch current ammo color
		FLinearColor NewColor = CurrentGun->GetAmmoCurrent() == 0 ? FLinearColor::Red : FLinearColor::White;
		EMS->ColorAmmoChanged.Broadcast(NewColor);
	}
}

void AOPAG_Player::ReloadGun()
{
	if (bInspecting)	StopInspectGun();

	if (CurrentGun->CanReloading())
	{
		if (bIsInADS)
		{
			ResetFPSCamera();
			bIsTryingToADSDuringReload = true;
		}
		// Invoke reload to current gun
		CurrentGun->Reload();
		SMS->PlayWeaponSounds(PrimaryGun, EWeaponSoundAction::Reload);
		const float ReloadAnimationLength = ReloadMontageToPlay->GetPlayLength();
		const float ReloadTime = PrimaryGun->GetReloadTime();
		Mesh1P->GetAnimInstance()->Montage_Play(ReloadMontageToPlay, ReloadAnimationLength / ReloadTime, EMontagePlayReturnType::MontageLength, 0, true);
		
		// Update gun ui color
		EMS->ColorAmmoChanged.Broadcast(FLinearColor::White);

		// Update last fire time
		LastFireTime = GetGameTimeSinceCreation() + ReloadTime;
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

void AOPAG_Player::InspectGun()
{
	if(bInspecting)	return;

	Mesh1P->GetAnimInstance()->Montage_Play(InspectGunMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
	bInspecting = true;
}

void AOPAG_Player::StopInspectGun()
{
	Mesh1P->GetAnimInstance()->Montage_Stop(.01f, InspectGunMontageToPlay);
	bInspecting = false;
}
#pragma endregion Guns

#pragma region Shooting Behavior
void AOPAG_Player::StartShooting()
{
	if(bInspecting)	StopInspectGun();

	if (CurrentGun->CanShoot())
	{
		const float FirePeriod = AOPAG_GunBase::CalculatePeriod(CurrentGun->GetStatsTotal().FireRate);
		if (GetGameTimeSinceCreation() - LastFireTime > FirePeriod)
		{
			Shoot();
		}
	}
}

void AOPAG_Player::KeepShooting()
{
	if (bInspecting)	StopInspectGun();

	if (CurrentGun->CanShoot())
	{
		const float FirePeriod = AOPAG_GunBase::CalculatePeriod(CurrentGun->GetStatsTotal().FireRate);
		float ElapsedTime = GetGameTimeSinceCreation() - LastFireTime;
		while (ElapsedTime > FirePeriod)
		{
			Shoot();
			ElapsedTime -= FirePeriod;
		}
	}
}

void AOPAG_Player::StopShooting()
{
	CurrentGun->StopShoot();
}

// This method is linked with input fire 
// Calculate standard direction for shooting and invoke shoot behavior with gun and start fire animation
void AOPAG_Player::Shoot()
{
	// Calculate shoot direction
	FVector StartLocation;
	FVector Direction;
	GetShootingDirection(StartLocation, Direction);

	// Invoke shoot
	const int AmmoConsumate = CurrentGun->Shoot(StartLocation, Direction, this);

	// Active montage 
	Mesh1P->GetAnimInstance()->Montage_Play(FireMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);

	// Active recoil
	RandomRecoilYaw = FMath::RandRange(RecoilYawMin, RecoilYawMax);
	RandomRecoilYaw *= ((100 - CurrentGun->GetStatsTotal().Stability) / 100) * CurrentGun->GetRecoilStrength();
	RecoilReverseTimeline->Stop();
	RecoilTimeline->SetPlayRate(CurrentGun->GetRecoilSpeed());
	RecoilReverseTimeline->SetPlayRate(CurrentGun->GetRecoilSpeed());
	RecoilTimeline->PlayFromStart();

	// Update last fire time
	LastFireTime = GetGameTimeSinceCreation();

	// Update Ammo
	CurrentGun->DecrementAmmoCurrent(AmmoConsumate);
	EMS->CurrentAmmoChanged.Broadcast(CurrentGun->GetAmmoCurrent());
	if (CurrentGun->GetAmmoCurrent() == 0)
	{
		// Update ui gun icon -> set linear color to red
		EMS->ColorAmmoChanged.Broadcast(FLinearColor::Red);

		// Auto-reload
		ReloadGun();
	}
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
	AddMovementInput(GetActorForwardVector(), AxisY);
}

void AOPAG_Player::MoveRight(const FInputActionValue& Value)
{
	AxisX = Value[0];
	AddMovementInput(GetActorRightVector(), AxisX);
}

void AOPAG_Player::Lookup(const FInputActionValue& Value)
{
	LookupScale = Value[1];
	if (AccumulatedRecoilDeviation.Pitch > .0f && LookupScale > .0f)
	{
		AccumulatedRecoilDeviation.Pitch -= LookupScale;
		if (AccumulatedRecoilDeviation.Pitch <= .0f)
		{
			RecoilReverseTimeline->Stop();
			AccumulatedRecoilDeviation = FRotator::ZeroRotator;
		}
	}
	if (LookupScale != 0.0f)
	{
		AddControllerPitchInput(LookupScale * (bIsInADS ? ADSSensitivity : HipSensitivity));

	}
}

void AOPAG_Player::Turn(const FInputActionValue& Value)
{
	const float TurnScale = Value[0];
	if (TurnScale != 0.0f)
	{
		AddControllerYawInput(TurnScale * (bIsInADS ? ADSSensitivity : HipSensitivity));
	}
}

void AOPAG_Player::Jumping()
{
	if (JumpCurrentCount < JumpMaxCount)
	{
		SMS->PlayCharacterSounds(this, ECharacterSoundAction::Movement_Jump);
		ACharacter::Jump();
	}
}
#pragma endregion Movement Input

#pragma region Camera
void AOPAG_Player::MakingAimCameraRotate()
{
	if (CurrentCamera != Camera)
	{
		const FQuat ControlRotation = Camera->GetComponentRotation().Quaternion();
		CurrentCamera->SetWorldRotation(ControlRotation, false, nullptr, ETeleportType::None);
	}
}

void AOPAG_Player::SwitchToADSCamera()
{
	if (!CurrentGun->CanAim() || bInspecting)
	{
		bIsTryingToADSDuringReload = true;
		return;
	}
	bIsInADS = true;
	bIsTryingToADSDuringReload = false;
	UpdateCamera(true);
}

void AOPAG_Player::ResetFPSCamera()
{
	bIsInADS = false;
	bIsTryingToADSDuringReload = false;
	UpdateCamera(false);
}

void AOPAG_Player::UpdateCamera(bool InADS)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (InADS)
	{
		AActor* NewViewTarget;
		const bool bIsPrimaryGun = IsPrimaryGunActive();
		if (bIsPrimaryGun && PrimaryGun->HasSight())
		{
			NewViewTarget = PrimaryGun->GetModuleBySocketType(EModuleTypes::Sight);
			CurrentCamera = PrimaryGun->GetSightCamera();
		}
		else
		{
			NewViewTarget = CurrentGun;
			CurrentCamera = CurrentGun->GetADSCamera();
		}

		PlayerController->SetViewTargetWithBlend(NewViewTarget, .2f, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
	}
	else
	{
		PlayerController->SetViewTarget(this);
		CurrentCamera = Camera;
	}
}

FVector AOPAG_Player::GetViewLocation() const
{
	const float ScaledCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return FVector(.0f, .0f, ScaledCapsuleHalfHeight) + ViewOffset;
}

void AOPAG_Player::OnPauseGame()
{
	EMS->GoldChanged.Broadcast(GetInventorySystemComponent()->GoldAmount);
}

void AOPAG_Player::RelocateComponets()
{
	// Update the Pitch separately so that it is only applied to the camera.
	const float NewPitch = GetControlRotation().Pitch;
	SpringArm->SetRelativeRotation(FRotator(NewPitch, .0f, .0f));

	// Apply the Yaw to the whole character.
	const float NewYaw = GetControlRotation().Yaw;
	SetActorRelativeRotation(FRotator(.0f, NewYaw, .0f));
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
		//UE_LOG(LogTemp, Log);
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
	const int EquippableType = Module->GetEquippableFireTypes();
	const int FireType = (int)CurrentGun->GetFireType();
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
		const FOPAG_GunStats NewModuleStats = Module->GetStats();
		const EModuleRarity NewModuleRarity = Module->GetRarity();
		const EModuleBrand NewModuleBrand = Module->GetBrand();
		const int NewModuleEquippable = Module->GetEquippableFireTypes();
		const TSubclassOf<AOPAG_ModuleBase> NewModuleClass = Module->GetClass();

		// Create old module params
		FOPAG_GunStats OldModuleStats;
		EModuleRarity OldModuleRarity;
		EModuleBrand OldModuleBrand;
		int OldModuleEquippable;
		TSubclassOf<AOPAG_ModuleBase> OldModuleClass;
		
		const bool bResult = PrimaryGun->AddNewModule(NewModuleClass, NewModuleRarity, NewModuleBrand, 
			NewModuleStats, NewModuleEquippable, OldModuleClass, 
			OldModuleRarity, OldModuleBrand, OldModuleStats, OldModuleEquippable);
		if (bResult)
		{
			AOPAG_MagazineBase* Magazine = Cast<AOPAG_MagazineBase>(Module);
			if (Magazine) 
			{
				EMS->ColorAmmoChanged.Broadcast(FLinearColor::White);
			}

			Module->Destroy();

			// Control if the gun can equip the module
			if (OldModuleClass)
			{
				// Spawn old module in scene
				const FVector ModuleSpawnLocation = CalculateModuleSpawnLocation();
				FActorSpawnParameters SpawnParams;
				AOPAG_ModuleBase* OldModule = GetWorld()->SpawnActor<AOPAG_ModuleBase>(OldModuleClass, ModuleSpawnLocation, FRotator::ZeroRotator, SpawnParams);
				OldModule->SetParameters(OldModuleBrand, OldModuleStats, OldModuleEquippable);
				OldModule->StartDropTimeline();
			}

			// Update debug stats
			#if WITH_EDITOR
			DebugCurretGunStats = CurrentGun->GetStatsTotal();
			DebugCurretGunModules = PrimaryGun->GetModulesStruct();
			#endif
		}
	}
}

void AOPAG_Player::PickupGun(AOPAG_ModularGun* Gun)
{
	//New Gun Modules
	const int NewAmmoCurrent = Gun->GetAmmoCurrent();
	const int NewAmmoMax = Gun->GetAmmoMax();
	const TSubclassOf<AOPAG_ModularGun> NewGunClass = Gun->GetClass();
	const FOPAG_GunModules NewGunModules = Gun->GetModulesStruct();

	//Old Gun Modules
	const int OldAmmoCurrent = PrimaryGun->GetAmmoCurrent();
	const int OldAmmoMax = PrimaryGun->GetAmmoMax();
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
		Target->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketRifleName);
		PrimaryGun = Cast<AOPAG_ModularGun>(Target);
		PrimaryGun->SetAmmoParams(NewAmmoCurrent, NewAmmoMax);
		PrimaryGun->CreateAndAttachAllModules(NewGunModules);
		CurrentGun = PrimaryGun;
		SMS->PlayWeaponSounds(CurrentGun, EWeaponSoundAction::Pickup);
		
		// Update debug stats and struct
		#if WITH_EDITOR
		DebugCurretGunStats = CurrentGun->GetStatsTotal();
		DebugCurretGunModules = PrimaryGun->GetModulesStruct();
		#endif

		// Update ammo
		PrimaryGun->SetAmmoCurrent(PrimaryGun->GetStatsTotal().ClipSize);
		EMS->CurrentAmmoChanged.Broadcast(PrimaryGun->GetAmmoCurrent());
	}

	// Spawn the old primary gun in scene
	const FVector GunSpawnLocation = CalculateModuleSpawnLocation();
	AOPAG_ModularGun* OldGun = GetWorld()->SpawnActor<AOPAG_ModularGun>(OldGunClass, GunSpawnLocation, FRotator::ZeroRotator);
	if (OldGun)
	{
		OldGun->SetAmmoParams(OldAmmoCurrent, OldAmmoMax);
		OldGun->CreateAndAttachAllModules(OldGunModules);
		OldGun->DetachAllModules(CalculateThrowForce());
		OldGun->StartDropTimeline();
	}
}

const FVector AOPAG_Player::CalculateModuleSpawnLocation() const
{
	const FVector SpawnLocation = AActor::GetActorLocation() + AActor::GetActorForwardVector() * 100.f;
	return SpawnLocation;
}

void AOPAG_Player::CalculateLookAtLocations(FVector& OutStartLocation, FVector& OutDirection, float MaxDistance)
{
	const FVector CameraRotationToForward = FRotationMatrix(Camera->GetComponentRotation()).GetScaledAxis(EAxis::X);
	const FVector CameraLocation = Camera->GetComponentLocation();

	OutStartLocation = (CameraRotationToForward * LookUpDistanceToPlayer) + CameraLocation;
	OutDirection = OutStartLocation + (CameraRotationToForward * MaxDistance);
}

AActor* AOPAG_Player::CheckLookAtInteractableItems()
{
	// Calculate main location for tracing
	FVector CameraStartLocation;
	FVector CameraDirection;
	CalculateLookAtLocations(CameraStartLocation, CameraDirection, LookUpMaxDistance);

	// Calculate Tracing
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	this->GetAllChildActors(ActorsToIgnore);
	ActorsToIgnore.Add(this);
	bool bHasHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), Camera->GetComponentLocation(), 
		CameraDirection, LookUpCapsuleRadius, 0, InteractableTypesQuery,
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, 
		FLinearColor::Red, FLinearColor::Green, 5.f);
	OutHits.Sort([](FHitResult Lhs, FHitResult Rhs)
	{
		auto LeftModule = Cast<AOPAG_ModuleBase>(Lhs.GetActor());
		if (LeftModule) return true;
		auto LeftWeapon = Cast<AOPAG_ModularGun>(Lhs.GetActor());
		if (LeftModule) return true;
		return false;
	});
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

void AOPAG_Player::UpdateInteractableMessage() {
	bool bShouldUpdateMessage = false;

	if (InteractableObject) 
	{
		if (IOPAG_InteractableInterface::Execute_WillChangeInteractStateDynamically(InteractableObject))
		{
			bShouldUpdateMessage = true;
		}

		if(!bHasAlreadyLooked)
			bShouldUpdateMessage = true;
	}

	if (!bShouldUpdateMessage && bHasAlreadyLooked) 
	{
		bHasAlreadyLooked = InteractableObject != nullptr;

		if (!bHasAlreadyLooked) 
		{
			EMS->UnlookedAtInteractable.Broadcast();
		}
		else 
			bShouldUpdateMessage = true;
	}

	if (bShouldUpdateMessage && InteractableObject) {
		//IOPAG_InteractableInterface* Int = Cast<IOPAG_InteractableInterface>(InteractableObject);

		AOPAG_ModularGun* const Gun = Cast<AOPAG_ModularGun>(InteractableObject);

		if (Gun && !(IOPAG_InteractableInterface::Execute_CanInteract(InteractableObject)))
				return;

		bool bCanInteract = IOPAG_InteractableInterface::Execute_CanInteract(InteractableObject);
		FString str = IOPAG_InteractableInterface::Execute_InteractString(InteractableObject);

		AOPAG_ModuleBase* const Module = Cast<AOPAG_ModuleBase>(InteractableObject);
		if (Module) {
			int otherModule = Module->GetEquippableFireTypes();
			int myModule = 1 << (int)PrimaryGun->GetFireType();
			bool canEquip = otherModule & myModule;
			bCanInteract = canEquip;
		}

		EMS->LookedAtInteractable.Broadcast(bCanInteract, str);
		bHasAlreadyLooked = true;
	}
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
		if (Gun && ( IOPAG_InteractableInterface::Execute_CanInteract(Gun) || IOPAG_InteractableInterface::Execute_CanBuy(Gun) )) {

			const FOPAG_GunStats OtherGunStats = Gun->GetStatsTotal();
			const FString OtherGunName = (Gun->GetGunName()).ToString();
			const FString OtherGunFireType = AOPAG_ModularGun::GetFireTypeName((Gun->GetFireType()));

			EMS->LookedAtGun.Broadcast(PrimaryGun->GetStatsTotal(), OtherGunStats, OtherGunName, OtherGunFireType);

			PreviousLookedAtModule = InteractableObject;
		}

		AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(InteractableObject);
		if (Module && (IOPAG_InteractableInterface::Execute_CanInteract(Module) || IOPAG_InteractableInterface::Execute_CanBuy(Module)))
		{
			FOPAG_GunStats CurrentModuleStats;
			AOPAG_ModuleBase* CurrentModule = PrimaryGun->GetModuleBySocketType(Module->GetType());

			int CurrentFireTypeMask = 1 << (int)CurrentGun->GetFireType();
			bool CanBeEquipped = (CurrentFireTypeMask & Module->GetEquippableFireTypes());

			if (CurrentModule) {
				CurrentModuleStats = CurrentModule->GetStats();
			}

			FString AbilityDescription = "";
			
			AOPAG_AccessoryBase* Accessory = Cast<AOPAG_AccessoryBase>(Module);

			if (Accessory)
				AbilityDescription = Accessory->AbilityDescription;

			EMS->LookedAtModule.Broadcast(CurrentModuleStats, Module->GetStats(),
				Module->GetModuleName(), AbilityDescription, Module->GetRarity(), Module->GetType(), Module->GetEquippableFireTypes(), CanBeEquipped);

			PreviousLookedAtModule = InteractableObject;
		}
	}

}

void AOPAG_Player::LookAtEnemy()
{
	FVector StartLocation;
	FVector OutDirection;
	CalculateLookAtLocations(StartLocation, OutDirection, LookAtMaxEnemyDistance);
	TArray<AActor*> ActorsToIgnore;
	GetAllChildActors(ActorsToIgnore);
	TArray<FHitResult> OutHitResults;
	bool bResult = UKismetSystemLibrary::LineTraceMulti(GetWorld(), StartLocation, OutDirection,
		ETraceTypeQuery::TraceTypeQuery1, true,  ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);
	if (bResult && OutHitResults.Num() > 0) {
		for (int i = 0; i < OutHitResults.Num(); i++) {
			AOPAG_Enemy* Enemy = Cast<AOPAG_Enemy>(OutHitResults[i].GetActor());
			if (Enemy != nullptr) {
				Enemy->SetHealthBarVisible();
			}
		}
		return;
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

			EMS->PotionsAmountChanged.Broadcast(InventorySystemComp->PotionsAmount);
			//EMS->PotionsAmountDecrement.Broadcast();
		}
	}
}
#pragma endregion Others Input

#pragma region GET
bool AOPAG_Player::GetIsADS() const
{
	return bIsInADS;
}

bool AOPAG_Player::GetIsRunning() const
{
	return bRunning;
}

float AOPAG_Player::GetDashLenght() const
{
	return DashRestoreTime;
}

float AOPAG_Player::GetDashSpeed() const
{
	return DashSpeed;
}

#pragma endregion GET

#pragma region Timeline
void AOPAG_Player::DashTimelineFinished()
{
	DashTimeline->SetTimelineLength(AttributeSetPlayer->GetDashRestoreTime());
	AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetDashCountAttribute(), AttributeSetPlayer->GetDashCount() + 1);
	
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
	Value *= ((100 - CurrentGun->GetStatsTotal().Stability) / 100) * CurrentGun->GetRecoilStrength();

	// Add yaw input to camera with randomized value
	AddControllerYawInput(-RandomRecoilYaw);

	AddControllerPitchInput(-Value);
	AccumulatedRecoilDeviation.Pitch += Value;
	AccumulatedRecoilDeviation.Yaw += RandomRecoilYaw;
}

void AOPAG_Player::RecoilTimelineFinished()
{
	AccumulatedRecoilDeviationBeforeGoingDown = AccumulatedRecoilDeviation;
	LastTimelimeReverseRecoilValue = .0f;

	RecoilReverseTimeline->PlayFromStart();
}

void AOPAG_Player::RecoilReverseTimelineProgress(float Value)
{
	if (AccumulatedRecoilDeviation.Pitch < .0f)
	{
		RecoilReverseTimeline->Stop();
		AccumulatedRecoilDeviation = FRotator::ZeroRotator;
	}

	float DeltaTimeline = RecoilReverseTimeline->GetPlaybackPosition() / RecoilReverseTimeline->GetTimelineLength() - LastTimelimeReverseRecoilValue;
	LastTimelimeReverseRecoilValue = RecoilReverseTimeline->GetPlaybackPosition() / RecoilReverseTimeline->GetTimelineLength();

	float DeltaPitch = AccumulatedRecoilDeviationBeforeGoingDown.Pitch * DeltaTimeline;
	AddControllerPitchInput(DeltaPitch);

	float DeltaYaw = AccumulatedRecoilDeviationBeforeGoingDown.Yaw * DeltaTimeline;
	AddControllerYawInput(DeltaYaw);

	AccumulatedRecoilDeviation.Pitch -= DeltaPitch;
	AccumulatedRecoilDeviation.Yaw -= DeltaYaw;
}

void AOPAG_Player::RecoilReverseTimelineFinished()
{
	// UE_LOG(LogTemp, Warning, TEXT("Finished Accumulated:%s"), *AccumulatedRecoilDeviation.ToString());
	// Only to avoid that float errors accumulated
	AccumulatedRecoilDeviation = FRotator::ZeroRotator;
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
	Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
			EMS->ShieldChanged.Broadcast(AttributeSetPlayer->GetShieldCapacity(),
				AttributeSetPlayer->GetShieldCapacityMax(),
				bIsShieldBroken);
			return;
		}

		const float NewShieldCapicityValue = FMath::Clamp(AttributeSetPlayer->GetShieldCapacity() + AttributeSetPlayer->GetShieldRechargeRate(), 0.f, AttributeSetPlayer->GetShieldCapacityMax());
		AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetShieldCapacityAttribute(), NewShieldCapicityValue);
	
		EMS->ShieldChanged.Broadcast(AttributeSetPlayer->GetShieldCapacity(), 
			AttributeSetPlayer->GetShieldCapacityMax(),
			bIsShieldBroken);

		//UE_LOG(LogTemp, Warning, TEXT("Current Shield Capacity: %f"), AttributeSetPlayer->GetShieldCapacity());
	}
}

void AOPAG_Player::ShieldCooldownTimer()
{
	if (AttributeSetPlayer != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(RestartShieldPlayerHandle, this, &AOPAG_Player::ResetShieldTimer, 1.f, true);
		//UE_LOG(LogTemp, Warning, TEXT("Complete Shield Recharge Cooldown"));
	}
}
#pragma endregion Timer

#pragma region Player State
void AOPAG_Player::SavePlayerState()
{
	FOPAG_PlayerState NewState;

	NewState.LoadType = PlayerStateLoadType::PREVIOUS_STAGE;
	NewState.Health = GetHealth();
	NewState.Shield = AttributeSetPlayer->GetShieldCapacity();
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
		AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetShieldCapacityAttribute(), StateToApply.Shield);
		AbilitySystemComp->SetNumericAttributeBase(AttributeSetPlayer->GetDashCountAttribute(), StateToApply.DashCount);

		bIsDashAlreadyInitialized = false;
		InitDashValues();

		InventorySystemComp->GoldAmount = StateToApply.GoldAmount;
		InventorySystemComp->PotionsAmount = StateToApply.PotionsAmount;
		InventorySystemComp->BoltsAmount = StateToApply.BoltsAmount;
		bAreCountersAlreadyInitialized = false;
		InitUICounters();

		if (StateToApply.PrimaryGun.GunClass)
		{
			PrimaryGunComponent->SetChildActorClass(StateToApply.PrimaryGun.GunClass);
			if (PrimaryGunComponent->GetChildActor())
			{
				SwapGuns();

				PrimaryGun = Cast<AOPAG_ModularGun>(PrimaryGunComponent->GetChildActor());
				PrimaryGun->CreateAndAttachAllModules(StateToApply.PrimaryGun.AttachedModules, StateToApply.PrimaryGun.AttachedModuleStats);

				AttachDefaultGuns();

				PrimaryGun->SetAmmoCurrent(StateToApply.PrimaryGun.AmmoCurrent);
				PrimaryGun->SetAmmoMax(StateToApply.PrimaryGun.AmmoMax);

				if (StateToApply.bIsPrimaryActive) SwapGuns();

				EMS->CurrentAmmoChanged.Broadcast(CurrentGun->GetAmmoCurrent());
				EMS->TotalAmmoChanged.Broadcast(CurrentGun->GetAmmoMax());
			}
		}
	}
}
#pragma endregion Player State

#pragma region Animation Notify
void AOPAG_Player::AttachMagazineToHandNotify()
{
	if (PrimaryGun == nullptr)	return;

	PrimaryGun->GetModuleBySocketType(EModuleTypes::Magazine)->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketHandLeft);
}

void AOPAG_Player::AttachMagazineToGunNotify()
{
	if (PrimaryGun == nullptr || PrimaryGun->GetModuleBySocketType(EModuleTypes::Magazine) == nullptr)	return;

	PrimaryGun->GetModuleBySocketType(EModuleTypes::Magazine)->AttachToComponent(PrimaryGun->GetReceiverMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Magazine");
}

void AOPAG_Player::CompleteInspectNotify()
{
	bInspecting = false;
}

void AOPAG_Player::FootstepsMakeNoiseNotify()
{
	MakeNoise(FootstepsAILoudness, this, this->GetActorLocation(), 0);
}
#pragma endregion Animation Notify

#pragma region AbilitySystemUtilities
bool AOPAG_Player::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComp && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComp->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}
#pragma endregion AbilitySystemUtilities

void AOPAG_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOPAG_Drop* object = Cast<AOPAG_Drop>(OtherActor);
	if (object) {
		InventorySystemComp->AnalyzeItem(object);
	}
}