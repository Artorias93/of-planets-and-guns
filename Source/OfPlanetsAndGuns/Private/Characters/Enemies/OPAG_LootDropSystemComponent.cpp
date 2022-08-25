// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/OPAG_LootDropSystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Math/UnrealMathUtility.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Components/SphereComponent.h"
#include "Managers/OPAG_GameInstance.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"

UOPAG_LootDropSystemComponent::UOPAG_LootDropSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOPAG_LootDropSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	PopulateLootChanceArray();

	AOPAG_Enemy* Enemy = GetOwner<AOPAG_Enemy>();
	if (Enemy) {
		Owner = Enemy;
	}
}

void UOPAG_LootDropSystemComponent::PopulateLootChanceArray()
{
	const FString Context;
	if (LootChanceTable)
		LootChanceTable->GetAllRows(Context, LootChances);
	if (DropListTable)
		DropListTable->GetAllRows(Context, DropList);
	if (LootModuleTable)
		LootModuleTable->GetAllRows(Context, ModuleList);
}

void UOPAG_LootDropSystemComponent::DropLoot() const
{
	float Chance = FMath::RandRange(0.f, 100.f);

	for (FOPAG_LootChance *Item : LootChances)
	{
		if (!Item) continue;
		if (Chance < Item->LootChance) {


			FVector SpawnLocation = Owner->GetActorLocation();

			for (FOPAG_DropList* Drop : DropList) 
			{
				if (!Drop) continue;

				if (Item->LootType == Drop->DropType)
				{
					AOPAG_Drop* DropObj = GetWorld()->SpawnActor<AOPAG_Drop>(Drop->DropClass, 
						SpawnLocation, FRotator::ZeroRotator);

					if (DropObj)
					{
						DropObj->Sphere->SetSimulatePhysics(true);
						continue;
					}
				}
			}

			if (Item->LootType == ELootType::Module)
			{
				TMap<TSubclassOf<AOPAG_ModuleBase>, float> ModuleMap;
				for (FOPAG_ModuleList* Module : ModuleList)
				{
					ModuleMap.Add(Module->ModuleClass, Module->ModuleLevel);
				}
				if (ModuleMap.Num() > 0)
				{
					UOPAG_GameInstance* GI = GetWorld()->GetGameInstance<UOPAG_GameInstance>();
					checkf(GI, TEXT("Couldn't find GameInstance"));

					UOPAG_UtilityFunctionLibrary::MapBaseItemsWithDifficulty(ModuleMap,
						GI->GetStageDifficulty());

					TSubclassOf<AOPAG_ModuleBase> ModuleType =
						UOPAG_UtilityFunctionLibrary::GetRandomItem(ModuleMap);

					if (ModuleType)
					{
						AOPAG_ModuleBase* ModuleObj = GetWorld()->SpawnActor<AOPAG_ModuleBase>(ModuleType,
							SpawnLocation, FRotator::ZeroRotator);

						if (ModuleObj)
						{
							ModuleObj->Sphere->SetSimulatePhysics(true);
						}
					}
				}
			}

		}
	}
}
