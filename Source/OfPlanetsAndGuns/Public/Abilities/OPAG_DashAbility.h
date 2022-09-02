#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_DashAbility.generated.h"

class AOPAG_Player;
class UCharacterMovementComponent;
class UOPAG_DashCooldown;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_DashAbility : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()
public:
	UOPAG_DashAbility(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category = "DashAbility", VisibleAnywhere, BlueprintReadWrite)
		AOPAG_Player* Player;

	UPROPERTY(Category = "DashAbility", VisibleAnywhere, BlueprintReadWrite)
		float Friction = 500.f;

	UPROPERTY(Category = "DashAbility", VisibleAnywhere, BlueprintReadWrite)
		FVector LaunchDirection = FVector::ZeroVector;

	UPROPERTY(Category = "DashAbility", VisibleAnywhere, BlueprintReadWrite)
		FVector InitialLocation = FVector::ZeroVector;

	UPROPERTY(Category = "DashAbility", VisibleAnywhere, BlueprintReadWrite)
		FTimerHandle DashHandle;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION() void LaunchUpAndThenStrong();
	UFUNCTION() void LaunchStrong();
	UFUNCTION() const FVector CalculateDirection() const;
	UFUNCTION() const FVector CalculateForce() const;
	// OLD
	//UFUNCTION() void StopDash();
	//UFUNCTION() void ApplicateFriction();
};
