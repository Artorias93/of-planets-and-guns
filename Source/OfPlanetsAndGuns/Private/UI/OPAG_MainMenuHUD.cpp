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

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	 if (PlayerController) {
	 	PlayerController->SetInputMode(FInputModeUIOnly());
	 	PlayerController->bShowMouseCursor = true;
	 }

	//Main menu sequence names
	const FStringAssetReference StartSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_MenuStart.LS_MenuStart");
	const FStringAssetReference LevelSelectionSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToLevelSelection.LS_ToLevelSelection");	
	const FStringAssetReference Level1SequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToPlanet1.LS_ToPlanet1");	
	const FStringAssetReference OptionsSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToOptionsView.LS_ToOptionsView");
	const FStringAssetReference ExitSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToExit.LS_ToExit");
	const FStringAssetReference QuitGameSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_QuitGame.LS_QuitGame");
	const FStringAssetReference HowToPlaySequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToHowToPlay.LS_ToHowToPlay");
	const FStringAssetReference CreditsSequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_ToCredits.LS_ToCredits");

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

	//How to play sequence initialization
	if(ULevelSequence* const HowToPlayLevelSequence = Cast<ULevelSequence>(HowToPlaySequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings HowToPlaySettings;
		ALevelSequenceActor* HowToPlayOutActor;
		HowToPlayLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			HowToPlayLevelSequence, HowToPlaySettings, HowToPlayOutActor);
	}

	//Credits sequence initialization
	if(ULevelSequence* const CreditsLevelSequence = Cast<ULevelSequence>(CreditsSequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings CreditsSettings;
		ALevelSequenceActor* CreditsOutActor;
		CreditsLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			CreditsLevelSequence, CreditsSettings, CreditsOutActor);
	}

	//Executes first sequence (menu opening)
	SequencePlaySelection(Start, MainMenu);

	//Delegate binding
	Level1LevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::StartLevel1);
	QuitGameLevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::AbortGame);
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
				case HowToPlay:
					HowToPlayLevelSequencePlayer->Play();
					break;
				case Credits:
					CreditsLevelSequencePlayer->Play();
				default:
					break;
			}
			break;
		case LevelSelection:
			switch(To)
			{
				case Level1:
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
		case HowToPlay:
			switch(To)
			{
				case MainMenu:
					HowToPlayLevelSequencePlayer->PlayReverse();
					break;
				default:
					break;
			}
			break;
		case Credits:
			switch(To)
			{
				case MainMenu:
					CreditsLevelSequencePlayer->PlayReverse();
					break;
				default:
					break;
			}
		default:
			break;
	}
}

void AOPAG_MainMenuHUD::StartLevel1()
{
	const UOPAG_GameInstance* const GameInstance = GetWorld()->GetGameInstance<UOPAG_GameInstance>();
	const UOPAG_EventManagerSubsystem* const EMS =  GameInstance->GetEventManager();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
	
	EMS->StartPlanet.Broadcast(0);
}

void AOPAG_MainMenuHUD::AbortGame()
{
	// const UOPAG_GameInstance* const GameInstance = GetWorld()->GetGameInstance<UOPAG_GameInstance>();
	// const UOPAG_EventManagerSubsystem* const EMS =  GameInstance->GetEventManager();
	// EMS->QuitGame.Broadcast();
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayerController(),	EQuitPreference::Quit, false);
}