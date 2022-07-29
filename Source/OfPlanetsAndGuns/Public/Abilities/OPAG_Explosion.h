// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_Explosion.generated.h"

class AOPAG_Grenade;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_Explosion : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
