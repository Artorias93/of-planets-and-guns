#include "Characters/Player/OPAG_CheatManager.h"
#include "Characters/Player/OPAG_Player.h"
#include "Weapons/Guns/OPAG_ModularGun.h"

void UOPAG_CheatManager::SetPlayerDamageMultiplier(const float DamageMultiplier)
{
	AOPAG_Player* Pawn = Cast<AOPAG_Player>(GetOuterAPlayerController()->GetPawn());
	if (Pawn != nullptr)
	{
		Pawn->SetDamageMultiplier(DamageMultiplier);
	}
}

void UOPAG_CheatManager::NoobGun()
{
	AOPAG_Player* Player = Cast<AOPAG_Player>(GetOuterAPlayerController()->GetPawn());
	if (Player != nullptr)
	{
		const FOPAG_GunStats NewStats = FOPAG_GunStats(300.f, 500.f, 80.f, 80.f, 4.f, 30);
		Player->GetPrimaryGun()->SetStatsTotal(NewStats);
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
