#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_Grenade.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UGameplayStatics;
class UParticleSystem;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Grenade : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_Grenade();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* Projectile;

public:
	UPROPERTY(Category = "Grenade", EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* EmitterTemplate;

	UPROPERTY(Category = "Grenade", EditAnywhere, BlueprintReadOnly)
		float Damage = 0;

	UPROPERTY(Category = "Grenade", EditAnywhere, BlueprintReadOnly)
		FVector ImpactPoint = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	virtual void Tick(float DeltaTime) override;

};