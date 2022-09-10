#pragma once

#include "GameFramework/CheatManager.h"
#include "OPAG_CheatManager.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_CheatManager : public UCheatManager
{
	GENERATED_BODY()

	UFUNCTION(Exec, BlueprintCallable, Category = "Cheat Manager")
	void SetPlayerDamageMultiplier(const float DamageMultiplier);

	virtual void God() override;
};