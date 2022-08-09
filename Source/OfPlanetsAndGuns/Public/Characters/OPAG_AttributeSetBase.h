#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OPAG_AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_AttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_AttributeSetBase();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	//Attributes for Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetBase, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetBase, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
	FGameplayAttributeData DamageTakenAmount;
	ATTRIBUTE_ACCESSORS(UOPAG_AttributeSetBase, DamageTakenAmount)
};