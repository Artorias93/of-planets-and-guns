#pragma once

#include "Engine/DataTable.h"
#include "OPAG_LootChance.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_LootManagerComponent.generated.h"

class AOPAG_LootBox;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_LootManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOPAG_LootManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
	UDataTable* LootTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
	AOPAG_LootBox* LootBox;

private:
	TArray<FOPAG_LootChance*> Drops;
	TArray<FOPAG_LootChance*> Modules;
	TArray<FOPAG_LootChance*> Guns;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateLoot(const float DifficultyValue);

private:
	void PopulateLootLists();
};
