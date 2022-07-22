// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Items/OPAG_Ammo.h"
#include "Characters/Player/OPAG_Player.h"

UOPAG_InventorySystemComponent::UOPAG_InventorySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOPAG_InventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	AOPAG_Player* Player = GetOwner<AOPAG_Player>();
	if (Player) {
		Owner = Player;
	}
}

//WIP
void UOPAG_InventorySystemComponent::AnalyzeItem(AOPAG_Drop* Target)
{
	if (Target) {
		switch (Target->DropType) {

		case EDropType::Bolts:
			BoltsAmount++;
			//EMS -> Broadcast
			Target->Destroy();
			break;

		case EDropType::Gold:
			GoldAmount++;
			//EMS -> Broadcast
			Target->Destroy();
			break;

		case EDropType::Ammo:
			break;

		default:
			break;
		}
	}
}