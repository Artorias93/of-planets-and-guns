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

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Vendor : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()

	bool bIsInteractable = true;
	
public:	

	AOPAG_Vendor();

	// Pedestal on which guns and modules spawn (upon interaction)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Pedestal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;

	// Positions of the two guns and three modules
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MerchSpawnPoint_Gun1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MerchSpawnPoint_Gun2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MerchSpawnPoint_Module1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MerchSpawnPoint_Module2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDataTable* DataTableGuns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDataTable* DataTableModules;

public:
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadWrite)
	bool bIsInfinitelyInteractable = false;

private:
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool CanInteract();
	virtual bool CanInteract_Implementation() override;

	void SpawnItem(const float RoomDifficulty);

	void AttachModulesToGun(AOPAG_ModularGun& Gun, const bool IsPowerful, const float DifficultyValue);

};
