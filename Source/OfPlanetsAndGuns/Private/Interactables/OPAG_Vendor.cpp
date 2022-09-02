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
#include <Components/TextRenderComponent.h>
#include <Components/ChildActorComponent.h>
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "../../Public/Managers/OPAG_GameInstance.h"

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

	PriceTextRender_Gun1 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PriceTextRender_Gun1"));
	if (PriceTextRender_Gun1)
	{
		PriceTextRender_Gun1->SetVisibility(false);
		PriceTextRender_Gun1->SetupAttachment(RootComponent);
	}

	PriceTextRender_Gun2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PriceTextRender_Gun2"));
	if (PriceTextRender_Gun2)
	{
		PriceTextRender_Gun2->SetVisibility(false);
		PriceTextRender_Gun2->SetupAttachment(RootComponent);
	}

	PriceTextRender_Module1 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PriceTextRender_Module1"));
	if (PriceTextRender_Module1)
	{
		PriceTextRender_Module1->SetVisibility(false);
		PriceTextRender_Module1->SetupAttachment(RootComponent);
	}

	PriceTextRender_Module2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PriceTextRender_Module2"));
	if (PriceTextRender_Module2)
	{
		PriceTextRender_Module2->SetVisibility(false);
		PriceTextRender_Module2->SetupAttachment(RootComponent);
	}

	ChildActor_Gun1 = CreateDefaultSubobject<UChildActorComponent>("LootSpawnPoint_Gun1");
	if (ChildActor_Gun1)
		ChildActor_Gun1->SetupAttachment(RootComponent);
	ChildActor_Gun2 = CreateDefaultSubobject<UChildActorComponent>("LootSpawnPoint_Gun2");
	if (ChildActor_Gun2)
		ChildActor_Gun2->SetupAttachment(RootComponent);
	ChildActor_Module1 = CreateDefaultSubobject<UChildActorComponent>("LootSpawnPoint_Module1");
	if (ChildActor_Module1)
		ChildActor_Module1->SetupAttachment(RootComponent);
	ChildActor_Module2 = CreateDefaultSubobject<UChildActorComponent>("LootSpawnPoint_Module2");
	if (ChildActor_Module2)
		ChildActor_Module2->SetupAttachment(RootComponent);
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

	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);
	
	EMS->DisableGunTextInVendor.AddDynamic(this, &AOPAG_Vendor::DisableGunTextInVendor);
	EMS->DisableModuleTextInVendor.AddDynamic(this, &AOPAG_Vendor::DisableModuleTextInVendor);

}

void AOPAG_Vendor::Unlock() {
	PopulateGunAndModuleArray();
	SpawnItem(666);
}

