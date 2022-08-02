// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_HealthPotion : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
