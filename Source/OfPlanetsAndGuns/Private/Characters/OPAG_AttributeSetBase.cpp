#include "Characters/OPAG_AttributeSetBase.h"
#include "GameplayEffectExtension.h"

UOPAG_AttributeSetBase::UOPAG_AttributeSetBase() : DamageMultiplier(1.0f) {}

void UOPAG_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}