#include "UI/OPAG_UWLoadingScreenView.h"
#include "Components/ProgressBar.h"

void UOPAG_UWLoadingScreenView::SetLoadingBar(float Percentage)
{
	if(LoadingBar && (Percentage >= 0.0f && Percentage <= 1.0f))
		LoadingBar->SetPercent(Percentage);
}