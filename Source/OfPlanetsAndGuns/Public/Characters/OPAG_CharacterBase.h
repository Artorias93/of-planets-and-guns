#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <AbilitySystemInterface.h>
#include "OPAG_CharacterBase.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_CharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);		

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	void GetHealthValues(float& Health, float& MaxHealth);
	
	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	void GetDamagedValues(float& Damaged);

	void HealthChanged(const FOnAttributeChangeData& Data);	

	void DamageTakenAmountChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
	void OnHealthChanged(float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
	void OnDamageTakenAmountChanged(float OldValue, float NewValue);

	void TakeDamage(float Damage);

protected:

	virtual void BeginPlay() override;

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")						
	class UAbilitySystemComponent* AbilitySystemComp;														
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	const class UOPAG_AttributeSetBase* BaseAttributeSetComp;										

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	class UInputMappingContext* MappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase")
	TSubclassOf<UGameplayAbility> BasicShot;
};