// Called every frame
void AOPAG_Vendor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	TArray<UChildActorComponent*> ChildActorGuns;
	ChildActorGuns.Add(ChildActor_Gun1);
	ChildActorGuns.Add(ChildActor_Gun2);

	TArray<FVector> TempGunsSpawnLocation;
	TempGunsSpawnLocation.Add(ChildActor_Gun1->GetComponentLocation());
	TempGunsSpawnLocation.Add(ChildActor_Gun2->GetComponentLocation());

	TArray<UTextRenderComponent*> TempGunsTextRender;
	TempGunsTextRender.Add(PriceTextRender_Gun1);
	TempGunsTextRender.Add(PriceTextRender_Gun2);

	for (int i = 0; i < 2; i++) {
		const float RandomGunChance = FMath::RandRange(0.f, 100.f);
		float Sum = 0.f;

		UOPAG_UtilityFunctionLibrary::ShuffleArray(GunsList);

		for (FOPAG_GunList* Gun : GunsList)
		{
			Sum += Gun->GunChance;
			if (RandomGunChance <= Sum)
			{			
				ChildActorGuns[i]->SetChildActorClass(Gun->GunClass);

				AOPAG_ModularGun* NewGun = Cast<AOPAG_ModularGun>(ChildActorGuns[i]->GetChildActor());
				TempGunsTextRender[i]->SetVisibility(true);
				IOPAG_InteractableInterface::Execute_ApplyFly(NewGun);

				TempGunsList.Add(NewGun);

				GunsList.Remove(Gun); // to avoid two guns of same type
				break;
			}
		}
	}
	if (TempGunsList.Num() != 2) {
		ChildActorGuns[1]->SetChildActorClass(GunsList[0]->GunClass);

		AOPAG_ModularGun* NewGun = Cast<AOPAG_ModularGun>(ChildActorGuns[1]->GetChildActor());
		TempGunsTextRender[1]->SetVisibility(true);
		IOPAG_InteractableInterface::Execute_ApplyFly(NewGun);

		TempGunsList.Add(NewGun);
	}

	// Attach modules to guns
	AttachModulesToGun(*TempGunsList[0], false, RoomDifficulty);
	AttachModulesToGun(*TempGunsList[1], true, RoomDifficulty);

	// Set price to texts
	TempGunsTextRender[0]->SetText(FString::SanitizeFloat(IOPAG_InteractableInterface::Execute_GetPriceTotal(TempGunsList[0])));
	TempGunsTextRender[1]->SetText(FString::SanitizeFloat(IOPAG_InteractableInterface::Execute_GetPriceTotal(TempGunsList[1])));

	// Spawn two modules
	if (ModulesList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VENDOR: Empty ModulesList as input to SpawnItem."));
		return;
	}

	// Init modules list to spawn
	TArray<TSubclassOf<AActor>> ModulesToSpawnList;

	TArray<UChildActorComponent*> ChildActorModules;
	ChildActorModules.Add(ChildActor_Module1);
	ChildActorModules.Add(ChildActor_Module2);
	
	// Init modules spawn locations
	TArray<FVector> TempModulesSpawnLocation;
	TempModulesSpawnLocation.Add(ChildActor_Module1->GetComponentLocation());
	TempModulesSpawnLocation.Add(ChildActor_Module2->GetComponentLocation());

	TArray<UTextRenderComponent*> TempModulesTextRender;
	TempModulesTextRender.Add(PriceTextRender_Module1);
	TempModulesTextRender.Add(PriceTextRender_Module2);

	// Init player for check fire type gun
	AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr)	return;

	// Init fire type
	const int FireMask = 1 << (int)Player->GetPrimaryGun()->GetFireType();

	// Spawn two modules
	for (int i = 0; i < 2; i++)
	{
		// Shuffle modules list
		UOPAG_UtilityFunctionLibrary::ShuffleArray(ModulesList);

		// Select random module
		TSubclassOf<AActor> NewModuleClass = UOPAG_LootManagerComponent::SelectRandomModule(ModulesList, RoomDifficulty, FireMask);
		ChildActorModules[i]->SetChildActorClass(NewModuleClass);

		AOPAG_ModuleBase* NewModule = Cast<AOPAG_ModuleBase>(ChildActorModules[i]->GetChildActor());
		TempModulesTextRender[i]->SetVisibility(true);
		TempModulesTextRender[i]->SetText(FString::SanitizeFloat(IOPAG_InteractableInterface::Execute_GetPriceTotal(NewModule)));
		IOPAG_InteractableInterface::Execute_ApplyFly(NewModule);
	}
}

void AOPAG_Vendor::AttachModulesToGun(AOPAG_ModularGun& Gun, const bool IsPowerful, const float DifficultyValue)
{
	int MinModules = IsPowerful ? 2 : 0;
	int MaxModules = IsPowerful ? 6 : 3;

	// Init gun's fire type (util for pick random modules)
	const int FireMask = 1 << (int)Gun.GetFireType();
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
		TSubclassOf<AActor> Module = UOPAG_LootManagerComponent::SelectRandomModule(ModulesList, DifficultyValue, FireMask, ModuleTypes[RandomModuleIndex]);
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

void AOPAG_Vendor::DisableGunTextInVendor(AOPAG_ModularGun* Gun)
{
	if (ChildActor_Gun1->GetChildActor() == Gun)
	{
		PriceTextRender_Gun1->SetVisibility(false);
	}
	else if (ChildActor_Gun2->GetChildActor() == Gun)
	{
		PriceTextRender_Gun2->SetVisibility(false);
	}
}

void AOPAG_Vendor::DisableModuleTextInVendor(AOPAG_ModuleBase* Module)
{
	if (ChildActor_Module1->GetChildActor() == Module)
	{
		PriceTextRender_Module1->SetVisibility(false);
	}
	else if (ChildActor_Module2->GetChildActor() == Module)
	{
		PriceTextRender_Module2->SetVisibility(false);
	}
}