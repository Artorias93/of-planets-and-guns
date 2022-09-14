#include "UI/OPAG_GameplayHUD.h"
#include "UI/OPAG_UWGunView.h"
#include "UI/OPAG_UWCurrencyView.h"
#include "UI/OPAG_UWPlayerStatsView.h"
#include "UI/OPAG_UWPauseView.h"
#include "UI/OPAG_UWLoadingScreenView.h"
//#include "UI/OPAG_UWModuleView.h"
#include "UI/OPAG_UWNewModuleView.h"
#include "UI/OPAG_UWGunCompareView.h"
#include "UI/OPAG_UWInteractView.h"
#include "UI/OPAG_UWDamageIndicatorView.h"
#include "UI/OPAG_UWGameOverView.h"
#include "UI/OPAG_UWOptionsView.h"
#include "UI/OPAG_UWGameplayHUDView.h"
#include "Components/RetainerBox.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Weapons/Guns/OPAG_GunStats.h"


void AOPAG_GameplayHUD::DrawHUD() 
{
	Super::DrawHUD();
}

void AOPAG_GameplayHUD::BindEvents() {
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	if (EMS) 
	{
		EMS->PauseGame.AddDynamic(this, &AOPAG_GameplayHUD::ShowPauseView);
		EMS->LoadingProgress.AddDynamic(this, &AOPAG_GameplayHUD::UpdateLoadingScreenBar);
		EMS->StageLoaded.AddDynamic(this, &AOPAG_GameplayHUD::HideLoadingScreenView);
		EMS->LookedAtModule.AddDynamic(this, &AOPAG_GameplayHUD::ShowModuleCompareView);
		EMS->UnlookedAtModule.AddDynamic(this, &AOPAG_GameplayHUD::HideModuleCompareView);
		EMS->LookedAtGun.AddDynamic(this, &AOPAG_GameplayHUD::ShowGunCompareView);
		EMS->UnlookedAtGun.AddDynamic(this, &AOPAG_GameplayHUD::HideGunCompareView);
		EMS->PlayerDied.AddDynamic(this, &AOPAG_GameplayHUD::ShowGameOverView);
		EMS->LookedAtInteractable.AddDynamic(this, &AOPAG_GameplayHUD::ShowInteractView);
		EMS->UnlookedAtInteractable.AddDynamic(this, &AOPAG_GameplayHUD::HideInteractView);
		EMS->DamagedBy.AddDynamic(this, &AOPAG_GameplayHUD::ShowDamageIndicatorView);
	}
}

void AOPAG_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	BindEvents();

	if (GameplayHUDViewWidgetClass)
	{
		GameplayHUDViewWidget = CreateWidget<UOPAG_UWGameplayHUDView>(GetWorld(), GameplayHUDViewWidgetClass);

		if (GameplayHUDViewWidget)
		{
			GameplayHUDViewWidget->AddToViewport(1);
		}
	}

	if (PlayerStatsViewWidgetClass)
	{
		PlayerStatsViewWidget = CreateWidget<UOPAG_UWPlayerStatsView>(GetWorld(), PlayerStatsViewWidgetClass);

		if (PlayerStatsViewWidget) 
		{
			//PlayerStatsViewWidget->AddToViewport(1);
			GameplayHUDViewWidget->GameplayRetainerBox->AddChild(PlayerStatsViewWidget);
		}
	}

	if (GunViewWidgetClass)
	{
		GunViewWidget = CreateWidget<UOPAG_UWGunView>(GetWorld(), GunViewWidgetClass);

		if (GunViewWidget) 
		{
			GunViewWidget->AddToViewport(1);
		}
	}

	if (CurrencyViewWidgetClass)
	{
		CurrencyViewWidget = CreateWidget<UOPAG_UWCurrencyView>(GetWorld(), CurrencyViewWidgetClass);

		if (CurrencyViewWidget) 
		{
			CurrencyViewWidget->AddToViewport(1);
		}
	}

	if (LoadingScreenViewWidgetClass)
	{
		LoadingScreenViewWidget = CreateWidget<UOPAG_UWLoadingScreenView>(GetWorld(), LoadingScreenViewWidgetClass); 
		
		if (LoadingScreenViewWidget) 
		{
			LoadingScreenViewWidget->AddToViewport(3);
		}
	}

	if (PauseViewWidgetClass)
	{
		PauseViewWidget = CreateWidget<UOPAG_UWPauseView>(GetWorld(), PauseViewWidgetClass);
	}
	
	if (ModuleViewWidgetClass)
	{
		ModuleViewWidget = CreateWidget<UOPAG_UWNewModuleView>(GetWorld(), ModuleViewWidgetClass);
	}
	
	if (GunCompareViewWidgetClass)
	{
		GunCompareViewWidget = CreateWidget<UOPAG_UWGunCompareView>(GetWorld(), GunCompareViewWidgetClass);
	}

	if (GameOverViewWidgetClass)
	{
		GameOverViewWidget = CreateWidget<UOPAG_UWGameOverView>(GetWorld(), GameOverViewWidgetClass);
	}

	if (InteractViewWidgetClass)
	{
		InteractViewWidget = CreateWidget<UOPAG_UWInteractView>(GetWorld(), InteractViewWidgetClass);
	}

	if (OptionsViewWidgetClass)
	{
		OptionsViewWidget = CreateWidget<UOPAG_UWOptionsView>(GetWorld(), OptionsViewWidgetClass);
	}
}

