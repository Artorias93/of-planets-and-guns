#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "Weapons/Throwables/OPAG_Grenade.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"

AOPAG_AccessoryBase::AOPAG_AccessoryBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Accessory;
}

void AOPAG_AccessoryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_AccessoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_AccessoryBase::OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	Super::OnAttach(NewRarity, NewStats, NewEquippableFireTypes, bIsAttachedToPlayer);

	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	EMS->ActivePassiveAbilityEffect.Broadcast(PassiveAbilityClass);
}

void AOPAG_AccessoryBase::OnDetach()
{
	Super::OnDetach();

	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	EMS->RemoveAbilityEffect.Broadcast(PassiveAbilityTagContainer);
}

const TSubclassOf<UGameplayAbility> AOPAG_AccessoryBase::GetActiveAbility() const
{
	return ActiveAbilityClass;
}

const TSubclassOf<UGameplayAbility> AOPAG_AccessoryBase::GetPassiveAbility() const
{
	return PassiveAbilityClass;
}