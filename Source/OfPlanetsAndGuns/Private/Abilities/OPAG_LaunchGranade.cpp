#include "Abilities/OPAG_LaunchGranade.h"
#include "Weapons/Modules/OPAG_BarrelBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Camera/CameraComponent.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Weapons/Modules/SpecializedModules/Accessory/OPAG_AccessoryThrowable.h"
#include "Weapons/Throwables/OPAG_Grenade.h"
#include "Weapons/Throwables/OPAG_Throwable.h"

UOPAG_LaunchGranade::UOPAG_LaunchGranade(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

void UOPAG_LaunchGranade::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		const AOPAG_Player* const Player = GetPlayer();
		const AOPAG_AccessoryThrowable* const Accessory = Cast<AOPAG_AccessoryThrowable>(Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Accessory));
		if (Player == nullptr || Accessory == nullptr)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
			return;
		}

		const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
		if (!bResult || Player == nullptr || Accessory == nullptr)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		const FRotator CameraRotation = Player->GetCurrentPlayerCamera()->GetComponentRotation();
		const TSubclassOf<AOPAG_Throwable> ThrowableClass = Accessory->GetThrowable();
		Player->GetPrimaryGun()->ShootThrowable(ThrowableClass, CameraRotation);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
