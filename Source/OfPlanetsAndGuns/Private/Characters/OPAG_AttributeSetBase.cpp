#include "Characters/OPAG_AttributeSetBase.h"
#include "GameplayEffect.h"				//GAS_Plugin
#include "GameplayEffectExtension.h"	//GAS_Plugin

UOPAG_AttributeSetBase::UOPAG_AttributeSetBase() {}

//GAS_Plugin
void UOPAG_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	/*if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}*/
}

void UOPAG_AttributeSetBase::SetAbilityDamage(float Damage)
{
	SetDamaged(Damage);
}

void UOPAG_AttributeSetBase::SetAbilityHealth(float HealthValue)
{
	SetHealth(HealthValue);
}

void UOPAG_AttributeSetBase::SetAbilityMaxHealth(float HealthValue)
{
	SetMaxHealth(HealthValue);
}