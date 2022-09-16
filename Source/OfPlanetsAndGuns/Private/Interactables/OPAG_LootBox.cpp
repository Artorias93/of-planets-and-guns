#include "Interactables/OPAG_LootBox.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include <Components/PrimitiveComponent.h>

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
//#include <NiagaraSystem.h>

#include "Characters/Player/OPAG_Player.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"


AOPAG_LootBox::AOPAG_LootBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Crate = CreateDefaultSubobject<UStaticMeshComponent>("Crate");
	if (Crate)
		RootComponent = Crate;

	Lid = CreateDefaultSubobject<UStaticMeshComponent>("Lid");
	if (Lid)
		Lid->SetupAttachment(RootComponent);
		

	LootSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>("LootSpawnPoint");
	if (LootSpawnPoint)
		LootSpawnPoint->SetupAttachment(RootComponent);

	DropTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DropTimeline"));
}

bool AOPAG_LootBox::WillChangeInteractStateDynamically_Implementation()
{
	return true;
}

FString AOPAG_LootBox::InteractString_Implementation()
{
	if (bHasBeenOpened)
		return "";
	else if(!bIsInteractable)
		return "Locked";
	else 
		return "Open Lootbox [E]";
}

void AOPAG_LootBox::DropTimelineProgress(FVector Value)
{
	FVector NewLocation = LootSpawnPoint->GetComponentLocation();
	NewLocation.X += RandomDropDir.X * Value.X * DropCurveXFactor;
	NewLocation.Y += RandomDropDir.Y * Value.X * DropCurveXFactor;
	NewLocation.Z += Value.Z * DropCurveZFactor;

	SpawnedItem->SetActorLocation(NewLocation);
}

void AOPAG_LootBox::BeginPlay()
{
	Super::BeginPlay();
	SMS = GetGameInstance<UOPAG_GameInstance>()->GetSoundManager();
	
	DropTimeline->SetLooping(false);
	DropTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	OnDropTimelineProgress.BindUFunction(this, FName("DropTimelineProgress"));
	DropTimeline->AddInterpVector(CurveDropVector, OnDropTimelineProgress, FName("DropTimelinePropertyName"));
}

void AOPAG_LootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_LootBox::SetItemsToSpawn(TArray<TSubclassOf<AActor>>& ItemsList)
{
	Items = ItemsList;
}

void AOPAG_LootBox::Unlock()
{
	ActivationParticleEffect();
	bIsInteractable = true;
}

void AOPAG_LootBox::SetModules(FOPAG_GunModules& ModulesStruct)
{
	Modules = ModulesStruct;
}

void AOPAG_LootBox::ActivationParticleEffect()
{
	if (Crate == nullptr) return;
	FVector EffectLocation = Crate->GetComponentLocation();
	if (ActivationParticles != nullptr) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ActivationParticles, EffectLocation);// FRotator::MakeFromEuler(FVector(0, 0, 1)), FVector(50.0f));
	}
	
}

void AOPAG_LootBox::Interact_Implementation()
{
	// particle effect
	ActivationParticleEffect();
	// Explosion behavior with half parabolic method
	Lid->SetSimulatePhysics(true);
	FVector RandomDir = Lid->GetUpVector();
	float RandomAngle = FMath::DegreesToRadians(FMath::RandRange(0, 360));
	RandomDir.X = FMath::Sin(RandomAngle);
	RandomDir.Y = FMath::Cos(RandomAngle);

	const FVector Force = RandomDir * ThrowLid;
	Lid->AddForce(Force, NAME_None, true);
	Lid->AddTorqueInRadians(RandomDir * 100, NAME_None, true);
	// Set can interact to false after player interact with box
	bIsInteractable = false;

	bHasBeenOpened = true;


	if(SMS)
		SMS->PlayEnviromentSounds(this, EEnviromentSoundAction::Chest_Open);

	// Active Timer for spawn items
	SpawnItem();
}

bool AOPAG_LootBox::CanInteract_Implementation()
{
	return bIsInteractable;
}

// Call method for any item in the list
void AOPAG_LootBox::SpawnItem()
{
	if (Items.Num() == 0)
	{
		return;
	}

	// Spawn Item
	const FVector SpawnLocation = LootSpawnPoint->GetComponentLocation();//RootComponent->GetComponentLocation();
	const FRotator SpawnRotator = FRotator::ZeroRotator;
	AActor* Item = GetWorld()->SpawnActor(Items[0], &SpawnLocation, &SpawnRotator);
	Items.RemoveAt(0);

	if (Item)
		SpawnedItem = Item;

	// If the item is gun, that gun is generate with random modules
	if (AOPAG_ModularGun* GunModular = Cast<AOPAG_ModularGun>(Item))
	{
		GunModular->CreateAndAttachAllModules(Modules);
	}
	// If the item is a module, apply bonus stats
	if (AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(Item))
	{
		const AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Player != nullptr)
		{
			const AOPAG_ModularGun* PrimaryGun = Player->GetPrimaryGun();
			if (PrimaryGun != nullptr)
			{
				Module->SetStats(PrimaryGun->GetSecondaryStatsModifiers());
			}
		}
	}

	// Apply force for any items
	if (Item && Item->GetClass()->ImplementsInterface(UOPAG_InteractableInterface::StaticClass()))
	{
		float RandomAngle = FMath::DegreesToRadians(FMath::RandRange(0, 360));
		RandomDropDir.X = FMath::Sin(RandomAngle);
		RandomDropDir.Y = FMath::Cos(RandomAngle);

		DropTimeline->PlayFromStart();	
	}

	// Re-call this method for any items
	GetWorld()->GetTimerManager().SetTimer(SpawnItemsHandle, this, &AOPAG_LootBox::SpawnItem, 0.5, false);
}