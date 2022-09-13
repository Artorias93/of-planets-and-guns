#include "UI/OPAG_MainMenuHUD.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "UI/OPAG_UWConfirmExitView.h"
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

	if (ConfirmExitViewWidgetClass)
	{
		ConfirmExitViewWidget = CreateWidget<UOPAG_UWConfirmExitView>(GetWorld(), ConfirmExitViewWidgetClass);
	}

	if (HubViewWidgetClass)
	{
		HubViewWidget = CreateWidget<UOPAG_UWHubView>(GetWorld(), HubViewWidgetClass);
	}

	//Main menu sequence names
	const FStringAssetReference StartSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_MenuStart.LS_MenuStart");
	const FStringAssetReference OptionsSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToOptions.LS_ToOptions");
	const FStringAssetReference ExitSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToExit.LS_ToExit");
	const FStringAssetReference QuitGameSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_QuitGame.LS_QuitGame");

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

	//Quit Game sequence initialization
	if(ULevelSequence* const QuitGameLevelSequence = Cast<ULevelSequence>(QuitGameSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings QuitGameSettings;
		ALevelSequenceActor* QuitGameOutActor;
		QuitGameLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			QuitGameLevelSequence, QuitGameSettings, QuitGameOutActor);
		QuitGameLevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::MenuCloseGame);
	}

	//Reverse Exit sequence initialization
	if(ULevelSequence* const ReverseExitLevelSequence = Cast<ULevelSequence>(ExitSequenceName.TryLoad()))
	{
		FMovieSceneSequencePlaybackSettings ReverseExitSettings;
		ReverseExitSettings.PlayRate = -1.f;
		ALevelSequenceActor* ReverseExitOutActor;
		ReverseExitLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			ReverseExitLevelSequence, ReverseExitSettings, ReverseExitOutActor);
		ReverseExitLevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::ShowMainMenu2);
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
		case Exit:
			switch(To)
			{
				case QuitGame:
					QuitGameLevelSequencePlayer->Play();
					break;
				case MainMenu:
					ReverseExitLevelSequencePlayer->Play();
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

void AOPAG_MainMenuHUD::ShowMainMenu2()
{
	UE_LOG(LogTemp, Warning, TEXT("IN CALLING MAIN MENU TWO"));
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

void AOPAG_MainMenuHUD::ShowExitMenu()
{
	if(ConfirmExitViewWidget)
	{
		ConfirmExitViewWidget->AddToViewport(1);
	}
}

void AOPAG_MainMenuHUD::HideExitMenu()
{
	if (ConfirmExitViewWidget)
	{
		ConfirmExitViewWidget->RemoveFromParent();
	}
}

void AOPAG_MainMenuHUD::MenuCloseGame()
{
	//Closes the application
	UE_LOG(LogTemp, Warning, TEXT("CLOSING THE APPLICATION"));
	//FPlatformMisc::RequestExit(false);
}

void AOPAG_MainMenuHUD::ShowOptionsMenu()
{
	
}

void AOPAG_MainMenuHUD::HideOptionsMenu()
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

