#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <AbilitySystemInterface.h>

#include "Abilities/GameplayAbility.h"
#include "OPAG_CharacterBase.generated.h"

class UAbilitySystemComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AOPAG_CharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);		

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY()
	class UOPAG_EventManagerSubsystem* EMS;

	UPROPERTY()
	class UOPAG_SoundManagerSubsystem* SMS;
	
	virtual void ReceiveDamage(const float Damage);
	virtual void ReceiveDamage(AActor* Throwable);

	// Ability system
	float GetHealth() const;
	void SetHealth(const float Health);
	float GetHealthMax() const;
	void SetHealthMax(const float HealthMax);

	// Shoot system
	virtual void GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance = 0);

protected:
	virtual void BeginPlay() override;
	virtual void Death();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")						
	class UAbilitySystemComponent* AbilitySystemComp;														
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	const class UOPAG_AttributeSetBase* BaseAttributeSet;										

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	class UInputMappingContext* MappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase")
	TSubclassOf<UGameplayAbility> BasicShot;

public:
	UPROPERTY(Category = "Movement", VisibleAnywhere, BlueprintReadWrite)
		float AxisX;

	UPROPERTY(Category = "Movement", VisibleAnywhere, BlueprintReadWrite)
		float AxisY;
};
