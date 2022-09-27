#include "Abilities/OPAG_SpawnDroneAbility.h"
#include "Weapons/Modules/SpecializedModules/Accessory/OPAG_AccessoryDrone.h"
#include "Characters/Player/OPAG_Player.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "Weapons/Drones/OPAG_DroneBase.h"

UOPAG_SpawnDroneAbility::UOPAG_SpawnDroneAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	/*CooldownGameplayEffectClass = UOPAG_HealthPotionCooldown::StaticClass();
	const FGameplayTag PotionTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.items.potion"));
	AbilityTags.AddTag(PotionTag);*/
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
			Player->SetDrone(DroneClass, TimeBeforeDestroy);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}