#pragma once

#include "Engine/DataTable.h"
#include "OPAG_LootChance.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_LootManagerComponent.generated.h"

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
	AOPAG_LootBox* LootBox;

private:
	TArray<FOPAG_LootChance*> LootChance;
	TArray<FOPAG_DropList*> Drops;
	TArray<FOPAG_ModuleList*> Modules;
	TArray<FOPAG_GunList*> Guns;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateLoot(const float DifficultyValue);
	void Unlock() const;

private:
	void PopulateLootLists();
	TSubclassOf<AActor> SelectRandomModules(const float DifficultyValue, const int FireMask);
	TSubclassOf<AActor> SelectRandomModules(const float DifficultyValue, const int FireMask, const EModuleTypes ModuleTypes);
};
