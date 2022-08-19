#pragma once

#include "Engine/DataTable.h"
#include "OPAG_LootChance.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_LootManagerComponent.generated.h"

class AOPAG_LootSpawnPoint;
class AOPAG_LootBox;
class AOPAG_ModuleBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_LootManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOPAG_LootManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
	UDataTable* LootDropTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
	UDataTable* LootModuleTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
	UDataTable* LootGunTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
		TArray<AOPAG_LootSpawnPoint*> Spawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager", Meta = (UIMin = 0.f, UIMax = 100.f))
		float VendorChance = 30.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
		TSubclassOf<AOPAG_LootBox> SmallLootBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
		TSubclassOf<AOPAG_LootBox> LargeLootBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
		TSubclassOf<AActor> VendorClass;

private:
	TPair<AOPAG_LootBox*, TArray<FOPAG_LootChance*>> LootBoxA;
	TPair<AOPAG_LootBox*, TArray<FOPAG_LootChance*>> LootBoxB;

	TArray<FOPAG_DropList*> Drops;
	TArray<FOPAG_ModuleList*> Modules;
	TArray<FOPAG_GunList*> Guns;

	AActor* Vendor;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateLoot(const float DifficultyValue);
	void Unlock() const;

private:
	void SpawnLootContainers();

	void GenerateIndividualLoot(AOPAG_LootBox* LootBox, TArray<FOPAG_LootChance*>& LootChance, const float DifficultyValue);
	void PopulateLootLists();
	TSubclassOf<AActor> SelectRandomModules(const float DifficultyValue, const int FireMask);
	TSubclassOf<AActor> SelectRandomModules(const float DifficultyValue, const int FireMask, const EModuleTypes ModuleTypes);
};
