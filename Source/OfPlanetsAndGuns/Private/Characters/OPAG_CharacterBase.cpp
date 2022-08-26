#include "Characters/OPAG_CharacterBase.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystemComponent.h>
#include <Abilities/GameplayAbility.h>
#include <GameplayTagsManager.h>
#include <GameplayTagContainer.h>
//My Classes
#include "Abilities/OPAG_BasicShot.h"
#include "Characters/OPAG_AttributeSetBase.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"



AOPAG_CharacterBase::AOPAG_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	BasicShot = UOPAG_BasicShot::StaticClass();
}

//Binds functionality to input
void AOPAG_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AOPAG_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AOPAG_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	if (AbilitySystemComp)
	{
		BaseAttributeSet = AbilitySystemComp->GetSet<UOPAG_AttributeSetBase>();

		InitializeAbility(BasicShot, 0);
	}
}

float AOPAG_CharacterBase::GetHealth() const
{
	if(AbilitySystemComp)
	{
		return AbilitySystemComp->GetNumericAttribute(BaseAttributeSet->GetHealthAttribute());
	}
	return 1;
}

void AOPAG_CharacterBase::SetHealth(const float Health)
{
	AbilitySystemComp->SetNumericAttributeBase(BaseAttributeSet->GetHealthAttribute(), Health);
}

float AOPAG_CharacterBase::GetHealthMax() const
{
	if (AbilitySystemComp)
	{
		return AbilitySystemComp->GetNumericAttribute(BaseAttributeSet->GetMaxHealthAttribute());
	}
	return 1;
}

void AOPAG_CharacterBase::SetHealthMax(const float HealthMax)
{
	AbilitySystemComp->SetNumericAttributeBase(BaseAttributeSet->GetMaxHealthAttribute(), HealthMax);
}

void AOPAG_CharacterBase::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (AbilitySystemComp)
	{
		if (HasAuthority() && AbilityToGet)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void AOPAG_CharacterBase::ReceiveDamage(const float Damage)
{
	if(AbilitySystemComp && BaseAttributeSet)
	{
		AbilitySystemComp->SetNumericAttributeBase(BaseAttributeSet->GetDamageTakenAmountAttribute(), Damage);

		//TODO: Rename BasicShot in AbilityTakeDamage
		AbilitySystemComp->TryActivateAbilityByClass(BasicShot);

		const FGameplayTag HitGameplayTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.ranged.hitEvent"));
		FGameplayEventData HitEventData = FGameplayEventData();
		HitEventData.Target = this;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitGameplayTag, HitEventData);

		if(!bDead && GetHealth() <= 0)
		{
			bDead = true;
			Death();
		}	
	}
}

void AOPAG_CharacterBase::ReceiveDamage(AActor* Throwable)
{
	//TODO: Implement
}

void AOPAG_CharacterBase::GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance)
{
	//Overridden by children
}

void AOPAG_CharacterBase::Death()
{
	//Overridden by children
}

