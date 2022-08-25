// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Managers/RoomManager/OPAG_LootChance.h"
#include "OPAG_LootDropSystemComponent.generated.h"

class AOPAG_Drop;
class AOPAG_Enemy;

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_Loot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_Drop> Drop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DroppingChance;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_LootDropSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOPAG_LootDropSystemComponent();

	void DropLoot() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* LootChanceTable;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* DropListTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* LootModuleTable;

protected:
	virtual void BeginPlay() override;

private:
	AOPAG_Enemy* Owner;
	TArray<FOPAG_LootChance*> LootChances;
	TArray<FOPAG_DropList*> DropList;
	TArray<FOPAG_ModuleList*> ModuleList;

	void PopulateLootChanceArray();
};
