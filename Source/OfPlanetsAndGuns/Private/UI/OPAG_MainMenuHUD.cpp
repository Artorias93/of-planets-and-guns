#include "UI/OPAG_MainMenuHUD.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "UI/OPAG_UWMainMenuView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_GameInstance.h"

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

	//Main menu sequence names
	const FStringAssetReference StartSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_MenuStart.LS_MenuStart");
	const FStringAssetReference LevelSelectionSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToLevelSelection.LS_ToLevelSelection");	
	const FStringAssetReference Level1SequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToPlanet1.LS_ToPlanet1");	
	const FStringAssetReference OptionsSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToOptionsView.LS_ToOptionsView");
	const FStringAssetReference OptionsToMenuSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_OptionsToMenu.LS_OptionsToMenu");
	const FStringAssetReference ExitSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToExit.LS_ToExit");
	const FStringAssetReference QuitGameSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_QuitGame.LS_QuitGame");

	//Start sequence initialization
	if(ULevelSequence* const StartLevelSequence = Cast<ULevelSequence>(StartSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings StartSettings;
		ALevelSequenceActor* StartOutActor;
		StartLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			StartLevelSequence, StartSettings, StartOutActor);
	}

	//Level Selection sequence initialization
	if(ULevelSequence* const LevelSelectionLevelSequence = Cast<ULevelSequence>(LevelSelectionSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings LevelSelectionSettings;
		ALevelSequenceActor* LevelSelectionOutActor;
		LevelSelectionLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			LevelSelectionLevelSequence, LevelSelectionSettings, LevelSelectionOutActor);
	}

	//Level 1 sequence initialization
	if(ULevelSequence* const Level1LevelSequence = Cast<ULevelSequence>(Level1SequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings Level1Settings;
		ALevelSequenceActor* Level1OutActor;
		Level1LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			Level1LevelSequence, Level1Settings, Level1OutActor);
	}

	//Options sequence initialization
	if(ULevelSequence* const OptionsLevelSequence = Cast<ULevelSequence>(OptionsSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings OptionsSettings;
		ALevelSequenceActor* OptionsOutActor;
		OptionsLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			OptionsLevelSequence, OptionsSettings, OptionsOutActor);
	}

	//Options to menu sequence initialization
	if(ULevelSequence* const OptionsToMenuLevelSequence = Cast<ULevelSequence>(OptionsToMenuSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings OptionsToMenuSettings;
		ALevelSequenceActor* OptionsToMenuOutActor;
		OptionsToMenuLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			OptionsToMenuLevelSequence, OptionsToMenuSettings, OptionsToMenuOutActor);
	}

	//Exit sequence initialization
	if(ULevelSequence* const ExitLevelSequence = Cast<ULevelSequence>(ExitSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings ExitSettings;
		ALevelSequenceActor* ExitOutActor;
		ExitLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			ExitLevelSequence, ExitSettings, ExitOutActor);
	}

	//Quit Game sequence initialization
	if(ULevelSequence* const QuitGameLevelSequence = Cast<ULevelSequence>(QuitGameSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings QuitGameSettings;
		ALevelSequenceActor* QuitGameOutActor;
		QuitGameLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			QuitGameLevelSequence, QuitGameSettings, QuitGameOutActor);
	}

	//Executes first sequence (menu opening)
	SequencePlaySelection(Start, MainMenu);

	//Events binding
	Level1LevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::StartLevel1);
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
				case LevelSelection:
					LevelSelectionLevelSequencePlayer->Play();
					break;
				case Options:
					OptionsLevelSequencePlayer->Play();
					break;
				case Exit:
					ExitLevelSequencePlayer->Play();
					break;
				default:
					break;
			}
			break;
		case LevelSelection:
			switch(To)
			{
				case Level1:
					UE_LOG(LogTemp, Warning, TEXT("InLevel1Selection"));
					Level1LevelSequencePlayer->Play();
					break;
				case MainMenu:
					LevelSelectionLevelSequencePlayer->PlayReverse();
					break;
				default:
					break;
			}
			break;			
		case Options:
			switch(To)
			{
				case MainMenu:
					OptionsLevelSequencePlayer->PlayReverse();
					break;
				default:
					break;
			}
			break;
		case Exit:
			switch(To)
			{
				case QuitGame:
					QuitGameLevelSequencePlayer->Play();
					break;
				case MainMenu:
					ExitLevelSequencePlayer->PlayReverse();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void AOPAG_MainMenuHUD::StartLevel1()
{
	const UOPAG_GameInstance* const GameInstance = GetWorld()->GetGameInstance<UOPAG_GameInstance>();
	const UOPAG_EventManagerSubsystem* const EMS =  GameInstance->GetEventManager();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
	EMS->StartPlanet.Broadcast(0);
}

//TODO: EDITOR FUNCTIONS, TO BE DELETED
void AOPAG_MainMenuHUD::GoFromMainMenuToLevelSelection()
{
	SequencePlaySelection(MainMenu, LevelSelection);
}

void AOPAG_MainMenuHUD::GoFromLevelSelectionToMainMenu()
{
	SequencePlaySelection(LevelSelection, MainMenu);
}
void AOPAG_MainMenuHUD::GoFromMainMenuToExit()
{
	SequencePlaySelection(MainMenu, Exit);
}

void AOPAG_MainMenuHUD::GoFromExitToMainMenu()
{
	SequencePlaySelection(Exit, MainMenu);
}

void AOPAG_MainMenuHUD::GoFromMainMenuToOptions()
{
	SequencePlaySelection(MainMenu, Options);
}

void AOPAG_MainMenuHUD::GoFromOptionsToMainMenu()
{
	SequencePlaySelection(Options, MainMenu);
}