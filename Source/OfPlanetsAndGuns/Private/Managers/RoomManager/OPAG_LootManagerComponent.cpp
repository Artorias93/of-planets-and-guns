#include "Managers/RoomManager/OPAG_LootManagerComponent.h"

#include "Interactables/OPAG_LootBox.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/GameplayStatics.h>

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
	TArray<TSubclassOf<AActor>> Items;
	for (FOPAG_LootChance* Drop : Drops)
	{
		if (Drop->LootChance == 100.f)
		{
			Items.Add(Drop->DropClass);
		}
		else
		{
			const float RandomDropChance = FMath::RandRange(0.f, 100.f);
			if (Drop->LootChance >= RandomDropChance)
			{
				Items.Add(Drop->DropClass);
			}
		}
	}

	// Guns Or Modules
	const float RandomChance = FMath::RandRange(0.f, 100.f);
	float Sum = 0.f;
	TSubclassOf<AActor> Item = nullptr;
	for (FOPAG_LootChance* Gun : Guns)
	{
		Sum += Gun->LootChance;
		if (RandomChance < Gun->LootChance)
		{
			Item = Gun->DropClass;
			Items.Add(Item);
			break;
		}
	}
	if (!Item)
	{
		AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Player)
		{
			TArray<TSubclassOf<AActor>> EquippableModules;
			UOPAG_UtilityFunctionLibrary::ShuffleArray(Modules);
			EFireTypes FireType = Player->PrimaryGun->FireType;
			for (FOPAG_LootChance* Module : Modules)
			{
				AOPAG_ModuleBase* TempModule = Module->DropClass->GetDefaultObject<AOPAG_ModuleBase>();
				if (TempModule)
				{
					const int EquippableType = TempModule->EquippableFireTypes;
					const int FireMask = 1 << (int)FireType;
					if (FireMask & EquippableType)
					{
						Sum += Module->LootChance;
						if (RandomChance < Sum)
						{
							Item = Module->DropClass;
							Items.Add(Item);
							break;
						}
						EquippableModules.Add(Module->DropClass);
					}
				}
			}
			if (!Item)
			{
				UOPAG_UtilityFunctionLibrary::ShuffleArray(EquippableModules);
				Items.Add(EquippableModules[0]);
			}
		}
	}

	// Set items in the lootbox
	LootBox->SetItemsToSpawn(Items);
}

void UOPAG_LootManagerComponent::PopulateLootLists()
{
	const FString Context;
	TArray<FOPAG_LootChance*> TempList;
	if (LootTable)
	{
		LootTable->GetAllRows(Context, TempList);
	}

	for (FOPAG_LootChance* Item : TempList)
	{
		switch (Item->DropType)
		{
			case ELootType::Drop :
				Drops.Add(Item);
				break;
			case ELootType::Module :
				Modules.Add(Item);
				break;
			case ELootType::Gun :
				Guns.Add(Item);
				break;
		}
	}
}