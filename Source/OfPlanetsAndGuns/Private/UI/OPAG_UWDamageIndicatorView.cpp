#include "UI/OPAG_UWDamageIndicatorView.h"

#include "Characters/Player/OPAG_Player.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UOPAG_UWDamageIndicatorView::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Player);
}

AActor* UOPAG_UWDamageIndicatorView::GetCurrentThreatActor() const
{
	return CurrentThreatActor;
}

void UOPAG_UWDamageIndicatorView::SetCurrentThreatActor(AActor* const Causer)
{
	CurrentThreatActor = Causer;
}

AOPAG_Player* UOPAG_UWDamageIndicatorView::GetPlayer() const
{
	return Player;
}