#include "Character\BaseAttributeSet.h"
#include "GameplayEffect.h"				//GAS_Plugin
#include "GameplayEffectExtension.h"	//GAS_Plugin

UBaseAttributeSet::UBaseAttributeSet()
{

}

//GAS_Plugin
void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}

void UBaseAttributeSet::SetTesting(float Damage)
{
	SetDamaged(Damage);
}

void UBaseAttributeSet::SetTestingHealth(float HealthValue)
{
	SetHealth(HealthValue);
}

void UBaseAttributeSet::SetTestingMaxHealth(float HealthValue)
{
	SetMaxHealth(HealthValue);
}