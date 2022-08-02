// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWModuleCompareView.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "UI/OPAG_ModuleViewUW.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWModuleCompareView::NativeConstruct()
{

}

/*
void UOPAG_UWModuleCompareView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS) {
		EMS->LookedAtModule.AddDynamic(this, &UOPAG_UWCurrencyView::UpdateModulesValues);
	}
}
*/

void UOPAG_UWModuleCompareView::UpdateModulesValues(const FOPAG_GunStats& CurrentModule, const FOPAG_GunStats& OtherModule,
	const FString CurrentModuleName, const EModuleRarity CurrentModuleRarity, const FString OtherModuleName, const EModuleRarity OtherModuleRarity)
{

}
