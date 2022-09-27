#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "Engine/DataTable.h"
#include "Managers/RoomManager/OPAG_LootChance.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "OPAG_Vendor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTextRenderComponent;
class UChildActorComponent;
class UOPAG_EventManagerSubsystem;
class UNiagaraSystem;
class UNiagaraComponent;
class UArrowComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Vendor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_Vendor();

	// Pedestal on which guns and modules spawn (upon interaction)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Pedestal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxMoneyTrigger;

	// Positions of the two guns and two modules
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Gun1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Gun2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Module1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Module2;

	// Text displaying prices
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* PriceTextRender_Gun1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* PriceTextRender_Gun2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* PriceTextRender_Module1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* PriceTextRender_Module2;	

	// Particle effects
	//UPROPERTY(Category = "OPAG | Vendor| Particles", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	UNiagaraSystem* NsActivationParticles;
	UPROPERTY(Category = "OPAG | Vendor| Particles", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NcParticlesInteract;
	UPROPERTY(Category = "OPAG | Vendor| Particles", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NcParticlesPool;
	UPROPERTY(Category = "OPAG | Vendor| Particles", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NcParticlesSign;

	// Data tables
	UPROPERTY(Category = "OPAG | Vendor| Tables", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDataTable* DataTableGuns;
	UPROPERTY(Category = "OPAG | Vendor| Tables", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDataTable* DataTableModules;

private:
	UPROPERTY(Category = "OPAG | Vendor| Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MinBare = 0;

	UPROPERTY(Category = "OPAG | Vendor| Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxBare = 3;

	UPROPERTY(Category = "OPAG | Vendor| Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MinPowerful = 2;

	UPROPERTY(Category = "OPAG | Vendor| Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxPowerful = 6;

	UPROPERTY(Category = "OPAG | Vendor| Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DifficultyMultiplierGun = 1.f;
	

private:
	UOPAG_EventManagerSubsystem* EMS;
	TArray<FOPAG_ModuleList*> ModulesList;
	TArray<FOPAG_GunList*> GunsList;
	FOPAG_GunModules Modules;
	FTimerHandle SpawnItemsHandle;
	bool ShowingMerch;

	void PopulateGunAndModuleArray();
	void ParticleEffectInteraction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Unlock(const float Difficulty);

	void SpawnItem(const float RoomDifficulty);
	void AttachModulesToGun(AOPAG_ModularGun& Gun, const bool IsPowerful, const float DifficultyValue);

	UFUNCTION() void DisableGunTextInVendor(AOPAG_ModularGun* Gun);
	UFUNCTION() void DisableModuleTextInVendor(AOPAG_ModuleBase* Module);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};