#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OPAG_GameplayAbilityBase.generated.h"

class OPAG_CharacterBase;
class OPAG_Player;
class OPAG_Enemy;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UOPAG_GameplayAbilityBase(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION() AOPAG_CharacterBase* GetCharacter() const;
	UFUNCTION() AOPAG_Player* GetPlayer() const;
	UFUNCTION() AOPAG_Enemy* GetEnemy() const;

};
