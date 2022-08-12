#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "OPAG_LootBox.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_LootBox : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()

	bool bIsInteractable = true;

public:	
	AOPAG_LootBox();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Lid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Crate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* LootSpawnPoint;

public:
	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	float ThrowLid = 100.f;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	float ThrowItems = 100.f;

private:
	TArray<TSubclassOf<AActor>> Items;   
	FOPAG_GunModules Modules;
	FTimerHandle SpawnItemsHandle;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetItemsToSpawn(TArray<TSubclassOf<AActor>>& ItemsList);
	void SetModules(FOPAG_GunModules& ModulesStruct);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool CanInteract();
	virtual bool CanInteract_Implementation() override;

	UFUNCTION() void SpawnItem();
};
