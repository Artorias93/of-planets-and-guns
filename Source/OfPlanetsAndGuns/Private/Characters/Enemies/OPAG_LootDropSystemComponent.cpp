// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/OPAG_LootDropSystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Math/UnrealMathUtility.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Characters/Player/OPAG_Player.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/RoomManager/OPAG_LootManagerComponent.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include <GameFramework/CharacterMovementComponent.h>

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

void UOPAG_LootDropSystemComponent::DropLoot()
{
	float Chance = FMath::RandRange(0.f, 100.f);

	// Calculate SpawnLocation
	FVector SpawnLocation = Owner->GetActorLocation();
	if (!Owner->GetCharacterMovement()->IsMovingOnGround())
	{
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;
		const bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			Owner->GetActorLocation(), Owner->GetActorLocation() + (-Owner->GetActorUpVector() * 5000),
			TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
		
		if (bResult)
		{
			SpawnLocation = HitResult.ImpactPoint;
		}
	}

	for (FOPAG_LootChance *Item : LootChances)
	{
		if (!Item) continue;
		if (Chance < Item->LootChance)
		{
			for (FOPAG_DropList* Drop : DropList) 
			{
				if (!Drop) continue;

				if (Item->LootType == Drop->DropType)
				{
					AOPAG_Drop* DropObj = GetWorld()->SpawnActor<AOPAG_Drop>(Drop->DropClass, 
						SpawnLocation, FRotator::ZeroRotator);

					if (DropObj != nullptr)
					{
						DropObj->StartDropTimeline();
					}

					/*if (DropObj)
					{
						//DropObj->Sphere->SetSimulatePhysics(true);
						continue;
					}*/
				}
			}

			if (Item->LootType == ELootType::Module)
			{
				const AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				if (Player != nullptr)
				{
					const AOPAG_ModularGun* PrimaryGun = Player->GetPrimaryGun();
					if (PrimaryGun != nullptr)
					{
						const int FireMask = 1 << static_cast<int>(PrimaryGun->GetFireType());
						UOPAG_GameInstance* GI = GetWorld()->GetGameInstance<UOPAG_GameInstance>();
						checkf(GI, TEXT("Couldn't find GameInstance"));
						TSubclassOf<AOPAG_ModuleBase> ModuleType {UOPAG_LootManagerComponent::SelectRandomModule(
							ModuleList, GI->GetStageDifficulty(), FireMask)};

						if (ModuleType)
						{
							AOPAG_ModuleBase* ModuleObj = GetWorld()->SpawnActor<AOPAG_ModuleBase>(ModuleType,
								SpawnLocation, FRotator::ZeroRotator);

							if (ModuleObj)
							{
								ModuleObj->SetStats(PrimaryGun->GetSecondaryStatsModifiers());
								ModuleObj->GetModuleSphereComponent()->SetSimulatePhysics(false);
								ModuleObj->StartDropTimeline();
							}
						}
					}
				}
			}

		}
	}
}
