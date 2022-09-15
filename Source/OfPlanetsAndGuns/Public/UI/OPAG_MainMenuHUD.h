#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_MainMenuHUD.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

UENUM()
enum EMenuScreen
{
	Start     UMETA(DisplayName = "Start"),
	MainMenu     UMETA(DisplayName = "MainMenu"),
	Options      UMETA(DisplayName = "Options"),
	Exit      UMETA(DisplayName = "Exit"),
	QuitGame      UMETA(DisplayName = "QuitGame"),
	PlanetSelection   UMETA(DisplayName = "PlanetSelection")
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
	ULevelSequencePlayer* QuitGameLevelSequencePlayer;
	
	UPROPERTY()
	class UOPAG_UWMainMenuView* MainMenuViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOPAG_UWMainMenuView> MainMenuViewWidgetClass;
	UPROPERTY()
	class UOPAG_UWConfirmExitView* ConfirmExitViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOPAG_UWConfirmExitView> ConfirmExitViewWidgetClass;
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
	void MenuCloseGame();
	UFUNCTION()
	void ShowOptionsMenu();
	UFUNCTION()
	void HideOptionsMenu();

	UFUNCTION(CallInEditor)
	void GoFromMainMenuToExit();
	UFUNCTION(CallInEditor)
	void GoFromExitToMainMenu();
	UFUNCTION()
	void SequencePlaySelection(const EMenuScreen From, const EMenuScreen To) const;

protected:
	virtual void BeginPlay() override;

//private:
	//void BindEvents();	
};