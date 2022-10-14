#include "Abilities/OPAG_SpawnDroneAbility.h"
#include "Weapons/Modules/SpecializedModules/Accessory/OPAG_AccessoryDrone.h"
#include "Characters/Player/OPAG_Player.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "Weapons/Drones/OPAG_DroneBase.h"
#include "Abilities/OPAG_DroneCooldown.h"

UOPAG_SpawnDroneAbility::UOPAG_SpawnDroneAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	CooldownGameplayEffectClass = UOPAG_DroneCooldown::StaticClass();
	const FGameplayTag DroneTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.drone"));
	AbilityTags.AddTag(DroneTag);
}

void UOPAG_SpawnDroneAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		AOPAG_Player* Player = GetPlayer();
		const AOPAG_AccessoryDrone* const Accessory = Cast<AOPAG_AccessoryDrone>(Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Accessory));
		const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
		if (Player == nullptr || Accessory == nullptr || !bResult)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		TSubclassOf<AOPAG_DroneBase> DroneClass = Accessory->GetDroneClass();
		float TimeBeforeDestroy = Accessory->GetTimeBeforeDestroyDrone();
		if (DroneClass != nullptr)
		{
			Player->SpawnDrone(DroneClass, TimeBeforeDestroy);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UOPAG_SpawnDroneAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if(CooldownGE == nullptr)	return;

	AOPAG_Player* Player = GetPlayer();
	const AOPAG_AccessoryDrone* const Accessory = Cast<AOPAG_AccessoryDrone>(Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Accessory));
	if(Player == nullptr || Accessory == nullptr)	return;

	// Make spec with Cooldown
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	FGameplayTagContainer DroneGrantedTagsContainer;
	const FGameplayTag DroneCooldownTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.drone.cooldown"));
	DroneGrantedTagsContainer.AddTag(DroneCooldownTag);
	SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(DroneGrantedTagsContainer);
	
	// Make ScalableFloat with time remaining 
	TSubclassOf<AOPAG_DroneBase> DroneClass = Accessory->GetDroneClass();
	float TimeBeforeDestroy = Accessory->GetTimeBeforeDestroyDrone();
	FScalableFloat DroneScalableFloat;
	DroneScalableFloat.SetValue(TimeBeforeDestroy);

	// Apply new duration to Cooldown
	SpecHandle.Data.Get()->SetDuration(TimeBeforeDestroy, true);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}