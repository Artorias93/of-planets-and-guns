#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "OPAG_GameplayHUD.generated.h"

enum class EModuleRarity : uint8;
struct FOPAG_GunStats;

class UOPAG_PlayerStatus;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GameplayHUD : public AHUD
{
	GENERATED_BODY()

	void BindEvents();

	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class UOPAG_UWPlayerStatsView* PlayerStatsViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWPlayerStatsView> PlayerStatsViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWGunView* GunViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWGunView> GunViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWCurrencyView* CurrencyViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWCurrencyView> CurrencyViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWPauseView* PauseViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWPauseView> PauseViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWLoadingScreenView* LoadingScreenViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWLoadingScreenView> LoadingScreenViewWidgetClass;

	UPROPERTY()
		class UOPAG_UWNewModuleView* ModuleViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWNewModuleView> ModuleViewWidgetClass;

	UPROPERTY()
		class UOPAG_UWGunCompareView* GunCompareViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWGunCompareView> GunCompareViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWGameOverView* GameOverViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWGameOverView> GameOverViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWInteractView* InteractViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWInteractView> InteractViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWDamageIndicatorView* DamageIndicatorViewWidget;
	UPROPERTY()
	UOPAG_UWDamageIndicatorView* ShieldDamageIndicatorViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWDamageIndicatorView> DamageIndicatorViewWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOPAG_UWDamageIndicatorView> ShieldDamageIndicatorViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWOptionsView* OptionsViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWOptionsView> OptionsViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWPauseMenuGunView* PauseMenuGunViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWPauseMenuGunView> PauseMenuGunViewWidgetClass;
	UPROPERTY() 
		UOPAG_PlayerStatus* PlayerStatusViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_PlayerStatus> PlayerStatusViewWidgetClass;

public:
	virtual void DrawHUD() override;

	UFUNCTION()
	void ShowPauseView(const FOPAG_GunStats GunStats, const FString GunName, const FString GunFireType, 
			const TArray<AOPAG_ModuleBase*> Modules);

	UFUNCTION()
	void HidePauseView();

	UFUNCTION()
	void UpdateLoadingScreenBar(float Percentage);
	UFUNCTION()
	void ShowLoadingScreenView();
	UFUNCTION()
	void HideLoadingScreenView(int32 RoomCount);

	UFUNCTION()
	void ShowModuleCompareView(const FOPAG_GunStats CurrentModule,
	                           const FOPAG_GunStats OtherModule, const FString OtherModuleName, 
							   const FString OtherModuleAbilityName, const EModuleRarity OtherModuleRarity,
	                           const EModuleTypes OtherModuleType, const int EquippableTypes, const bool CanBeEquipped);
	UFUNCTION()
	void HideModuleCompareView();

	UFUNCTION()
	void ShowGunCompareView(const FOPAG_GunStats GunStats, 
							const FOPAG_GunStats OtherGun, 
							const FString OtherGunName,
							const FString OtherGunType);
	UFUNCTION()
	void HideGunCompareView();

	UFUNCTION()
	void ShowGameOverView();

	UFUNCTION()
	void ShowInteractView(bool bCanInteract, const FString InteractText);
	UFUNCTION()
	void HideInteractView();

	UFUNCTION()
	void ShowDamageIndicatorView(AActor* const CurrentThreatActor);
	UFUNCTION()
	void ShowShieldDamageIndicatorView(AActor* const CurrentThreatActor);
	UFUNCTION()
	void HideDamageIndicatorView();

	UFUNCTION()
	void ShowOptionsView();
	UFUNCTION()
	void HideOptionsView();

	UFUNCTION()
	void GoToMenu();
};
