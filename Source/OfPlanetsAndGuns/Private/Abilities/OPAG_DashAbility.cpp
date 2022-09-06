#include "Abilities/OPAG_DashAbility.h"
#include "Characters/Player/OPAG_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/OPAG_DashCooldown.h"

UOPAG_DashAbility::UOPAG_DashAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CooldownGameplayEffectClass = UOPAG_DashCooldown::StaticClass();
	const FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.movement.dash"));
	AbilityTags.AddTag(DashTag);
}

void UOPAG_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
										const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
	Player = GetPlayer();
	if (Player)
	{
		if (bResult)
		{
			// Gives small upwards force, after delay strong force
			LaunchUpAndThenForward();
		}
		else
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		}
	}
}

void UOPAG_DashAbility::LaunchUpAndThenForward()
{
	const FVector Force(0.f, 0.f, 2.f);
	Player->LaunchCharacter(Force, false, false);
	GetWorld()->GetTimerManager().SetTimer(DashHandle, this, &UOPAG_DashAbility::LaunchStrong, 0.1f, false);
}

void UOPAG_DashAbility::LaunchStrong()
{
	const FVector Force = CalculateForce();
	Player->LaunchCharacter(Force, true, true);
}

void UOPAG_DashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
								   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

}

const FVector UOPAG_DashAbility::CalculateForce() const
{
	const FVector Direction = CalculateDirection();
	const FVector Force = Direction * Player->GetDashSpeed();
	return Force;
}

const FVector UOPAG_DashAbility::CalculateDirection() const
{
	FVector CurrentVelocity = Player->GetVelocity();
	if (CurrentVelocity.IsNearlyZero(0.0001f))
		return Player->GetActorForwardVector();

	const float AxisX = Player->AxisX;
	const float AxisY = Player->AxisY;
	const FVector Direction = (Player->GetActorForwardVector() * AxisY) + (Player->GetActorRightVector() * AxisX);

	return Direction.GetSafeNormal();
}


//	OLD DASH IMPLEMENTATION:
//	OLD DASH SPEED: 10000.0f
// 
//void UOPAG_DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
// const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	//ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
//	Player = GetPlayer();
//	if (Player)
//	{
//		if (bResult)
//		{
//			InitialLocation = Player->GetActorLocation();
//			const FVector Force = CalculateForce();
//			Player->LaunchCharacter(Force, true, true);
//
//			StopDash();
//		}
//		else
//		{
//			StopDash();
//			CancelAbility(Handle, ActorInfo, ActivationInfo, false);
//		}
//	}
//}
//
// 
//void UOPAG_DashAbility::StopDash()
//{
//	const float LocationLenght = (Player->GetActorLocation() - InitialLocation).Size();
//	if (LocationLenght > Player->GetDashLenght())
//	{
//		ApplicateFriction();
//	}
//	else
//	{
//		GetWorld()->GetTimerManager().SetTimer(DashHandle, this, &UOPAG_DashAbility::StopDash, 0.1f, false);
//	}
//}
//
// 
//void UOPAG_DashAbility::ApplicateFriction()
//{
//	const FVector CurrentVelocity = Player->GetVelocity();
//	const FVector NewVelocity = CurrentVelocity / Friction;
//	Player->GetCharacterMovement()->Velocity = NewVelocity;
//}
