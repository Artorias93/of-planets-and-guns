#include "UI/OPAG_PlayerStatus.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Components/Image.h"

void UOPAG_PlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();

	BindEvents();
}

void UOPAG_PlayerStatus::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	if(EMS == nullptr)	return;

	EMS->UpdateShieldDamage.AddDynamic(this, &UOPAG_PlayerStatus::UpdateShiedlDamage);
}

void UOPAG_PlayerStatus::UpdateShiedlDamage(const float OpacityAmount)
{
	// Stop last timer
	GetWorld()->GetTimerManager().ClearTimer(ClearShieldDamageHanlde);

	// Update opacity value and start new timer for clear
	FLinearColor NewShieldDamageColor = OpacityAmount < 1.f ? FLinearColor(.01096f, .533277f, .752942f, OpacityAmount) :
		FLinearColor(.752942f, .064938f, .030472f, OpacityAmount);
	ShieldDamageImage->SetColorAndOpacity(NewShieldDamageColor);
	GetWorld()->GetTimerManager().SetTimer(ClearShieldDamageHanlde, this, &UOPAG_PlayerStatus::ClearShieldDamageTimer, StartClearTimerAmount, false);
}

void UOPAG_PlayerStatus::ClearShieldDamageTimer()
{
	FLinearColor CurrentShieldOpacity = ShieldDamageImage->ColorAndOpacity;
	CurrentShieldOpacity.A = FMath::Clamp(CurrentShieldOpacity.A - ClearIntepAmount, 0.f, 1.f);

	ShieldDamageImage->SetColorAndOpacity(CurrentShieldOpacity);

	if (CurrentShieldOpacity.A > .0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ClearShieldDamageHanlde, this, &UOPAG_PlayerStatus::ClearShieldDamageTimer, ClearIntepAmount, false);
	}
}