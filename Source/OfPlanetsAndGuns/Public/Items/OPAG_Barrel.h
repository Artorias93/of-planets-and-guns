#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_Barrel.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UMaterialInterface;
class USoundBase;
class UCameraShakeBase;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Barrel : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BarrelMesh;
	
public:	
	AOPAG_Barrel();

private:
	UPROPERTY(Category = "OPAG | Barrel | Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Damage = 100.f;

	UPROPERTY(Category = "OPAG | Barrel | Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Radius = 300.f;

	UPROPERTY(Category = "OPAG | Barrel | Settings", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector ImpactPoint = FVector::ZeroVector;

	UPROPERTY(Category = "OPAG | Barrel | Settings", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> ExplosionShake;

	UPROPERTY(Category = "OPAG | Barrel | Material", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* ExplosionDecal;

	UPROPERTY(Category = "OPAG | Barrel | Emitter", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ExplosionEmitter;

	UPROPERTY(Category = "OPAG | Barrel | Sound", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* ExplosionSound;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Detonate();

private:
	void DamageAllTargetsInRange();
};
