#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"			//GAS_Plugin
#include "AbilitySystemInterface.h"			//GAS_Plugin
#include "Abilities/GameplayAbility.h"		//GAS_Plugin
#include <EnhancedInput/Public/EnhancedInputLibrary.h>
#include "OPAG_CharacterBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_CharacterBase : public ACharacter, public IAbilitySystemInterface		//GAS_Plugin (last element)
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

protected:

	virtual void BeginPlay() override;

public:

	AOPAG_CharacterBase();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")										//GAS_Plugin
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);			//GAS_Plugin	

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;								//GAS_Plugin

	UFUNCTION(BlueprintPure, Category = "CharacterBase")											//GAS_Plugin
	void GetHealthValues(float& Health, float& MaxHealth);											//GAS_Plugin
	
	UFUNCTION(BlueprintPure, Category = "CharacterBase")											//GAS_Plugin
	void GetManaValues(float& Mana, float& MaxMana);												//GAS_Plugin

	UFUNCTION(BlueprintPure, Category = "CharacterBase")											//GAS_Plugin
	void GetStaminaValues(float& Stamina, float& MaxStamina);										//GAS_Plugin
	
	UFUNCTION(BlueprintPure, Category = "CharacterBase")											//GAS_Plugin
	void GetDamagedValues(float& Damaged);															//GAS_Plugin

	void OnHealthChangedNative(const FOnAttributeChangeData& Data);									//GAS_Plugin

	void OnManaChangedNative(const FOnAttributeChangeData& Data);									//GAS_Plugin

	void OnStaminaChangedNative(const FOnAttributeChangeData& Data);								//GAS_Plugin

	void OnDamagedChangedNative(const FOnAttributeChangeData& Data);								//GAS_Plugin

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")								//GAS_Plugin
	void OnHealthChanged(float OldValue, float NewValue);											//GAS_Plugin

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")								//GAS_Plugin
	void OnManaChanged(float OldValue, float NewValue);												//GAS_Plugin

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")								//GAS_Plugin
	void OnStaminaChanged(float OldValue, float NewValue);											//GAS_Plugin

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")								//GAS_Plugin
	void OnDamagedChanged(float OldValue, float NewValue);

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")						//GAS_Plugin
	UAbilitySystemComponent* AbilitySystemComp;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")						//GAS_Plugin
	const class UBaseAttributeSet* BaseAttributeSetComp;											//GAS_Plugin

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UInputMappingContext* MappingContext;
};
