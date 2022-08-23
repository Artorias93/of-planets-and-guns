#include "Interactables/OPAG_Vendor.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include <Components/PrimitiveComponent.h>
#include "Interactables/OPAG_InteractableInterface.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Managers/RoomManager/OPAG_LootManagerComponent.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>

// Sets default values
AOPAG_Vendor::AOPAG_Vendor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	if (BoxCollision != nullptr)
	{
		BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		BoxCollision->SetBoxExtent(FVector(32.f, 200.f, 32.f));
		RootComponent = BoxCollision;
	}

	Pedestal = CreateDefaultSubobject<UStaticMeshComponent>("Pedestal");
	if (Pedestal != nullptr)
	{
		Pedestal->SetCollisionProfileName(TEXT("BlockAll"));
		Pedestal->SetWorldLocation(FVector(.0f, -60.f, -30.f));
		Pedestal->SetWorldScale3D(FVector(1.f, 3.f, 1.5f));
		Pedestal->SetupAttachment(RootComponent);
	}

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


void AOPAG_Vendor::Interact_Implementation()
{
	// Interacting with Vendor spawns merch. Set to false to avoid respawn.
	bIsInteractable = bIsInfinitelyInteractable;

	PopulateGunAndModuleArray();

	// Active Timer for spawn items
	SpawnItem(666);
}

bool AOPAG_Vendor::CanInteract_Implementation()
{
	return bIsInteractable;
}

// Call method for spawning all guns
void AOPAG_Vendor::SpawnItem(const float RoomDifficulty)
{	
	const FRotator SpawnRotator = FRotator::ZeroRotator;

	if (GunsList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VENDOR: Empty GunList as input to SpawnItem."));
		return;
	}

	TArray<AOPAG_ModularGun*> TempGunsList;
	TArray<FVector> TempGunsSpawnLocation;
	TempGunsSpawnLocation.Add(MerchSpawnPoint_Gun1->GetComponentLocation());
	TempGunsSpawnLocation.Add(MerchSpawnPoint_Gun2->GetComponentLocation());

	for (int i = 0; i < 2; i++) {
		const float RandomGunChance = FMath::RandRange(0.f, 100.f);
		float Sum = 0.f;

		UOPAG_UtilityFunctionLibrary::ShuffleArray(GunsList);

		for (FOPAG_GunList* Gun : GunsList)
		{
			Sum += Gun->GunChance;
			if (RandomGunChance <= Sum)
			{			
				AOPAG_ModularGun* NewGun = Cast<AOPAG_ModularGun>(GetWorld()->SpawnActor(Gun->GunClass, &TempGunsSpawnLocation[i], &SpawnRotator));
				NewGun->ApplyFly_Implementation();
				TempGunsList.Add(NewGun);

				GunsList.Remove(Gun); // to avoid two guns of same type
				break;
			}
		}
	}
	if (TempGunsList.Num() != 2) {
		AOPAG_ModularGun* NewGun = Cast<AOPAG_ModularGun>(GetWorld()->SpawnActor(GunsList[0]->GunClass, &TempGunsSpawnLocation[1], &SpawnRotator));
		NewGun->ApplyFly_Implementation();
		TempGunsList.Add(NewGun);
	}
	AttachModulesToGun(*TempGunsList[0], false, RoomDifficulty );
	AttachModulesToGun(*TempGunsList[1], true, RoomDifficulty);

	// Spawn two modules
	if (ModulesList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VENDOR: Empty ModulesList as input to SpawnItem."));
		return;
	}

	// Init modules list to spawn
	TArray<TSubclassOf<AActor>> ModulesToSpawnList;
	
	// Init modules spawn locations
	TArray<FVector> TempModulesSpawnLocation;
	TempModulesSpawnLocation.Add(MerchSpawnPoint_Module1->GetComponentLocation());
	TempModulesSpawnLocation.Add(MerchSpawnPoint_Module2->GetComponentLocation());

	// Init player for check fire type gun
	AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr)	return;

	// Init fire type
	const int FireMask = 1 << (int)Player->PrimaryGun->FireType;

	// Spawn two modules
	for (int i = 0; i < 2; i++)
	{
		// Shuffle modules list
		UOPAG_UtilityFunctionLibrary::ShuffleArray(ModulesList);

		// Select random module
		TSubclassOf<AActor> NewModuleClass = UOPAG_LootManagerComponent::SelectRandomModules(ModulesList, RoomDifficulty, FireMask);
		AOPAG_ModuleBase* NewModule = Cast<AOPAG_ModuleBase>(GetWorld()->SpawnActor(NewModuleClass, &TempModulesSpawnLocation[i], &SpawnRotator));
		NewModule->ApplyFly_Implementation();
	}
}

void AOPAG_Vendor::AttachModulesToGun(AOPAG_ModularGun& Gun, const bool IsPowerful, const float DifficultyValue)
{
	int MinModules = IsPowerful ? 2 : 0;
	int MaxModules = IsPowerful ? 6 : 3;

	// Init gun's fire type (util for pick random modules)
	const int FireMask = 1 << (int)Gun.FireType;
	const int RandomNumberOfModules = FMath::RandRange(MinModules, MaxModules);

	// Create array of modules type
	TArray<EModuleTypes> ModuleTypes;
	ModuleTypes.Add(EModuleTypes::Barrel);
	ModuleTypes.Add(EModuleTypes::Stock);
	ModuleTypes.Add(EModuleTypes::Grip);
	ModuleTypes.Add(EModuleTypes::Magazine);
	ModuleTypes.Add(EModuleTypes::Sight);
	ModuleTypes.Add(EModuleTypes::Accessory);

	// Select random modules to attach 
	FOPAG_GunModules ModulesStruct = Gun.GetStandardModulesStruct();
	int Count = 0;
	while (Count < RandomNumberOfModules && ModuleTypes.Num() > 0)
	{
		const int RandomModuleIndex = FMath::RandRange(0, ModuleTypes.Num() - 1);
		TSubclassOf<AActor> Module = UOPAG_LootManagerComponent::SelectRandomModules(ModulesList, DifficultyValue, FireMask, ModuleTypes[RandomModuleIndex]);
		if (Module != nullptr)
		{
			switch (ModuleTypes[RandomModuleIndex])
			{
			case EModuleTypes::Barrel:
				ModulesStruct.Barrel = Module;
				break;
			case EModuleTypes::Stock:
				ModulesStruct.Stock = Module;
				break;
			case EModuleTypes::Grip:
				ModulesStruct.Grip = Module;
				break;
			case EModuleTypes::Magazine:
				ModulesStruct.Magazine = Module;
				break;
			case EModuleTypes::Sight:
				ModulesStruct.Sight = Module;
				break;
			case EModuleTypes::Accessory:
				ModulesStruct.Accessory = Module;
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Module types error!"));
				break;
			}

			Count++;
		}

		// Remove the select index for no-repeat the same or not fiend module
		ModuleTypes.RemoveAt(RandomModuleIndex);
	}

	Gun.CreateAndAttachAllModules(ModulesStruct);
}