#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_DroneBase.generated.h"

class AOPAG_Enemy;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_DroneBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DroneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* FirstShootPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SecondShootPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* BurstParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SpawnParticleSystemComponent;
	
public:	
	AOPAG_DroneBase();

protected:
	UPROPERTY(Category = "OPAG | Drone | Settings", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float VisibilityRange = 10.f;

	UPROPERTY(Category = "OPAG | Drone | Settings", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ProjectileLenght = 1000.f;

	UPROPERTY(Category = "OPAG | Drone | Settings", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float InterpMovementSpeed = 20.f;

	UPROPERTY(Category = "OPAG | Drone | Damage", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Damage = 200.f;

	UPROPERTY(Category = "OPAG | Drone | Damage", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DamageRandomnessPercentage = 10.f;

	UPROPERTY(Category = "OPAG | Drone | Damage", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MultiplierByLevel = 1.f;

	UPROPERTY(Category = "OPAG | Drone | Damage", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RandomShootingAngle = 2.5f;

	UPROPERTY(Category = "OPAG | Drone | Shooting", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ProjectileTrace;

	UPROPERTY(Category = "OPAG | Drone | Emitter", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzlePartile;

	UPROPERTY(Category = "OPAG | Drone | Emitter", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ExplosionParticle;

protected:
	FTimerHandle ShootingHandle;
	FTimerHandle SpawnFinishedHandle;
	bool bIsReverse = false;

protected:
	virtual void BeginPlay() override;
	virtual void FiendEnemisTimer();
	virtual void Shoot(AOPAG_Enemy* Enemy, const FVector TraceStartLocation, const FRotator EffectiveRotation);
	void IdleMovement(float DeltaTime);
	const FVector CalculateEffectiveDirection(const FVector StartLocation, const FVector EnemyLocation) const;

	UFUNCTION() void ShootingEnemies(TArray<AOPAG_Enemy*> EnemiesTarget);
	UFUNCTION() float CalculateDamage() const;
	UFUNCTION() void SpawnFinishedTimer();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void DestroyDrone();
};
