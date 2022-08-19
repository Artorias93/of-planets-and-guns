// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWUpgradeSystemView.h"
#include "UI/OPAG_UWUpgradeSystemItemView.h"
#include "Components/ListView.h"
#include "Characters/OPAG_AttributeSetBase.h"


void UOPAG_UWUpgradeSystemView::NativeConstruct()
{
	Super::NativeConstruct();

	// Imagine we have an inventory class that provides us with the following:
	TArray<UOPAG_UWUpgradeSystemItemView*> Inventory;
	//InventoryListView->SetListItems(InventoryItems);
}