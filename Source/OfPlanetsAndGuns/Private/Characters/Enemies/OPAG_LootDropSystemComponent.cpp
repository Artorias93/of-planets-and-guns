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

	AOPAG_Enemy* Enemy = GetOwner<AOPAG_Enemy>();
	if (Enemy) {
		Owner = Enemy;
	}
}

void UOPAG_LootDropSystemComponent::DropLoot() const
{
	for (FOPAG_Loot Loot : LootArray)
	{
		float Chance = FMath::RandRange(0.f, 1.f);

		if (Chance < Loot.DroppingChance) {

			FVector SpawnLocation = Owner->GetActorLocation();

			if (Loot.Drop) {
				AOPAG_Drop* DropObj = GetWorld()->SpawnActor<AOPAG_Drop>(Loot.Drop, FVector::RightVector, FRotator::ZeroRotator);

				if (DropObj) {
					DropObj->Sphere->SetSimulatePhysics(true);
				}
			}
		}
	}
}
