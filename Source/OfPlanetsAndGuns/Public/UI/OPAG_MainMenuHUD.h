#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_MainMenuHUD.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;

UENUM()
enum EMenuScreen
{
	Start             UMETA(DisplayName = "Start"),
	MainMenu          UMETA(DisplayName = "MainMenu"),
	LevelSelection    UMETA(DisplayName = "LevelSelection"),
	Level1            UMETA(DisplayName = "Level1"),
	Options           UMETA(DisplayName = "Options"),
	Exit              UMETA(DisplayName = "Exit"),
	QuitGame          UMETA(DisplayName = "QuitGame"),
	HowToPlay         UMETA(DisplayName = "HowToPlay"),
	Credits           UMETA(DisplayName = "Credits")
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MainMenuHUD : public AHUD
{
	GENERATED_BODY()

	//Variables
	
	UPROPERTY()
	ULevelSequencePlayer* StartLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* LevelSelectionLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* Level1LevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* OptionsLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* OptionsToMenuLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* ExitLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* QuitGameLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* HowToPlayLevelSequencePlayer;
	UPROPERTY()
	ULevelSequencePlayer* CreditsLevelSequencePlayer;

	//Functions

public:
	
	virtual void DrawHUD() override;
	
	UFUNCTION()
	void SequencePlaySelection(const EMenuScreen From, const EMenuScreen To) const;

protected:
	
	virtual void BeginPlay() override;

private:
	
	UFUNCTION()
	void StartLevel1();
	UFUNCTION()
	void AbortGame();
};