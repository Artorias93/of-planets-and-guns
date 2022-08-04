#include "Abilities/OPAG_LaunchGranade.h"
#include "Weapons/Modules/OPAG_BarrelBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Camera/CameraComponent.h"
#include "Weapons/Guns/OPAG_ModularGun.h"

UOPAG_LaunchGranade::UOPAG_LaunchGranade(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

void UOPAG_LaunchGranade::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
	AOPAG_Player* Player = GetPlayer();
	if (Player)
	{
		const FRotator CameraRotation = Player->CurrentCamera->GetComponentRotation();
		Player->PrimaryGun->ShootThrowable(CameraRotation);
	}
}
