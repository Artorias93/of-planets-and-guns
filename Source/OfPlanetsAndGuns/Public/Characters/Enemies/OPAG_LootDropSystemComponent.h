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

	UPROPERTY(EditAnywhere, Category = LootDrop)
		UDataTable* LootChanceTable;	
	UPROPERTY(EditAnywhere, Category = LootDrop)
		UDataTable* DropListTable;

protected:
	virtual void BeginPlay() override;

private:
	AOPAG_Enemy* Owner;
	TArray<FOPAG_LootChance*> LootArray;
	TArray<FOPAG_DropList*> DropArray;

	void PopulateLootChanceArray();
};
