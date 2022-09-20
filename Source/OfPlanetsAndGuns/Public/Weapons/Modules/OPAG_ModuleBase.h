#pragma once

#include "Weapons/Guns/OPAG_GunBase.h"
#include "CoreMinimal.h"
#include "OPAG_ModuleStats.h"
#include "GameFramework/Actor.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "Components/TimelineComponent.h"
#include "OPAG_ModuleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ModuleBase : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()
	
public:	
	AOPAG_ModuleBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ModuleMesh;

protected:
	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Level = 0;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PriceBase = .0f;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EModuleTypes Type;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EModuleBrand Brand;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FOPAG_GunStats Stats;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EFireTypes"))
	int EquippableFireTypes;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PriceMuliplierPercentage = 1.f;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ModuleDropHighlight;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ModuleDropHighlightParticle;	

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* ModuleDropTimeline;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCurveVector* CurveModuleDropVector;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ModuleDropCurveXFactor = 200;

	UPROPERTY(Category = "OPAG | Module | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ModuleDropCurveZFactor = 100;

	FOnTimelineVector OnModuleDropTimelineProgress;
	
	UPROPERTY()
	FVector ModuleDropTimelinePropertyName;

	UPROPERTY()
	FVector RandomDropDir;

	UPROPERTY()
	FVector SpawnLocation;
	
protected:
	FTimerHandle FlyRotationHandle;
	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Interface implementations
	virtual void Interact_Implementation() override;
	bool CanInteract();
	virtual bool CanInteract_Implementation() override;
	void ApplyForce_Implementation(const FVector Force) override;
	void ApplyFly_Implementation() override;
	void FlyRotationTimer_Implementation() override;
	bool CanBuy_Implementation() override;
	const float GetPriceTotal_Implementation() override;
	FString InteractString_Implementation() override;

	// Attach module to gun
	virtual void OnAttach(const EModuleBrand NewBrand, const FOPAG_GunStats NewStats, const int
	                      NewEquippableFireTypes, const bool bIsAttachedToPlayer);

	// Detach module to gun
	virtual void OnDetach();
private:
	void CheckIfPhysical() const;

public:
	// Getter
	static FString GetRarityName(EModuleRarity ModuleRarity);
	static FString GetTypeName(EModuleTypes ModuleType);

	FString GetModuleName() const;
	int GetLevel() const;
	EModuleTypes GetType() const;
	EModuleRarity GetRarity() const;
	EModuleBrand GetBrand() const;
	FOPAG_GunStats GetStats() const;
	int GetEquippableFireTypes() const;
	void LoadParticleFromRarity(EModuleRarity ModuleRarity);

	//Setter
	void SetParameters(const EModuleBrand NewBrand, const FOPAG_GunStats NewStats,
		const int NewEquippableFireTypes);
	void SetStats(const TArray<FOPAG_SecondaryStatModifier*>& StatModifiers);
	void SetStats(const FOPAG_GunStats NewStats);

	UFUNCTION()
	void ModuleDropTimelineProgress(FVector Value);
	
public:
	FORCEINLINE USphereComponent* GetModuleSphereComponent() { return Sphere; }
	void StartDropTimeline();

};