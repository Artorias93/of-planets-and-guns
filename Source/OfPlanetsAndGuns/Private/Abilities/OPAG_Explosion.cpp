// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_Explosion.h"
#include "Weapons/Throwables/OPAG_Grenade.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"

void UOPAG_Explosion::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);

	AOPAG_Grenade* Grenade = Cast<AOPAG_Grenade>(GetOwningActorFromActorInfo());

	if (Grenade)
	{
		const FVector ImpactPoint = Grenade->GetImpactPoint();
		const TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> OutHits;

		const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			ImpactPoint,
			ImpactPoint,
			500.f,
			ETraceTypeQuery::TraceTypeQuery3,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutHits,
			true
		);

		for (int i = 0; i < OutHits.Num(); i++)
		{
			AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHits[i].GetActor());
			
			if (Character)
			{
				const float Damage = Grenade->GetDamage();
				Character->ReceiveDamage(Damage, Grenade);
			}
		}
	}
}
