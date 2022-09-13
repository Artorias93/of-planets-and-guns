#include "Characters/Player/OPAG_CheatManager.h"
#include "Characters/Player/OPAG_Player.h"

void UOPAG_CheatManager::SetPlayerDamageMultiplier(const float DamageMultiplier)
{
	AOPAG_Player* Pawn = Cast<AOPAG_Player>(GetOuterAPlayerController()->GetPawn());
	if (Pawn != nullptr)
	{
		Pawn->SetDamageMultiplier(DamageMultiplier);
	}
}

void UOPAG_CheatManager::God()
{
	AOPAG_Player* Pawn = Cast<AOPAG_Player>(GetOuterAPlayerController()->GetPawn());
	if (Pawn != nullptr)
	{
		Pawn->SetDamageMultiplier(0.0f);
	}
}
