#include "UI/OPAG_UWHubView.h"
#include "UI/OPAG_MainMenuHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWHubView::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWHubView::BindEvents()
{
	UpgradeSystemButton->OnClicked.AddDynamic(this, &UOPAG_UWHubView::UpgradeSystemButtonClicked);
}

void UOPAG_UWHubView::UpgradeSystemButtonClicked()
{
	UpgradeSystemButton->OnClicked.RemoveDynamic(this, &UOPAG_UWHubView::UpgradeSystemButtonClicked);

	AOPAG_MainMenuHUD* MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
		MyHUD->ShowUpgradeSystemView();
}