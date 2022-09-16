// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_InventorySystemComponent.generated.h"

class AOPAG_Drop;
class AOPAG_Player;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_InventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOPAG_InventorySystemComponent();

	void AnalyzeItem(AOPAG_Drop* Target);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Category = "Inventory Stuff", EditAnywhere, BlueprintReadWrite)
		int PotionsAmount = 0;
	UPROPERTY(Category = "Inventory Stuff", EditAnywhere, BlueprintReadWrite)
		int GoldAmount = 0;
	UPROPERTY(Category = "Inventory Stuff", EditAnywhere, BlueprintReadWrite)
		int BoltsAmount = 0;


private:
	AOPAG_Player* Owner;

	FTimerHandle GoldShowTimerHandle;

	float GoldShowTimer = 2.0f;

	UFUNCTION()
	void OnGoldShowFinished();
};
