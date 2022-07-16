#include "Characters/OPAG_CharacterBase.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystemComponent.h>
#include <Abilities/GameplayAbility.h>
#include <GameplayTagsManager.h>
#include <GameplayTagContainer.h>
//My Classes
#include "Characters/OPAG_AttributeSetBase.h"

AOPAG_CharacterBase::AOPAG_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");

	static ConstructorHelpers::FClassFinder<UGameplayAbility> BasicShotClass(TEXT("/Game/OfPlanetsAndGuns/Core/Abilities/GA_BasicShot"));
	if (BasicShotClass.Succeeded())
	{
		BasicShot = BasicShotClass.Class;
	}
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

	if (AbilitySystemComp)
	{
		BaseAttributeSet = AbilitySystemComp->GetSet<UOPAG_AttributeSetBase>();

		if (BaseAttributeSet)
		{
			/*AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
				BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &AOPAG_CharacterBase::HealthChanged);*/
		}

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

void AOPAG_CharacterBase::SetHealth(const float Health) const
{
	AbilitySystemComp->SetNumericAttributeBase(BaseAttributeSet->GetHealthAttribute(), Health);
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

void AOPAG_CharacterBase::TakeDamage(const float Damage)
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

		if(GetHealth()<=0)
		{
			Death();
		}	
	}
}

void AOPAG_CharacterBase::TakeDamage(AActor* Throwable)
{
	//TODO: Implement
}

void AOPAG_CharacterBase::GetShootingDirection(FVector& StartingLocation, FVector& Direction, float ShootingAccuracyTolerance)
{
	//Overridden by children
}

void AOPAG_CharacterBase::Death()
{
	//Overridden by children
}

//Called every time the value changes
//void AOPAG_CharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
//{
//	OnHealthChanged(Data.OldValue, Data.NewValue);
//}