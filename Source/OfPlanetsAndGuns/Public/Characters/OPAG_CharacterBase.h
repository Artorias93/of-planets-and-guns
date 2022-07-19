#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <AbilitySystemInterface.h>

#include "Abilities/GameplayAbility.h"
#include "OPAG_CharacterBase.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

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
	
	virtual void ReceiveDamage(const float Damage);
	virtual void ReceiveDamage(AActor* Throwable);

	const float GetHealth() const;
	void SetHealth(const float Health);

	const float GetHealthMax() const;
	void SetHealthMax(const float HealthMax);

protected:
	virtual void BeginPlay() override;
	virtual void GetShootingDirection(FVector& OutStartLocation, FVector& OutDirection, float ShootingAccuracyTolerance = 0);
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
