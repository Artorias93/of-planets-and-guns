// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Items/OPAG_Drop.h"
#include "Characters/Player/OPAG_Player.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"

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

void UOPAG_InventorySystemComponent::OnGoldShowFinished()
{
	Owner->EMS->HideGoldInGame.Broadcast();
}

void UOPAG_InventorySystemComponent::AnalyzeItem(AOPAG_Drop* Target)
{
	if (Target) {
		switch (Target->DropType) {

		case EDropType::Bolts:
			BoltsAmount += Target->Amount;
			if (Owner && Owner->EMS) {
				//Owner->EMS->BoltsAmountIncrement.Broadcast();
				Owner->EMS->BoltsAmountChanged.Broadcast(BoltsAmount);
				if (Owner->SMS)
					Owner->SMS->PlayAbiltySounds(Owner, EAbilitySoundAction::Ability_Ready);
			}
			Target->Destroy();
			break;

		case EDropType::Gold:
			SetGold(Target->Amount);
			Target->Destroy();
			break;

		case EDropType::Ammo:
			if (Owner && Owner->EMS) {
				Owner->EMS->AmmoPickedUp.Broadcast();
				if (Owner->SMS)
					Owner->SMS->PlayAbiltySounds(Owner, EAbilitySoundAction::Ability_Ready);
			}
			Target->Destroy();
			break;

		case EDropType::Potion:
			if (PotionsAmount < 4) 
			{
				PotionsAmount += Target->Amount;
				if (Owner && Owner->EMS) {
					//Owner->EMS->PotionsAmountIncrement.Broadcast();
					Owner->EMS->PotionsAmountChanged.Broadcast(PotionsAmount);
					if (Owner->SMS)
						Owner->SMS->PlayAbiltySounds(Owner, EAbilitySoundAction::Ability_Ready);
				}
				Target->Destroy();
			}
			break;

		default:
			break;
		}
	}
}

void UOPAG_InventorySystemComponent::SetGold(const int Value)
{
	GoldAmount += Value;
	if (Owner && Owner->EMS) {
		Owner->EMS->GoldChanged.Broadcast(GoldAmount);
		Owner->EMS->ShowGoldInGame.Broadcast();

		GetWorld()->GetTimerManager().SetTimer(GoldShowTimerHandle, this,
			&UOPAG_InventorySystemComponent::OnGoldShowFinished, GoldShowTimer, false);

		if (Owner->SMS)
			Owner->SMS->PlayAbiltySounds(Owner, EAbilitySoundAction::Ability_Ready);
	}
}