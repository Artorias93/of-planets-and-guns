#pragma once

#include "CoreMinimal.h"
#include "Characters/OPAG_CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "OPAG_Enemy.generated.h"

class UBehaviorTree;
class AOPAG_PatrolPath;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Enemy : public AOPAG_CharacterBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_Enemy();

	void UpdateSpeed(float const NewSpeed) const;

	void UpdateAcceleration(float const NewAcceleration) const;
	
	virtual void ReceiveDamage(const float Damage, AActor* const Causer) override;

	virtual void Attack();

	virtual void Death() override;

	UBehaviorTree* GetBehaviorTree() const;

	AOPAG_PatrolPath* GetPatrolPath() const;

protected:

	virtual void BeginPlay() override;

	void DropAndDespawn();

	void InitSpawnFX();

	UFUNCTION() void SpawnFXProgress(FVector Value);

	UFUNCTION() void SpawnFXFinished();

	//virtual void GetShootingDirection(FVector& StartLocation, FVector& Direction, float const ShootingAccuracyTolerance /* = 0 */) override;
	
	/////////////////////////////
	//Variables
	/////////////////////////////

private:
	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	class AOPAG_PatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	bool bUsingPatrolPath = false;

	UPROPERTY()
	FTimerHandle RagdollTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float RagdollDurationSeconds = 2.f;

	UPROPERTY(EditAnywhere, Category = "OPAG Loot Drop System", meta = (AllowPrivateAccess = true))
	class UOPAG_LootDropSystemComponent* LootDropSystemComponent;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	UParticleSystemComponent* SpawnParticle;



	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	UMaterialInstance* SpawnMaterial;

	TArray<UMaterial*> NormalMaterials;
	UMaterialInstanceDynamic* ActiveMaterial;

	UPROPERTY()
		UTimelineComponent* SpawnTimeline;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta = (AllowPrivateAccess = true))
		UCurveVector* SpawnFXCurve;

	FOnTimelineVector OnSpawnFXTimelineProgress;
	FOnTimelineEventStatic OnSpawnFXTimelineFinished;
};
