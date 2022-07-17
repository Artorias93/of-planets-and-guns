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
}

void AOPAG_Player::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilities();
	InitDefaultValues();
	InitADSTimeline();
	AttachDefaultGuns();
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
		OnADSTimelineFinished.BindUFunction(this, FName("ADSTimelineProgress"));

		ADSTimeline->AddInterpFloat(ADSCurveFloat, OnADSTimelineProgress);
		ADSTimeline->SetTimelineFinishedFunc(OnADSTimelineFinished);
	}
}

void AOPAG_Player::InitDashTimeline()
{
	if (ADSCurveFloat)
	{
		DashTimeline->SetLooping(false);
		DashTimeline->SetTimelineLength(0);

		OnDashTimelineFinished.BindUFunction(this, FName("DashTimelineFinished"));
		DashTimeline->SetTimelineFinishedFunc(OnADSTimelineFinished);
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
	/*const bool CheckIsReloading = CurrentGun->bIsReloading;
	if (CheckIsReloading)
	{
		CurrentGun->Reload();
	}*/
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
		// CurrentGun->Shoot(StartLocation, Direction);
		Mesh1P->GetAnimInstance()->Montage_Play(FireMontageToPlay, 1.f, EMontagePlayReturnType::MontageLength, 0, true);
	}
}

// Check if guns can shoot (control if gun is shooting, if player isn't reloading gun and gun has enough ammo)
const bool AOPAG_Player::CheckCanShoot() const
{
	//const bool bCanShoot = CurrentGun->bCanShoot && !CurrentGun->bReloading && CurrentGun->AmmoCurent > 0;
	//return bCanShoot;
	return true;
}

// Override the default method gestshootingdirection 
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
	bIsInADS = true;
	//const AOPAG_SightBase* Sight = Cast<AOPAG_SightBase>(CurrentGun->Sigh)
	if (!bIsPreADS)
	{
		GunPreADSRoll = CurrentGun->GetRootComponent()->GetComponentRotation().Pitch;
		bIsPreADS = true;
	}
	ADSTimeline->Play();
}

void AOPAG_Player::ResetFPSCamera(const FInputActionValue& Value)
{
	bIsInADS = false;
	CurrentCamera = Camera;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(this, .2f, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
	ADSTimeline->Reverse();
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
	/*const int EquippableType = 0;	//TODO: change with module equippableType
	const int FireType = CurrentGun->Type;
	const int FireMask = 1 << FireType;
	if (FireMask & EquippableType != 0)
	{
		// TODO: PickupModule
	}*/
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

#pragma region Timeline
void AOPAG_Player::ADSTimelineProgress(float Value)
{
	UAnimInstance* AnimationInstance = Mesh1P->GetAnimInstance();
	// TODO: problem with animation first person blueprint -> WRITE TO CPP
	const FRotator GunRotation = CurrentGun->GetRootComponent()->GetComponentRotation();
	const float BlendLerp = UKismetMathLibrary::Lerp(GunPreADSRoll, 0, Value);
	const FRotator BlendRotation(GunRotation.Roll, BlendLerp, GunRotation.Yaw);
	SetActorRotation(BlendRotation);
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
#pragma endregion Timeline