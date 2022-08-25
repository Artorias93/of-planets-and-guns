#include "Characters/Player/OPAG_AttributeSetPlayer.h"
#include "GameplayEffectExtension.h"

UOPAG_AttributeSetPlayer::UOPAG_AttributeSetPlayer() {}

void UOPAG_AttributeSetPlayer::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetShieldCapacityAttribute())
	{
		SetShieldCapacity(FMath::Clamp(GetShieldCapacity(), 0.0f, GetShieldCapacityMax()));
	}
}