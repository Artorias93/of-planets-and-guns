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

	// Positions of the two guns and three modules
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Gun1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Gun2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Module1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* ChildActor_Module2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* PriceTextRender_Gun1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* PriceTextRender_Gun2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* PriceTextRender_Module1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* PriceTextRender_Module2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDataTable* DataTableGuns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDataTable* DataTableModules;

public:

private:
	UOPAG_EventManagerSubsystem* EMS;
	TArray<FOPAG_ModuleList*> ModulesList;
	TArray<FOPAG_GunList*> GunsList;
	FOPAG_GunModules Modules;
	FTimerHandle SpawnItemsHandle;

	void PopulateGunAndModuleArray();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Unlock();

	void SpawnItem(const float RoomDifficulty);
	void AttachModulesToGun(AOPAG_ModularGun& Gun, const bool IsPowerful, const float DifficultyValue);

public:
	UFUNCTION() void DisableGunTextInVendor(AOPAG_ModularGun* Gun);
	UFUNCTION() void DisableModuleTextInVendor(AOPAG_ModuleBase* Module);

};