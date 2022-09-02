#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include <GameplayTagContainer.h>
#include "OPAG_AccessoryBase.generated.h"

class UGameplayAbility;
class AOPAG_Grenade;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AccessoryBase : public AOPAG_ModuleBase
{
	GENERATED_BODY()
	
public:	
	AOPAG_AccessoryBase();

protected:
	UPROPERTY(Category = "Accessory Ability", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	const TSubclassOf<UGameplayAbility> ActiveAbilityClass;

	UPROPERTY(Category = "Accessory Ability", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	const TSubclassOf<UGameplayAbility> PassiveAbilityClass;

	UPROPERTY(Category = "Accessory Ability", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer PassiveAbilityTagContainer;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats,
		const int NewEquippableFireTypes, const bool bIsAttachedToPlayer) override;
	virtual void OnDetach() override;

public:
	const TSubclassOf<UGameplayAbility> GetActiveAbility() const;
	const TSubclassOf<UGameplayAbility> GetPassiveAbility() const;

};
