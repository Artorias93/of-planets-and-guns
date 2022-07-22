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
#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"

AOPAG_Player::AOPAG_Player()
{
	PrimaryActorTick.bCanEverTick = true;

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
	
	if (BaseAttributeSet && AbilitySystemComp)
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &AOPAG_Player::HealthChanged);
	}

	InitializeAbilities();
	InitDefaultValues();
	InitADSTimeline();
	InitRecoilTimeline();
	AttachDefaultGuns();

	EMS->TotalAmmoChanged.Broadcast(CurrentGun->AmmoMax);
	EMS->CurrentAmmoChanged.Broadcast(CurrentGun->AmmoCurrent);
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
}

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
		PrimarytGun = CurrentGun;
	}
	if (Camera)
	{
		CurrentCamera = Camera;
	}
}

void AOPAG_Player::InitADSTimeline()
{
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
	if (ADSCurveFloat)
	{
		DashTimeline->SetLooping(false);
		DashTimeline->SetTimelineLength(0);

		OnDashTimelineFinished.BindUFunction(this, FName("DashTimelineFinished"));
		DashTimeline->SetTimelineFinishedFunc(OnDashTimelineFinished);
	}
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

void AOPAG_Player::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NameCurrentHealth = GET_MEMBER_NAME_CHECKED(AOPAG_Player, CurrentHealth);
	static const FName NameCurrentHealthMax = GET_MEMBER_NAME_CHECKED(AOPAG_Player, CurrentHealthMax);

	if (PropertyChangedEvent.Property)
	{
		const FName PropName = PropertyChangedEvent.MemberProperty->GetFName();
		if (PropName == NameCurrentHealth)
		{
			SetHealth(CurrentHealth);
		}
		if (PropName == NameCurrentHealthMax && BaseAttributeSet)
		{
			SetHealthMax(CurrentHealthMax);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#pragma region MathematicUtility
const FVector AOPAG_Player::CalculateThrowForce() const
{
	const FVector ForwardWithForce = AActor::GetActorForwardVector() * TossForce;
	const FVector UpWithForce = AActor::GetActorUpVector() * TossForce;
	const FVector Force = ForwardWithForce + UpWithForce;
	return Force;
}
#pragma endregion MathematicUtility

#pragma region EnhanchedInput
void AOPAG_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EIC);

	// All binding events for player's ability
	EIC->BindAction(InputActions.SwapGun, ETriggerEvent::Started, this, &AOPAG_Player::SwapGuns);
	EIC->BindAction(InputActions.ReloadGun, ETriggerEvent::Started, this, &AOPAG_Player::ReloadGun);
	EIC->BindAction(InputActions.Dash, ETriggerEvent::Triggered, this, &AOPAG_Player::ActiveDash);
	EIC->BindAction(InputActions.GunAbility, ETriggerEvent::Triggered, this, &AOPAG_Player::ActiveGunAbility);

	// All binding events for player's movements
	EIC->BindAction(InputActions.Move, ETriggerEvent::Triggered, this, &AOPAG_Player::MoveForward);
	EIC->BindAction(InputActions.Move, ETriggerEvent::Triggered, this, &AOPAG_Player::MoveRight);
	EIC->BindAction(InputActions.CameraMove, ETriggerEvent::Triggered, this, &AOPAG_Player::Lookup);
	EIC->BindAction(InputActions.CameraMove, ETriggerEvent::Triggered, this, &AOPAG_Player::Turn);
	EIC->BindAction(InputActions.Jump, ETriggerEvent::Triggered, this, &AOPAG_Player::Jumping);
	EIC->BindAction(InputActions.Jump, ETriggerEvent::Canceled, this, &AOPAG_Player::StopJumping);

	// All binding events for shoot
	EIC->BindAction(InputActions.Shoot, ETriggerEvent::Started, this, &AOPAG_Player::Shoot);
	EIC->BindAction(InputActions.Shoot, ETriggerEvent::Triggered, this, &AOPAG_Player::Shoot);

	// Binding event for interact with objects
	EIC->BindAction(InputActions.Interact, ETriggerEvent::Started, this, &AOPAG_Player::InteractWithItem);

	// Binding event for interact with camera ads
	EIC->BindAction(InputActions.ADS, ETriggerEvent::Started, this, &AOPAG_Player::SwitchToADSCamera);
	EIC->BindAction(InputActions.ADS, ETriggerEvent::Completed, this, &AOPAG_Player::ResetFPSCamera);

	// binding event for health 
	EIC->BindAction(InputActions.ADS, ETriggerEvent::Triggered, this, &AOPAG_Player::Health);
}
#pragma endregion EnhanchedInput

#pragma region Ability
void AOPAG_Player::InitializeAbilities()
{
	// Initialize and active stamina ability for health behavior
	InitializeAbility(Stamina, 0);
	AbilitySystemComp->TryActivateAbilityByClass(Stamina, true);

	// Initialize grenade ability (player launch the grenade with appropriate accessory)
	InitializeAbility(Grenade, 0);

	// Initialize dash ability
	InitializeAbility(Dash, 0);

	// This ability allow you to cure the player
	InitializeAbility(HealthPotion, 0);
}

void AOPAG_Player::ActiveDash(const FInputActionValue& Value)
{
	if (AbilitySystemComp && DashTimeline && DashCount > 0)
	{
		const bool bSuccess = AbilitySystemComp->TryActivateAbilityByClass(Dash, true);
		if (bSuccess)
		{
			DashCount--;
			if (bIsFirstDash)
			{
				bIsFirstDash = false;
				DashTimeline->PlayFromStart();
			}
		}
	}
}

void AOPAG_Player::ActiveGunAbility(const FInputActionValue& Value)
{
	if (AbilitySystemComp && CurrentGun == PrimarytGun && CurrentGun->Accessory->GetChildActor())
	{
		AOPAG_AccessoryBase* Module = Cast<AOPAG_AccessoryBase>(CurrentGun->Accessory->GetChildActor());
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
		AOPAG_GunBase* PrimaryGun = Cast<AOPAG_GunBase>(PrimaryGunComponent->GetChildActor());
		AOPAG_GunBase* SecondaryGun = Cast<AOPAG_GunBase>(SecondaryGunComponent->GetChildActor());
		if (CurrentGun == PrimaryGun)
		{
			PrimaryGunComponent->GetChildActor()->SetActorHiddenInGame(false);
			SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
			CurrentGun = SecondaryGun;
		}
		else
		{
			PrimaryGunComponent->GetChildActor()->SetActorHiddenInGame(true);
			SecondaryGunComponent->GetChildActor()->SetActorHiddenInGame(false);
			CurrentGun = PrimaryGun;
		}
	}
}

void AOPAG_Player::ReloadGun()
{
	const bool CheckIsReloading = CurrentGun->bIsReloading;
	if (!CheckIsReloading)
	{
		CurrentGun->Reload();
	}
}
#pragma endregion Guns

#pragma region Shooting Behavior
// This method is link with input fire 
// Calculate standard direction for shoot and invoke shoot behavior with gun and star fire's animation
void AOPAG_Player::Shoot(const FInputActionValue& Value)
{
	if (CheckCanShoot())
	{
		FVector StartLocation;
		FVector Direction;
		GetShootingDirection(StartLocation, Direction);
		CurrentGun->Shoot(StartLocation, Direction);
		Mesh1P->GetAnimInstance()->Montage_Play(FireMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
		RecoilTimeline->PlayFromStart();

		EMS->TotalAmmoChanged.Broadcast(CurrentGun->AmmoMax);
		EMS->CurrentAmmoChanged.Broadcast(CurrentGun->AmmoCurrent);
	}
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
	const float MoveForwardScale = Value[1];
	if (MoveForwardScale != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), MoveForwardScale);
	}
}

