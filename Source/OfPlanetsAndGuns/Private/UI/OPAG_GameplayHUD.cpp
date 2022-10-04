#include "UI/OPAG_GameplayHUD.h"
#include "UI/OPAG_UWGunView.h"
#include "UI/OPAG_UWCurrencyView.h"
#include "UI/OPAG_UWPlayerStatsView.h"
#include "UI/OPAG_UWPauseView.h"
#include "UI/OPAG_UWLoadingScreenView.h"
#include "UI/OPAG_UWNewModuleView.h"
#include "UI/OPAG_UWGunCompareView.h"
#include "UI/OPAG_UWInteractView.h"
#include "UI/OPAG_UWDamageIndicatorView.h"
#include "UI/OPAG_UWGameOverView.h"
#include "UI/OPAG_UWOptionsView.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "UI/OPAG_PlayerStatus.h"
#include "Weapons/Modules/OPAG_AccessoryBase.h"


class AOPAG_AccessoryBase;

void AOPAG_GameplayHUD::DrawHUD() 
{
	Super::DrawHUD();
}

void AOPAG_GameplayHUD::BindEvents() {
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	if (EMS) 
	{
		EMS->PauseMenuGunChanged.AddDynamic(this, &AOPAG_GameplayHUD::ShowPauseView);
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
		EMS->ShieldDamagedBy.AddDynamic(this, &AOPAG_GameplayHUD::ShowShieldDamageIndicatorView);
	}
}

void AOPAG_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	BindEvents();

	if (PlayerStatsViewWidgetClass)
	{
		PlayerStatsViewWidget = CreateWidget<UOPAG_UWPlayerStatsView>(GetWorld(), PlayerStatsViewWidgetClass);

		if (PlayerStatsViewWidget) 
		{
			PlayerStatsViewWidget->AddToViewport(1);
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

	if (PlayerStatusViewWidgetClass != nullptr)
	{
		PlayerStatusViewWidget = CreateWidget<UOPAG_PlayerStatus>(GetWorld(), PlayerStatusViewWidgetClass);

		if (PlayerStatusViewWidget)
		{
			PlayerStatusViewWidget->AddToViewport(0);
		}
	}
}

void AOPAG_GameplayHUD::ShowPauseView(const FOPAG_GunStats GunStats,
	const FString GunName, const FString GunFireType, const TArray<AOPAG_ModuleBase*> Modules)
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
		PauseViewWidget->UpdateGunValues(GunStats, GunName, GunFireType, Modules);
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

	if (OptionsViewWidget) 
	{
		if (OptionsViewWidget->IsInViewport())
			OptionsViewWidget->RemoveFromViewport();
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

void AOPAG_GameplayHUD::ShowModuleCompareView(const FOPAG_GunStats CurrentModuleStats,
                                              const AOPAG_ModuleBase* const OtherModule,
                                              const bool CanBeEquipped)
{
	if (GunCompareViewWidget->IsInViewport())
		HideGunCompareView();

	if (ModuleViewWidget) 
	{
		if (!ModuleViewWidget->IsInViewport()) 
		{
			ModuleViewWidget->AddToViewport(3);
		}
		
		const AOPAG_AccessoryBase* Accessory = Cast<AOPAG_AccessoryBase>(OtherModule);
		FString AbilityDescription = Accessory ? Accessory->AbilityDescription : "";
		
		ModuleViewWidget->UpdateModuleValues(
			CurrentModuleStats,
			OtherModule->GetStats(),
			OtherModule->GetModuleName(),
			AbilityDescription,
			OtherModule->GetRarity(),
			OtherModule->GetType(),
			OtherModule->GetBrand(),
			OtherModule->GetEquippableFireTypes(), CanBeEquipped);
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
			InteractViewWidget->AddToViewport(2);
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

void AOPAG_GameplayHUD::ShowShieldDamageIndicatorView(AActor* const CurrentThreatActor)
{
	// This widget can be instantiated multiple times

	if (ShieldDamageIndicatorViewWidgetClass)
	{
		ShieldDamageIndicatorViewWidget = CreateWidget<UOPAG_UWDamageIndicatorView>(GetWorld(), ShieldDamageIndicatorViewWidgetClass);
	}
	
	if(ShieldDamageIndicatorViewWidget)
	{
		if(!ShieldDamageIndicatorViewWidget->IsVisible())
		{
			ShieldDamageIndicatorViewWidget->AddToViewport(1);
		}

		ShieldDamageIndicatorViewWidget->SetCurrentThreatActor(CurrentThreatActor);
	}
}

void AOPAG_GameplayHUD::HideDamageIndicatorView()
{
	if (DamageIndicatorViewWidget) 
	{
		DamageIndicatorViewWidget->RemoveFromViewport();
	}
	if (ShieldDamageIndicatorViewWidget) 
	{
		ShieldDamageIndicatorViewWidget->RemoveFromViewport();
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

void AOPAG_GameplayHUD::GoToMenu()
{
	const UOPAG_EventManagerSubsystem* const EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS)
	{
		EMS->LoadMainMenu.Broadcast();
	}
}