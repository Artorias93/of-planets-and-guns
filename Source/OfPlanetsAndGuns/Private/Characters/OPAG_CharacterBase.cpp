#include "Characters/OPAG_CharacterBase.h"
#include "Character/BaseAttributeSet.h"			//GAS_Plugin

AOPAG_CharacterBase::AOPAG_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");			//GAS_Plugin
}

// Called when the game starts or when spawned
void AOPAG_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComp)
	{
		BaseAttributeSetComp = AbilitySystemComp->GetSet<UBaseAttributeSet>();

		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSetComp->GetHealthAttribute()).AddUObject(this, &AOPAG_CharacterBase::OnHealthChangedNative);			//GAS_Plugin
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSetComp->GetManaAttribute()).AddUObject(this, &AOPAG_CharacterBase::OnManaChangedNative);				//GAS_Plugin
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSetComp->GetStaminaAttribute()).AddUObject(this, &AOPAG_CharacterBase::OnStaminaChangedNative);			//GAS_Plugin
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(BaseAttributeSetComp->GetDamagedAttribute()).AddUObject(this, &AOPAG_CharacterBase::OnDamagedChangedNative);			//GAS_Plugin
	}
}

// Called to bind functionality to input
void AOPAG_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//GAS_Plugin
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

//GAS_Plugin
UAbilitySystemComponent* AOPAG_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

//GAS_Plugin
void AOPAG_CharacterBase::GetHealthValues(float& Health, float& MaxHealth)
{
	Health = BaseAttributeSetComp->GetHealth();
	MaxHealth = BaseAttributeSetComp->GetMaxHealth();
}

//GAS_Plugin
void AOPAG_CharacterBase::GetManaValues(float& Mana, float& MaxMana)
{
	Mana = BaseAttributeSetComp->GetMana();
	MaxMana = BaseAttributeSetComp->GetMaxMana();
}

//GAS_Plugin
void AOPAG_CharacterBase::GetStaminaValues(float& Stamina, float& MaxStamina)
{
	Stamina = BaseAttributeSetComp->GetStamina();
	MaxStamina = BaseAttributeSetComp->GetMaxStamina();
}

//GAS_Plugin
void AOPAG_CharacterBase::GetDamagedValues(float& Damaged)
{
	Damaged = BaseAttributeSetComp->GetDamaged();
}

//GAS_Plugin
void AOPAG_CharacterBase::OnHealthChangedNative(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

//GAS_Plugin
void AOPAG_CharacterBase::OnManaChangedNative(const FOnAttributeChangeData& Data)
{
	OnManaChanged(Data.OldValue, Data.NewValue);
}

//GAS_Plugin
void AOPAG_CharacterBase::OnStaminaChangedNative(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged(Data.OldValue, Data.NewValue);
}

//GAS_Plugin
void AOPAG_CharacterBase::OnDamagedChangedNative(const FOnAttributeChangeData& Data)
{
	OnDamagedChanged(Data.OldValue, Data.NewValue);
}