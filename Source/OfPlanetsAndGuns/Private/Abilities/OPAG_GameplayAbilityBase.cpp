#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "Characters/OPAG_CharacterBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Characters/Enemies/OPAG_Enemy.h"

UOPAG_GameplayAbilityBase::UOPAG_GameplayAbilityBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UOPAG_GameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UOPAG_GameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AOPAG_CharacterBase* UOPAG_GameplayAbilityBase::GetCharacter() const
{
	AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(GetOwningActorFromActorInfo());
	return Character;
}

AOPAG_Player* UOPAG_GameplayAbilityBase::GetPlayer() const
{
	AOPAG_Player* Player = Cast<AOPAG_Player>(GetOwningActorFromActorInfo());
	return Player;
}

AOPAG_Enemy* UOPAG_GameplayAbilityBase::GetEnemy() const
{
	AOPAG_Enemy* Enemy = Cast<AOPAG_Enemy>(GetOwningActorFromActorInfo());
	return Enemy;
}
