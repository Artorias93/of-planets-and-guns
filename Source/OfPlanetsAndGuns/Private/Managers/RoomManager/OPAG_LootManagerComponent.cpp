#include "Managers/RoomManager/OPAG_LootManagerComponent.h"

#include "Interactables/OPAG_LootBox.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/GameplayStatics.h>
#include "Items/OPAG_Drop.h"

UOPAG_LootManagerComponent::UOPAG_LootManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UOPAG_LootManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UOPAG_LootManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UOPAG_LootManagerComponent::GenerateLoot(const float DifficultyValue)
{
	if (!LootBox)	return;

	// Populate items list 
	PopulateLootLists();

	TArray<TSubclassOf<AActor>> Items;

	// Drop variables
	for (FOPAG_DropList* Drop : Drops)
	{
		for (FOPAG_LootChance* Loot : LootChance)
		{
			if (Loot->LootType == Drop->DropType)
			{
				if (Loot->LootChance == 100.f)
				{
					Items.Add(Drop->DropClass);
					break;
				}
				else
				{
					const float RandomDropChance = FMath::RandRange(0.f, 100.f);
					if (Loot->LootChance >= RandomDropChance)
					{
						Items.Add(Drop->DropClass);
						break;
					}
				}
			}
		}
	}

	// Guns Or Modules
	const float RandomGunOrModuleChance = FMath::RandRange(0.f, 100.f);
	TSubclassOf<AActor> Item = nullptr;
	if (LootChance[3]->LootChance >= RandomGunOrModuleChance)
	{
		// If is guns
		const float RandomGunChance = FMath::RandRange(0.f, 100.f);
		float Sum = 0.f;
		for (FOPAG_GunList* Gun : Guns)
		{
			Sum += Gun->GunChance;
			if (RandomGunChance < Sum)
			{
				// Init gun's fire type (util for pick random modules)
				AOPAG_ModularGun* DefaultGun = Gun->GunClass->GetDefaultObject<AOPAG_ModularGun>();
				const int FireMask = 1 << (int)DefaultGun->FireType;
				const int RandomNumberOfModules = FMath::RandRange(LootBox->MinModules, LootBox->MaxModules);

				// Create array of modules type
				TArray<EModuleTypes> ModuleTypes;
				ModuleTypes.Add(EModuleTypes::Barrel);
				ModuleTypes.Add(EModuleTypes::Stock);
				ModuleTypes.Add(EModuleTypes::Grip);
				ModuleTypes.Add(EModuleTypes::Magazine);
				ModuleTypes.Add(EModuleTypes::Sight);
				ModuleTypes.Add(EModuleTypes::Accessory);

				// Select random modules to attach 
				FOPAG_GunModules ModulesStruct = DefaultGun->GetStandardModulesStruct();
				int Count = 0;
				while (Count < RandomNumberOfModules && ModuleTypes.Num() > 0)
				{
					const int RandomModuleIndex = FMath::RandRange(0, ModuleTypes.Num() - 1);
					TSubclassOf<AActor> Module = SelectRandomModules(DifficultyValue, FireMask, ModuleTypes[RandomModuleIndex]);
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

				LootBox->SetModules(ModulesStruct);

				Item = Gun->GunClass;
				Items.Add(Item);
				break;
			}
		}
	}
	if (Item == nullptr)
	{
		AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Player != nullptr)
		{
			UOPAG_UtilityFunctionLibrary::ShuffleArray(Modules);
			int Count = 0;
			while (Count < LootBox->NumberOfModulesToSpawn && Count < Modules.Num())
			{
				const int FireMask = 1 << (int)Player->PrimaryGun->FireType;
				Item = SelectRandomModules(DifficultyValue, FireMask);
				if (!Items.Contains(Item))
				{
					Items.Add(Item);
				}
				Count++;
			}
		}
	}

	// Set items in the loot-box
	LootBox->SetItemsToSpawn(Items);
}

void UOPAG_LootManagerComponent::Unlock() const
{
	if (LootBox) LootBox->Unlock();
}

void UOPAG_LootManagerComponent::PopulateLootLists()
{
	const FString Context;
	if (LootBox->LootChanceTable)
	{
		LootBox->LootChanceTable->GetAllRows(Context, LootChance);
	}
	if (LootDropTable)
	{
		LootDropTable->GetAllRows(Context, Drops);
	}
	if (LootModuleTable)
	{
		LootModuleTable->GetAllRows(Context, Modules);
	}
	if (LootGunTable)
	{
		LootGunTable->GetAllRows(Context, Guns);
	}
}

TSubclassOf<AActor> UOPAG_LootManagerComponent::SelectRandomModules(const float DifficultyValue, const int FireMask)
{
	TMap<TSubclassOf<AOPAG_ModuleBase>, float> ModuleMap;
	UOPAG_UtilityFunctionLibrary::ShuffleArray(Modules);
	for (FOPAG_ModuleList* Module : Modules)
	{
		AOPAG_ModuleBase* DefaultModule = Module->ModuleClass->GetDefaultObject<AOPAG_ModuleBase>();
		if (DefaultModule)
		{
			const int EquippableType = DefaultModule->EquippableFireTypes;
			if (FireMask & EquippableType)
			{
				float Probability = Module->ModuleLevel;
				TSubclassOf<AOPAG_ModuleBase> ModuleClass = Module->ModuleClass;
				ModuleMap.Add(ModuleClass, Probability);
			}
		}
	}
	if (ModuleMap.Num() > 0)
	{
		UOPAG_UtilityFunctionLibrary::MapBaseItemsWithDifficulty(ModuleMap, DifficultyValue);
		return UOPAG_UtilityFunctionLibrary::GetRandomItem(ModuleMap);
	}
	return nullptr;
}

TSubclassOf<AActor> UOPAG_LootManagerComponent::SelectRandomModules(const float DifficultyValue, const int FireMask, const EModuleTypes ModuleTypes)
{
	TMap<TSubclassOf<AOPAG_ModuleBase>, float> ModuleMap;
	UOPAG_UtilityFunctionLibrary::ShuffleArray(Modules);
	for (FOPAG_ModuleList* Module : Modules)
	{
		AOPAG_ModuleBase* DefaultModule = Module->ModuleClass->GetDefaultObject<AOPAG_ModuleBase>();
		if (DefaultModule)
		{
			const int EquippableType = DefaultModule->EquippableFireTypes;
			if (FireMask & EquippableType && DefaultModule->Type == ModuleTypes)
			{
				float Probability = Module->ModuleLevel;
				TSubclassOf<AOPAG_ModuleBase> ModuleClass = Module->ModuleClass;
				ModuleMap.Add(ModuleClass, Probability);
			}
		}
	}
	if (ModuleMap.Num() > 0)
	{
		UOPAG_UtilityFunctionLibrary::MapBaseItemsWithDifficulty(ModuleMap, DifficultyValue);
		return UOPAG_UtilityFunctionLibrary::GetRandomItem(ModuleMap);
	}
	return nullptr;
}