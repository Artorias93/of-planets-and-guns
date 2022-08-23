#include "Interactables/OPAG_Vendor.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include <Components/PrimitiveComponent.h>
#include "Interactables/OPAG_InteractableInterface.h"

// Sets default values
AOPAG_Vendor::AOPAG_Vendor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pedestal = CreateDefaultSubobject<UStaticMeshComponent>("Pedestal");
	if (Pedestal)
		RootComponent = Pedestal;

	MerchSpawnPoint_Gun1 = CreateDefaultSubobject<USceneComponent>("LootSpawnPoint_Gun1");
	if (MerchSpawnPoint_Gun1)
		MerchSpawnPoint_Gun1->SetupAttachment(RootComponent);
	MerchSpawnPoint_Gun2 = CreateDefaultSubobject<USceneComponent>("LootSpawnPoint_Gun2");
	if (MerchSpawnPoint_Gun2)
		MerchSpawnPoint_Gun2->SetupAttachment(RootComponent);
	MerchSpawnPoint_Module1 = CreateDefaultSubobject<USceneComponent>("LootSpawnPoint_Module1");
	if (MerchSpawnPoint_Module1)
		MerchSpawnPoint_Module1->SetupAttachment(RootComponent);
	MerchSpawnPoint_Module2 = CreateDefaultSubobject<USceneComponent>("LootSpawnPoint_Module2");
	if (MerchSpawnPoint_Module2)
		MerchSpawnPoint_Module2->SetupAttachment(RootComponent);
}

void AOPAG_Vendor::PopulateGunAndModuleArray()
{
	const FString Context;
	if (DataTableModules)
	{
		DataTableModules->GetAllRows(Context, ModulesList);
	}
	if (DataTableGuns)
	{
		DataTableGuns->GetAllRows(Context, GunsList);
	}
}

// Called when the game starts or when spawned
void AOPAG_Vendor::BeginPlay()
{
	Super::BeginPlay();
	PopulateGunAndModuleArray();
	
}

// Called every frame
void AOPAG_Vendor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Vendor::Unlock()
{
	bIsInteractable = true;
}

void AOPAG_Vendor::SetGunMerchToSpawn(TArray<TSubclassOf<AActor>>& GunList)
{
	if (GunList.Num() != 2) {
		UE_LOG(LogTemp, Warning, TEXT("VENDOR: SetGunMerchToSpawn expects 2 elements!"));
		return;
	}
	GunsList = GunList;
}

void AOPAG_Vendor::SetModuleMerchToSpawn(FOPAG_GunModules& ModulesStruct)
{
	Modules = ModulesStruct;
}

void AOPAG_Vendor::Interact_Implementation()
{
	// Interacting with Vendor spawns merch. Set to false to avoid respawn.
	bIsInteractable = false;

	// Play some particle effect?

	// Active Timer for spawn items
	SpawnItem();
}

bool AOPAG_Vendor::CanInteract_Implementation()
{
	return bIsInteractable;
}

// Call method for spawning all guns
void AOPAG_Vendor::SpawnItem()
{
	
	if (GunsList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VENDOR: Empty GunList as input to SpawnItem."));
		return;
	}

	// Spawn Guns
	const FVector SpawnLocation_1 = MerchSpawnPoint_Gun1->GetComponentLocation();
	const FVector SpawnLocation_2 = MerchSpawnPoint_Gun2->GetComponentLocation();
	const FRotator SpawnRotator = FRotator::ZeroRotator;
	AActor* Gun1 = GetWorld()->SpawnActor(GunsList[0], &SpawnLocation_1, &SpawnRotator);
	AActor* Gun2 = GetWorld()->SpawnActor(GunsList[1], &SpawnLocation_2, &SpawnRotator);
	GunsList.Empty();

	// gun is generated with random modules
	if (AOPAG_ModularGun* GunModular1 = Cast<AOPAG_ModularGun>(Gun1))
	{
		GunModular1->CreateAndAttachAllModules(Modules);
	}
	if (AOPAG_ModularGun* GunModular2 = Cast<AOPAG_ModularGun>(Gun2))
	{
		GunModular2->CreateAndAttachAllModules(Modules);
	}

	// Apply constant rotation to the gun? Maybe property of all interactables?
	// Hereunder code for applying force:
	//IOPAG_InteractableInterface* TheInterface = Cast<IOPAG_InteractableInterface>(Item);
	//if (TheInterface)
	//{
	//	const FVector Force = Item->GetActorUpVector() * ThrowLid + Item->GetActorForwardVector() * ThrowLid;
	//	TheInterface->ApplyForce_Implementation(Force);
	//}

}

