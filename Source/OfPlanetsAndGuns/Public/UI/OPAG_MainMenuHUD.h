#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_MainMenuHUD.generated.h"

class ULevelSequencePlayer;

UENUM()
enum EMenuScreen
{
	Start     UMETA(DisplayName = "Start"),
	MainMenu     UMETA(DisplayName = "MainMenu"),
	Options      UMETA(DisplayName = "Options"),
	Exit      UMETA(DisplayName = "Exit"),
	PlanetSelection   UMETA(DisplayName = "Attacking")
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MainMenuHUD : public AHUD
{
	GENERATED_BODY()

	//Variables

private:

	UPROPERTY()
	ULevelSequencePlayer* StartLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* OptionsLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* ExitLevelSequencePlayer;
	
	UPROPERTY()
	class UOPAG_UWMainMenuView* MainMenuViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOPAG_UWMainMenuView> MainMenuViewWidgetClass;
	UPROPERTY()
	class UOPAG_UWHubView* HubViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOPAG_UWHubView> HubViewWidgetClass;

	//Functions

public:
	virtual void DrawHUD() override;

	UFUNCTION()
	void ShowHubView();

	UFUNCTION()
	void ShowMainMenu();
	UFUNCTION()
	void HideMainMenu();
	UFUNCTION()
	void ShowExitMenu();
	UFUNCTION()
	void HideExitMenu();
	UFUNCTION()
	void ShowOptionsMenu();
	UFUNCTION()
	void HideOptionsMenu();

	UFUNCTION()
	void SequencePlaySelection(const EMenuScreen From, const EMenuScreen To) const;

protected:
	virtual void BeginPlay() override;

private:
	void BindEvents();	
};