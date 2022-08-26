#include "UI/OPAG_EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UOPAG_EnemyHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

UProgressBar* UOPAG_EnemyHealthBar::GetProgressBar()
{
	return EnemyProgressBar;
}

void UOPAG_EnemyHealthBar::SetHelthPercent(const float Value)
{
	EnemyProgressBar->SetPercent(Value);
}
