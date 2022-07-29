// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_BasicShot.generated.h"


UCLASS()
class OFPLANETSANDGUNS_API UOPAG_BasicShot : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UOPAG_BasicShot(const FObjectInitializer& ObjectInitializer);
protected:

	UFUNCTION()
	void EventReceived(FGameplayEventData Payload);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
