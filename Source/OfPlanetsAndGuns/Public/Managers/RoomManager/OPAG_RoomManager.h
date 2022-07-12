// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "OPAG_RoomManager.generated.h"

class AOPAG_RoomStreamingVolume;
class AOPAG_RoomAttachPoint;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RoomManager : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AOPAG_RoomManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AOPAG_RoomAttachPoint* Entrance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AOPAG_RoomAttachPoint*> Exits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName GeometryLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AOPAG_RoomStreamingVolume* Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = ERoomExit))
		int ExitSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Weight;

	int AssignedRoomID;

private:
	void SetExitAsSpecial(bool bIsLastRoom, bool bHasSafeRoom) const;
	void BlockExit() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, CallInEditor)
		void UpdateData();

	UFUNCTION(BlueprintCallable)
		void CloseExits(bool bIsLastRoom, bool bHasSafeRoom) const;
};