void AOPAG_Player::MoveRight(const FInputActionValue& Value)
{
	const float MoveRightScale = Value[0];
	if (MoveRightScale != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), MoveRightScale);
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

void AOPAG_Player::Jumping(const FInputActionValue& Value)
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

void AOPAG_Player::SwitchToADSCamera(const FInputActionValue& Value)
{
	// Set ADS and initialize new camera view
	AActor* NewViewTarget = nullptr;
	bIsInADS = true;
	if (CurrentGun->Sight->GetChildActor())
	{
		NewViewTarget = CurrentGun->Sight->GetChildActor();
		CurrentCamera = CurrentGun->GetSightCamera();
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

void AOPAG_Player::ResetFPSCamera(const FInputActionValue& Value)
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
void AOPAG_Player::InteractWithItem(const FInputActionValue& Value)
{
	if (InteractableObject)
	{
		if (AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(InteractableObject))
		{
			InteractWithModule(Module);
		}
		// TODO: Potion check
		else
		{
			PotionCount++;
			InteractableObject->Destroy();
		}
	}
}

void AOPAG_Player::InteractWithModule(AOPAG_ModuleBase* Module)
{
	const int EquippableType = Module->EquippableFireTypes;
	const int FireType = (int)CurrentGun->Type;
	const int FireMask = 1 << FireType;
	if (FireMask & EquippableType)
	{
		PickupModule(Module);
	}
}

void AOPAG_Player::PickupModule(AOPAG_ModuleBase* Module)
{
	if (PrimarytGun)
	{
		// Initialize new module params
		const FGunStats NewModuleStats = Module->Stats;
		const EModuleRarity NewModuleRarity = Module->Rarity;
		const int NewModuleEquippable = Module->EquippableFireTypes;
		const TSubclassOf<AOPAG_ModuleBase> NewModuleClass = Module->GetClass();

		// Create old module params
		FGunStats OldModuleStats;
		EModuleRarity OldModuleRarity;
		int OldModuleEquippable;
		TSubclassOf<AOPAG_ModuleBase> OldModuleClass;
		
		const bool bResult = PrimarytGun->AddNewModule(NewModuleClass, NewModuleRarity, NewModuleStats, NewModuleEquippable, OldModuleClass, OldModuleRarity, OldModuleStats, OldModuleEquippable);
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

				// Throw the old module
				UPrimitiveComponent* SphereComponent = OldModule->GetSphereComponent();
				ThrowScrap(SphereComponent);
			}
		}
	}
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
	bool bHasHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), CameraStartLocation, CameraDirection, LookUpCapsuleRadius, 0, InteractableTypesQuery, 
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.f);
	if (bHasHit && OutHits.Num() > 0)
	{
		// TODO: change control tag with interface check [OutHits[0].GetActor()->GetClass()->ImplementsInterface(UMyInterface::StaticClass())]
		const bool bActorHasTag = OutHits[0].GetActor()->ActorHasTag("Interactable");
		if (bActorHasTag)
		{	
			return OutHits[0].GetActor();
		}
	}
	return nullptr;
}

void AOPAG_Player::ThrowScrap(UPrimitiveComponent* Target)
{
	const FVector Force = CalculateThrowForce();
	Target->SetSimulatePhysics(true);
	Target->AddImpulse(Force, NAME_None, false);
}
#pragma endregion LookAtInteractable

#pragma region Others Input
void AOPAG_Player::Health(const FInputActionValue& Value)
{
	if (PotionCount > 0 && AbilitySystemComp)
	{
		const bool bSuccess = AbilitySystemComp->TryActivateAbilityByClass(HealthPotion, true);
		if (bSuccess)
		{
			PotionCount--;
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
	DashTimeline->SetTimelineLength(DashRestoreTime);
	DashCount++;
	if (DashCount < DashMaxCount)
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

	UE_LOG(LogTemp, Warning, TEXT("%f"), RecoilPitch);
}

#pragma endregion Timeline

#pragma region Health

void AOPAG_Player::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("New Health: %f"), Data.NewValue);
	UE_LOG(LogTemp, Warning, TEXT("Max Health: %f"), GetHealthMax());

	EMS->HealthChanged.Broadcast(Data.NewValue, GetHealthMax());
}
#pragma endregion