// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"			//GAS_Plugin
#include "BaseAttributeSet.generated.h"

//GAS_Plugin
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class OFPLANETSANDGUNS_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()


		//GAS_Plugin
public:

	UBaseAttributeSet();

	//Attributes for Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

		//Attributes for Mana
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Mana)

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana)

		//Attributes for Mana
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina)

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseAttribute")
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina)

		void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
