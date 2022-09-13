#include "UI/OPAG_MainMenuHUD.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "UI/OPAG_UWMainMenuView.h"
#include "UI/OPAG_UWHubView.h"

void AOPAG_MainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AOPAG_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}

	if (MainMenuViewWidgetClass)
	{
		MainMenuViewWidget = CreateWidget<UOPAG_UWMainMenuView>(GetWorld(), MainMenuViewWidgetClass);
	}

	if (HubViewWidgetClass)
	{
		HubViewWidget = CreateWidget<UOPAG_UWHubView>(GetWorld(), HubViewWidgetClass);
	}

	//Main menu sequence names
	const FStringAssetReference StartSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_MenuStart.LS_MenuStart");
	const FStringAssetReference OptionsSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToOptions.LS_ToOptions");
	const FStringAssetReference ExitSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToExit.LS_ToExit");

	//Start sequence initialization
	if(ULevelSequence* const StartLevelSequence = Cast<ULevelSequence>(StartSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings StartSettings;
		ALevelSequenceActor* StartOutActor;
		StartLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			StartLevelSequence, StartSettings, StartOutActor);
		StartLevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::ShowMainMenu);
		
		SequencePlaySelection(Start, MainMenu);
	}

	//Options sequence initialization
	if(ULevelSequence* const OptionsLevelSequence = Cast<ULevelSequence>(OptionsSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings OptionsSettings;
		ALevelSequenceActor* OptionsOutActor;
		OptionsLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			OptionsLevelSequence, OptionsSettings, OptionsOutActor);
		OptionsLevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::ShowOptionsMenu);
	}

	//Exit sequence initialization
	if(ULevelSequence* const ExitLevelSequence = Cast<ULevelSequence>(ExitSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings ExitSettings;
		ALevelSequenceActor* ExitOutActor;
		ExitLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			ExitLevelSequence, ExitSettings, ExitOutActor);
		ExitLevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::ShowExitMenu);
	}
}

void AOPAG_MainMenuHUD::SequencePlaySelection(const EMenuScreen From, const EMenuScreen To) const
{
	switch(From)
	{
		case Start:
			StartLevelSequencePlayer->Play();
			break;
		case MainMenu:
			switch(To)
			{
				case Options:
					OptionsLevelSequencePlayer->Play();
					break;
				case Exit:
					ExitLevelSequencePlayer->Play();
					break;
				default:
					break;
			}			
		default:
			break;
	}	
}

void AOPAG_MainMenuHUD::ShowMainMenu()
{
	if (MainMenuViewWidget)
	{
		MainMenuViewWidget->AddToViewport(1);
	}
}

void AOPAG_MainMenuHUD::HideMainMenu()
{
	if (MainMenuViewWidget)
	{
		MainMenuViewWidget->RemoveFromParent();
	}
}

void AOPAG_MainMenuHUD::ShowOptionsMenu()
{
	
}

void AOPAG_MainMenuHUD::HideOptionsMenu()
{
	
}

void AOPAG_MainMenuHUD::ShowExitMenu()
{

}

void AOPAG_MainMenuHUD::HideExitMenu()
{

}



void AOPAG_MainMenuHUD::ShowHubView()
{
	if (MainMenuViewWidget) {
		MainMenuViewWidget->RemoveFromViewport();
	}

	if (HubViewWidget) {
		HubViewWidget->AddToViewport(1);
	}

	const UOPAG_EventManagerSubsystem* const EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
}

