#pragma once

#include "CoreMinimal.h"
#include "Weapons/Throwables/OPAG_Throwable.h"
#include "OPAG_Grenade.generated.h"

class UGameplayStatics;
class UParticleSystem;
class USoundBase;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Grenade : public AOPAG_Throwable
{
	GENERATED_BODY()
	
public:	
	AOPAG_Grenade();

	UPROPERTY()
		class UOPAG_SoundManagerSubsystem* SMS;


public:
	UPROPERTY(Category = "Grenade", EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* EmitterTemplate;

	UPROPERTY(Category = "Sound", EditDefaultsOnly, BlueprintReadWrite)
		USoundBase* ExplosionSound;

	UPROPERTY(Category = "Grenade", EditAnywhere, BlueprintReadWrite)
		float Damage = 100.f;

	UPROPERTY(Category = "Grenade", EditAnywhere, BlueprintReadWrite)
		float Radius = 300.f;

	UPROPERTY(Category = "Grenade", EditAnywhere, BlueprintReadOnly)
		FVector ImpactPoint = FVector::ZeroVector;

private:
	virtual void DamageAllTargetsInRange();

protected:
	virtual void BeginPlay() override;
	virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()	virtual void Detonate();

};