void AOPAG_GameplayHUD::ShowPauseView() 
{

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC) 
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}
	
	if (PauseViewWidget) 
	{
		PauseViewWidget->AddToViewport(2);
	}
}

void AOPAG_GameplayHUD::HidePauseView() {

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC) 
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}

	if (PauseViewWidget) 
	{
		PauseViewWidget->RemoveFromViewport();
	}

	const UOPAG_EventManagerSubsystem* const EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS) 
	{
		EMS->ResumeGame.Broadcast();
	}
}

void AOPAG_GameplayHUD::UpdateLoadingScreenBar(float Percentage)
{
	if (LoadingScreenViewWidget)
		LoadingScreenViewWidget->SetLoadingBar(Percentage);
}

void AOPAG_GameplayHUD::ShowLoadingScreenView()
{
	//TODO: Empty Function
}

void AOPAG_GameplayHUD::HideLoadingScreenView(int32 RoomCount)
{
	if (LoadingScreenViewWidget) 
	{
		LoadingScreenViewWidget->RemoveFromViewport();
	}
}

void AOPAG_GameplayHUD::ShowModuleCompareView(const FOPAG_GunStats CurrentModule, 
	const FOPAG_GunStats OtherModule, const FString OtherModuleName, const FString OtherModuleAbilityName, 
	const EModuleRarity OtherModuleRarity, const EModuleTypes OtherModuleType, const int EquippableTypes, 
	const bool CanBeEquipped)
{
	if (GunCompareViewWidget->IsInViewport())
		HideGunCompareView();

	if (ModuleViewWidget) 
	{
		if (ModuleViewWidget->IsInViewport()) 
		{
			ModuleViewWidget->UpdateModuleValues(CurrentModule, OtherModule, OtherModuleName, OtherModuleAbilityName, 
				OtherModuleRarity, OtherModuleType, EquippableTypes, CanBeEquipped);
		}
		else 
		{
			ModuleViewWidget->AddToViewport(3);
			ModuleViewWidget->UpdateModuleValues(CurrentModule, OtherModule, OtherModuleName, OtherModuleAbilityName, 
				OtherModuleRarity, OtherModuleType, EquippableTypes, CanBeEquipped);
		}
	}
}

void AOPAG_GameplayHUD::HideModuleCompareView()
{
	if (ModuleViewWidget) 
	{
		ModuleViewWidget->RemoveFromViewport();
	}
}

void AOPAG_GameplayHUD::ShowGunCompareView(const FOPAG_GunStats GunStats, const FOPAG_GunStats OtherGun, const FString OtherGunName, 
	const FString OtherGunType)
{
	if (ModuleViewWidget->IsInViewport())
		HideModuleCompareView();

	if (GunCompareViewWidget) 
	{
		if (GunCompareViewWidget->IsInViewport()) 
		{
			GunCompareViewWidget->UpdateGunValues(GunStats, OtherGun, OtherGunName, OtherGunType);
		}
		else 
		{
			GunCompareViewWidget->AddToViewport(3);
			GunCompareViewWidget->UpdateGunValues(GunStats, OtherGun, OtherGunName, OtherGunType);
		}
	}
}

void AOPAG_GameplayHUD::HideGunCompareView()
{
	if (GunCompareViewWidget) 
	{
		GunCompareViewWidget->RemoveFromViewport();
	}
}

void AOPAG_GameplayHUD::ShowGameOverView()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC) 
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}

	if (GameOverViewWidget) 
	{
		GameOverViewWidget->AddToViewport(3);
	}
}

void AOPAG_GameplayHUD::ShowInteractView(bool bCanInteract, const FString InteractText)
{
	if (InteractViewWidget) 
	{
		if (!InteractViewWidget->IsInViewport())
		{
			InteractViewWidget->AddToViewport(3);
		}

		InteractViewWidget->UpdateInteractText(bCanInteract, InteractText);
	}
}

void AOPAG_GameplayHUD::HideInteractView()
{
	if (InteractViewWidget) 
	{
		InteractViewWidget->RemoveFromViewport();
	}
}

void AOPAG_GameplayHUD::ShowDamageIndicatorView(AActor* const CurrentThreatActor)
{
	// This widget can be instantiated multiple times

	if (DamageIndicatorViewWidgetClass)
	{
		DamageIndicatorViewWidget = CreateWidget<UOPAG_UWDamageIndicatorView>(GetWorld(), DamageIndicatorViewWidgetClass);
	}
	
	if(DamageIndicatorViewWidget)
	{
		if(!DamageIndicatorViewWidget->IsVisible())
		{
			DamageIndicatorViewWidget->AddToViewport(1);
		}

		DamageIndicatorViewWidget->SetCurrentThreatActor(CurrentThreatActor);
	}
}

void AOPAG_GameplayHUD::HideDamageIndicatorView()
{
	if (DamageIndicatorViewWidget) 
	{
		DamageIndicatorViewWidget->RemoveFromViewport();
	}
}

void AOPAG_GameplayHUD::ShowOptionsView()
{
	if (OptionsViewWidget)
	{
		OptionsViewWidget->AddToViewport(3);
	}
}

void AOPAG_GameplayHUD::HideOptionsView()
{
	if (OptionsViewWidget)
	{
		OptionsViewWidget->RemoveFromViewport();
	}
}