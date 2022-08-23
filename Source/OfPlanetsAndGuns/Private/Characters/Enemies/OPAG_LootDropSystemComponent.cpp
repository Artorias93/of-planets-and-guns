// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/OPAG_LootDropSystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Math/UnrealMathUtility.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Components/SphereComponent.h"

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
		LootChanceTable->GetAllRows(Context, LootArray);
	if (DropListTable)
		DropListTable->GetAllRows(Context, DropArray);
}

void UOPAG_LootDropSystemComponent::DropLoot() const
{
	float Chance = FMath::RandRange(0.f, 100.f);

	for (FOPAG_LootChance *Item : LootArray)
	{
		if (!Item) continue;
		if (Chance < Item->LootChance) {


			FVector SpawnLocation = Owner->GetActorLocation();

			for (FOPAG_DropList* Drop : DropArray) {
				if (!Drop) continue;
				if (Item->LootType == Drop->DropType) {
					AOPAG_Drop* DropObj = GetWorld()->SpawnActor<AOPAG_Drop>(Drop->DropClass, SpawnLocation, FRotator::ZeroRotator);

					if (DropObj) {
						DropObj->Sphere->SetSimulatePhysics(true);
						return;
					}
				}
			}

		}
	}
}
