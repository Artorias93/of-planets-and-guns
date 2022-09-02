#include "Abilities/OPAG_ShieldAbility.h"
#include <UObject/ConstructorHelpers.h>
#include "Characters/Player/OPAG_Player.h"
#include "Abilities/OPAG_ShieldAbilityCooldown.h"


UOPAG_ShieldAbility::UOPAG_ShieldAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	CooldownGameplayEffectClass = UOPAG_ShieldAbilityCooldown::StaticClass();
	const FGameplayTag ShieldTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.shield"));
	AbilityTags.AddTag(ShieldTag);

	static ConstructorHelpers::FClassFinder<AActor> ShieldFinder(TEXT("/Game/FXVarietyPack/Blueprints/BP_ky_healAura"));
	if (ShieldFinder.Succeeded())
	{
		ShieldClass = ShieldFinder.Class;
	}
}

void UOPAG_ShieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
		if (!bResult)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		const FVector PlayerLocation = GetPlayer()->GetActorLocation();
		const FRotator SpawnRotator = FRotator::ZeroRotator;
		AActor* Item = GetWorld()->SpawnActor(ShieldClass, &PlayerLocation, &SpawnRotator);

		GetPlayer()->SetShield(Item);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}