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

protected:
	UPROPERTY(Category = "OPAG | Granade | Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Damage = 100.f;

	UPROPERTY(Category = "OPAG | Granade | Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Radius = 300.f;

	UPROPERTY(Category = "OPAG | Granade | Settings", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector ImpactPoint = FVector::ZeroVector;

	UPROPERTY(Category = "OPAG | Granade | Camera Shake", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShakeBase> ExplosionShake;

	UPROPERTY(Category = "OPAG | Granade | Emitter", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* EmitterTemplate;

private:
	virtual void DamageAllTargetsInRange();

protected:
	virtual void BeginPlay() override;
	virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()	virtual void Detonate();

public:
	float GetDamage() const;
	FVector GetImpactPoint() const;

};