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
	
	
	const FStringAssetReference SequenceName("/Game/OfPlanetsAndGuns/UI/Sequences/LS_MenuStart.LS_MenuStart");
	if(ULevelSequence* const LevelSequence = Cast<ULevelSequence>(SequenceName.TryLoad()))
	{
		const FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* OutActor;
		ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			LevelSequence, Settings, OutActor);
		LevelSequencePlayer->Play();
		LevelSequencePlayer->OnStop.AddUniqueDynamic(this, &AOPAG_MainMenuHUD::ShowMainMenu);
	}
}

void AOPAG_MainMenuHUD::ShowMainMenu()
{
	if (MainMenuViewWidget)
	{
		MainMenuViewWidget->AddToViewport(1);
	}
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