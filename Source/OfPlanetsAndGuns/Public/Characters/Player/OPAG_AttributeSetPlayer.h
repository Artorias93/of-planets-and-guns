#pragma once

#include "CoreMinimal.h"
#include "Characters/OPAG_AttributeSetBase.h"
#include "OPAG_AttributeSetPlayer.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_AttributeSetPlayer : public UOPAG_AttributeSetBase
{
	GENERATED_BODY()

public:
	UOPAG_AttributeSetPlayer();
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData ShieldCapacity;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, ShieldCapacity)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData ShieldCapacityMax;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, ShieldCapacityMax)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData ShieldRechargeRate;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, ShieldRechargeRate)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData ShieldCooldown;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, ShieldCooldown)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, MovementSpeed)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData DashCount;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, DashCount)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData DashMaxCount;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, DashMaxCount)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttribute")
	FGameplayAttributeData DashRestoreTime;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetPlayer, DashRestoreTime)
};
