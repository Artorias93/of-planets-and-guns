#pragma once

#include "CoreMinimal.h"
#include "Characters/OPAG_CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "OPAG_Enemy.generated.h"

class USphereComponent;
class UBehaviorTree;
class AOPAG_PatrolPath;
class UWidgetComponent;
class UOPAG_EnemyHealthBar;

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
	
	virtual void ReceiveDamage(const float Damage, AActor* const Causer, bool bCritic) override;

	virtual void Attack();

	virtual void Death() override;

	UBehaviorTree* GetBehaviorTree() const;

	AOPAG_PatrolPath* GetPatrolPath() const;

	virtual void SetHealthBarVisible();

	UFUNCTION(BlueprintCallable)
	bool GetIsJumping() const;

	UFUNCTION(BlueprintCallable)
	void SetIsJumping(const bool Value);

	UFUNCTION(BlueprintCallable)
	void TurnOffJumpingAfterSeconds(const float JumpDuration, const float AnimFraction);

	UFUNCTION(BlueprintCallable)
	void TurnOnBoosters() const;

	UFUNCTION(BlueprintCallable)
	void TurnOffBoosters() const;

	UFUNCTION(BlueprintCallable)
	AActor* GetFocusedActor() const;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Despawn();

	void InitSpawnFX();

	void AddRandomnessToDamage(float& Damage);

	void AddCritToDamage(float& Damage);

	UFUNCTION() void SpawnFXProgress(FVector Value);

	UFUNCTION() void SpawnFXFinished();

	UFUNCTION() void ResetHealthBarHidden();

	UFUNCTION() void SpawnTextDamage(float Damage, bool bCrit);

	//virtual void GetShootingDirection(FVector& StartLocation, FVector& Direction, float const ShootingAccuracyTolerance /* = 0 */) override;
	
	/////////////////////////////
	//Variables
	/////////////////////////////
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (AllowPrivateAccess = true))
	bool bIsJumping;
	
	FTimerHandle JumpingTimerHandle;
	FTimerDelegate JumpingDelegate;
	
	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	AOPAG_PatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	bool bUsingPatrolPath = false;

	UPROPERTY()
	FTimerHandle RagdollTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float RagdollDurationSeconds = 2.f;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta = (AllowPrivateAccess = true))
	USphereComponent* HeadCollider;

	UPROPERTY(EditAnywhere, Category = "OPAG Loot Drop System", meta = (AllowPrivateAccess = true))
	class UOPAG_LootDropSystemComponent* LootDropSystemComponent;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	UParticleSystemComponent* SpawnParticle;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BoosterParticleComponent1;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BoosterParticleComponent2;
	
	UPROPERTY(EditAnywhere, Category = "OPAG UI", meta = (AllowPrivateAccess = true))
	UWidgetComponent* EnemyHealthBarComponent;

	UPROPERTY(EditAnywhere, Category = "OPAG UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> EnemyHealthBarClass;



	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	UMaterialInstance* SpawnMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "OPAG AI")
	TSubclassOf<class AOPAG_TextDamage> TextDamageClass;

	TArray<UMaterial*> NormalMaterials;
	UMaterialInstanceDynamic* ActiveMaterial;
	UOPAG_EnemyHealthBar* EnemyHealthBar;


	UPROPERTY()
	UTimelineComponent* SpawnTimeline;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta = (AllowPrivateAccess = true))
	UCurveVector* SpawnFXCurve;

	FOnTimelineVector OnSpawnFXTimelineProgress;
	FOnTimelineEventStatic OnSpawnFXTimelineFinished;
	FTimerHandle UnseenByPlayerHandle;

};
