#pragma once

#include "CoreMinimal.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "Components/TimelineComponent.h"
#include "OPAG_LootBox.generated.h"

class UNiagaraSystem;

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
	UTimelineComponent* DropTimeline;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	class UCurveVector* CurveDropVector;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	float DropCurveXFactor;

	UPROPERTY(Category = LootBox, EditAnywhere, BlueprintReadWrite)
	float DropCurveZFactor;

	FOnTimelineVector OnDropTimelineProgress;
	
	UPROPERTY()
	FVector DropTimelinePropertyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ActivationParticles;

private:
	TArray<TSubclassOf<AActor>> Items;   
	FOPAG_GunModules Modules;
	FTimerHandle SpawnItemsHandle;

	class UOPAG_SoundManagerSubsystem* SMS = nullptr;

	UPROPERTY()
		AActor* SpawnedItem;

	UPROPERTY()
		FVector RandomDropDir;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetItemsToSpawn(TArray<TSubclassOf<AActor>>& ItemsList);
	void Unlock();
	void SetModules(FOPAG_GunModules& ModulesStruct);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool CanInteract();
	virtual bool CanInteract_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool WillChangeInteractStateDynamically();
	virtual bool WillChangeInteractStateDynamically_Implementation() override;

	FString InteractString_Implementation() override;

	UFUNCTION()
	void DropTimelineProgress(FVector Value);

	UFUNCTION() void SpawnItem();

	UFUNCTION()
	void ActivationParticleEffect();
};
