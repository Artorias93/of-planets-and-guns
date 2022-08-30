#include "Interactables/OPAG_LootBox.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include <Components/PrimitiveComponent.h>
#include "Interactables/OPAG_InteractableInterface.h"

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

void AOPAG_LootBox::BeginPlay()
{
	Super::BeginPlay();
	
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
	bIsInteractable = true;
}

void AOPAG_LootBox::SetModules(FOPAG_GunModules& ModulesStruct)
{
	Modules = ModulesStruct;
}

void AOPAG_LootBox::Interact_Implementation()
{
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

	// If the item is gun, that gun is generate with random modules
	if (AOPAG_ModularGun* GunModular = Cast<AOPAG_ModularGun>(Item))
	{
		GunModular->CreateAndAttachAllModules(Modules);
	}

	// Apply force for any items
	IOPAG_InteractableInterface* TheInterface = Cast<IOPAG_InteractableInterface>(Item);
	if (Item->GetClass()->ImplementsInterface(UOPAG_InteractableInterface::StaticClass()))
	{
		const FVector Force = Item->GetActorUpVector() * ThrowItems + Item->GetActorForwardVector() * ThrowItems;
		IOPAG_InteractableInterface::Execute_ApplyForce(Item, Force);
	}

	// Re-call this method for any items
	GetWorld()->GetTimerManager().SetTimer(SpawnItemsHandle, this, &AOPAG_LootBox::SpawnItem, 1.2f, false);
}