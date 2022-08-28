// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Characters/Player/OPAG_Player.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

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

void UOPAG_InventorySystemComponent::AnalyzeItem(AOPAG_Drop* Target)
{
	if (Target) {
		switch (Target->DropType) {

		case EDropType::Bolts:
			BoltsAmount += Target->Amount;
			if (Owner && Owner->EMS) {
				Owner->EMS->BoltsAmountIncrement.Broadcast();
			}
			Target->Destroy();
			break;

		case EDropType::Gold:
			GoldAmount += Target->Amount;
			if (Owner && Owner->EMS) {
				Owner->EMS->GoldChanged.Broadcast(GoldAmount);
			}
			Target->Destroy();
			break;

		case EDropType::Ammo:
			if (Owner && Owner->EMS) {
				Owner->EMS->AmmoPickedUp.Broadcast();
			}
			Target->Destroy();
			break;

		case EDropType::Potion:
			if (PotionsAmount < 4) {
				PotionsAmount += Target->Amount;
				if (Owner && Owner->EMS) {
					Owner->EMS->PotionsAmountIncrement.Broadcast();
				}
				Target->Destroy();
			}
			break;

		default:
			break;
		}
	}
}