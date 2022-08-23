#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "Engine/DataTable.h"
#include "OPAG_Vendor.generated.h"


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

private:
	TArray<TSubclassOf<AActor>> GunsList;
	TArray<TSubclassOf<AActor>> ModulesList;
	FOPAG_GunModules Modules;
	FTimerHandle SpawnItemsHandle;

	void PopulateGunAndModuleArray();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Unlock();
	void SetGunMerchToSpawn(TArray<TSubclassOf<AActor>>& GunList);
	void SetModuleMerchToSpawn(FOPAG_GunModules& ModulesStruct);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool CanInteract();
	virtual bool CanInteract_Implementation() override;

	UFUNCTION() void SpawnItem();

};
