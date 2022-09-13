#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_MainMenuHUD.generated.h"

class ULevelSequencePlayer;
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
	void ShowOptionsMenu();

	UFUNCTION()
	void SequenceToOptionsMenu();

protected:
	virtual void BeginPlay() override;

private:
	void BindEvents();	
};