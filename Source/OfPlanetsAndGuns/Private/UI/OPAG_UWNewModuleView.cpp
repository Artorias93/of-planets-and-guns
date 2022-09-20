// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWNewModuleView.h"
#include "UI/OPAG_ModuleStatItem.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Kismet/KismetTextLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/ListView.h"

void UOPAG_UWNewModuleView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOPAG_UWNewModuleView::UpdateModuleValues(
	const FOPAG_GunStats& CurrentModuleStats,
	const FOPAG_GunStats& OtherModuleStats, 
	const FString OtherModuleName,
	const FString OtherModuleAbilityName,
	const EModuleRarity OtherModuleRarity, 
	const EModuleTypes OtherModuleType,
	const int EquippableTypes,
	const bool CanBeEquipped
)
{
	//Remove all previous icons and stats
	EquippableTypesBox->ClearChildren();
	StatsListView->ClearListItems();
	AbilityBox->ClearChildren();

	//Equippable-mask check
	//Skippare parte int e shiftare di valore enum direttamente
	//Refactoring in una funzione a parte
	int ResAR = EquippableTypes;
	bool isAREquippable = ResAR & 1;
	if (isAREquippable) 
	{
		UImage* const ARIcon = WidgetTree->ConstructWidget<UImage>();

		if (ARIcon && FullyAutomaticIconTexture) {
			ARIcon->SetBrushFromTexture(FullyAutomaticIconTexture);
			EquippableTypesBox->AddChild(ARIcon);
		}

		UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(EquippableTypesBox->GetSlots()[(EquippableTypesBox->GetChildrenCount() - 1)]);

		if (MyBoxSlot) {
			MyBoxSlot->SetPadding(FMargin(10, 10, 10, 10));
		}
	}

	int ResSs = ResAR >> 1;
	bool isSsEquippable = ResSs & 1;
	if (isSsEquippable)
	{
		UImage* const SingleShotIcon = WidgetTree->ConstructWidget<UImage>();

		if (SingleShotIcon && SingleShotIconTexture) {
			SingleShotIcon->SetBrushFromTexture(SingleShotIconTexture);
			EquippableTypesBox->AddChild(SingleShotIcon);
		}

		UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(EquippableTypesBox->GetSlots()[(EquippableTypesBox->GetChildrenCount() - 1)]);

		if (MyBoxSlot) {
			MyBoxSlot->SetPadding(FMargin(10, 10, 10, 10));
		}
	}

	int ResB3 = ResSs >> 1;
	bool isBurst3Equippable = ResB3 & 1;
	if (isBurst3Equippable)
	{
		UImage* const B3Icon = WidgetTree->ConstructWidget<UImage>();

		if (B3Icon && Burst3IconTexture) {
			B3Icon->SetBrushFromTexture(Burst3IconTexture);
			EquippableTypesBox->AddChild(B3Icon);
		}

		UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(EquippableTypesBox->GetSlots()[(EquippableTypesBox->GetChildrenCount() - 1)]);

		if (MyBoxSlot) {
			MyBoxSlot->SetPadding(FMargin(10, 10, 10, 10));
		}
	}

	int ResShg = ResB3 >> 1;
	bool isShotgunEquippable = ResShg & 1;
	if (isShotgunEquippable) 
	{
		UImage* const ShotgunIcon = WidgetTree->ConstructWidget<UImage>();

		if (ShotgunIcon && ShotgunIconTexture) {
			ShotgunIcon->SetBrushFromTexture(ShotgunIconTexture);
			EquippableTypesBox->AddChild(ShotgunIcon);
		}

		UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(EquippableTypesBox->GetSlots()[(EquippableTypesBox->GetChildrenCount() - 1)]);

		if (MyBoxSlot) {
			MyBoxSlot->SetPadding(FMargin(10, 10, 10, 10));
		}
	}

	ModuleNameText->SetText(FText::FromString(OtherModuleName));
	ModuleRarityText->SetText(FText::FromString(AOPAG_ModuleBase::GetRarityName(OtherModuleRarity)));
	ModuleTypeText->SetText(FText::FromString(AOPAG_ModuleBase::GetTypeName(OtherModuleType)));

	AddStatEntry("Damage: ", (int)CurrentModuleStats.Damage, (int)OtherModuleStats.Damage);
	AddStatEntry("Fire Rate: ", (int)CurrentModuleStats.FireRate, (int)OtherModuleStats.FireRate);
	AddStatEntry("Accuracy: ", (int)CurrentModuleStats.Accuracy, (int)OtherModuleStats.Accuracy);
	AddStatEntry("Stability: ", (int)CurrentModuleStats.Stability, (int)OtherModuleStats.Stability);
	AddStatEntry("Reloading Time: ", CurrentModuleStats.ReloadingTime, OtherModuleStats.ReloadingTime, true);
	AddStatEntry("Clip Size: ", CurrentModuleStats.ClipSize, OtherModuleStats.ClipSize);
	
	if (OtherModuleType == EModuleTypes::Accessory)
	{
		UTextBlock* const AbilityText = WidgetTree->ConstructWidget<UTextBlock>();

		if (AbilityText) 
		{
			FString abilityDescription = OtherModuleAbilityName;

			AbilityText->SetText(FText::FromString("Ability: \n" + abilityDescription));
			AbilityBox->AddChild(AbilityText);
		}
		
		UImage* const BarImage = WidgetTree->ConstructWidget<UImage>();

		if (BarImage) 
		{
			BarImage->SetBrushSize(FVector2D(400, 10));
			BarImage->SetOpacity(0.4f);
			AbilityBox->AddChild(BarImage);
		}
	}

	if (CanBeEquipped) 
	{
		ModuleBackground->SetColorAndOpacity(BackgroundBlue);
		ModuleForeground->SetColorAndOpacity(ForegroundBlue);
		EquippableText->SetText(FText::FromString("Equipable"));
	}
	else 
	{
		ModuleBackground->SetColorAndOpacity(BackgroundRed);
		ModuleForeground->SetColorAndOpacity(ForegroundRed);
		EquippableText->SetText(FText::FromString("Not Equipable"));
	}
}

void UOPAG_UWNewModuleView::AddStatEntry(FString StatName, float OldStat, float NewStat, bool bIsInverted) 
{
	float Delta = NewStat - OldStat;

	if (Delta != 0) 
	{
		UOPAG_ModuleStatItem* item = NewObject<UOPAG_ModuleStatItem>(UOPAG_ModuleStatItem::StaticClass());
		item->Init(StatName, FString::Printf(TEXT("%.2f"), OldStat), FString::Printf(TEXT("%.2f"), NewStat), Delta < 0, bIsInverted);
		StatsListView->AddItem(item);
		item->MarkPendingKill();
	}
}

void UOPAG_UWNewModuleView::AddStatEntry(FString StatName, int OldStat, int NewStat, bool bIsInverted)
{
	int Delta = NewStat - OldStat;

	if (Delta != 0)
	{
		UOPAG_ModuleStatItem* item = NewObject<UOPAG_ModuleStatItem>(UOPAG_ModuleStatItem::StaticClass());
		item->Init(StatName, FString::FromInt(NewStat), FString::FromInt(Delta), Delta < 0, bIsInverted);
		StatsListView->AddItem(item);
		item->MarkPendingKill();
	}
}
