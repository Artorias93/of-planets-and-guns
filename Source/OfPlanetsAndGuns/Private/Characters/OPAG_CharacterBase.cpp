#include "Characters/OPAG_CharacterBase.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystemComponent.h>
#include <Abilities/GameplayAbility.h>
#include <GameplayTagsManager.h>
#include <EnhancedInput/Public/EnhancedInputLibrary.h>
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

// Called when the game starts or when spawned
void AOPAG_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComp)
	{
		BaseAttributeSetComp = AbilitySystemComp->GetSet<UOPAG_AttributeSetBase>();

		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSetComp->GetHealthAttribute()).AddUObject(this, &AOPAG_CharacterBase::HealthChanged);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSetComp->GetDamagedAttribute()).AddUObject(this, &AOPAG_CharacterBase::DamageTakenAmountChanged);

		InitializeAbility(BasicShot, 0);
	}
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

void AOPAG_CharacterBase::TakeDamage(float Damage)
{
	if(BaseAttributeSetComp)
	{
		//BaseAttributeSetComp->SetAbilityDamage(Damage);

		//if(AbilitySystemComp)
		//{
		//	//TODO: Change BasicShot name?
		//	AbilitySystemComp->TryActivateAbilityByClass(BasicShot);

		//	const FGameplayTag HitGameplayTag = UGameplayTagsManager::Get().RequestGameplayTag(TEXT("char.ability.ranged.hitEvent"));
		//	FGameplayEventData HitEventData;
		//	HitEventData.Target = this;

		//	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitGameplayTag, HitEventData);
		//	
		//}
	}
}

void AOPAG_CharacterBase::GetHealthValues(float& Health, float& MaxHealth)
{
	Health = BaseAttributeSetComp->GetHealth();
	MaxHealth = BaseAttributeSetComp->GetMaxHealth();
}

void AOPAG_CharacterBase::GetDamagedValues(float& Damaged)
{
	Damaged = BaseAttributeSetComp->GetDamaged();
}

//Called every time the value changes
void AOPAG_CharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

//Called every time the value changes
void AOPAG_CharacterBase::DamageTakenAmountChanged(const FOnAttributeChangeData& Data)
{
	OnDamageTakenAmountChanged(Data.OldValue, Data.NewValue);
}