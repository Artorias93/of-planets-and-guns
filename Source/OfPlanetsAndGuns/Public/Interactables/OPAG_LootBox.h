#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "OPAG_LootBox.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInstanceDynamic;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_LootBox : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()

	bool bHasBeenOpened = false;
	bool bIsInteractable = false;

public:	
	AOPAG_LootBox();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Lid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Crate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* LootSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
	UDataTable* LootChanceTable;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	float ThrowLid = 100.f;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	float ThrowItems = 100.f;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
		int MinModules = 0;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
		int MaxModules = 3;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	int NumberOfModulesToSpawn = 1;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* NsActivationParticles;


private:
	TArray<TSubclassOf<AActor>> Items;   
	FOPAG_GunModules Modules;
	FTimerHandle SpawnItemsHandle;
	class UOPAG_SoundManagerSubsystem* SMS = nullptr;
	UPROPERTY() AActor* SpawnedItem;
	UNiagaraComponent* NcActivationParticles;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetItemsToSpawn(TArray<TSubclassOf<AActor>>& ItemsList);
	void Unlock();
	void SetModules(FOPAG_GunModules& ModulesStruct);
	UMaterialInstanceDynamic* DynamicMaterial; //////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool CanInteract();
	virtual bool CanInteract_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool WillChangeInteractStateDynamically();
	virtual bool WillChangeInteractStateDynamically_Implementation() override;

	FString InteractString_Implementation() override;

	UFUNCTION() void SpawnItem();

	UFUNCTION()
	void ActivationParticleEffect();
};
