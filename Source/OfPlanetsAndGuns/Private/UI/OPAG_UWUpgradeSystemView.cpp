// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWUpgradeSystemView.h"
#include "UI/OPAG_UWUpgradeSystemItemView.h"


void UOPAG_UWUpgradeSystemView::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UOPAG_UWUpgradeSystemItemView* Item = Cast<UOPAG_UWUpgradeSystemItemView>(ListItemObject);
}

void UOPAG_UWUpgradeSystemView::NativeConstruct()
{
	Super::NativeConstruct();

	// Imagine we have an inventory class that provides us with the following:
	TArray<UOPAG_UWUpgradeSystemItemView*> Inventory;

	for (UOPAG_UWUpgradeSystemItemView* Item : Inventory)
	{
		// Instantiate the widget
		UOPAG_UWUpgradeSystemItemView* Entry = CreateWidget<UOPAG_UWUpgradeSystemItemView>(this, UpgradeSystemItemViewWidgetClass);

		// Set up its contents
		Entry->InitializeFromInventoryItem(Item);

		// Add it to the list
		EntriesVerticalBox->AddChildToVerticalBox(Entry);
	}
}