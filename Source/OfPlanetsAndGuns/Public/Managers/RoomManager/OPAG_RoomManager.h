// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_RoomOffset.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "OPAG_RoomManager.generated.h"

class AOPAG_Portal;
class UOPAG_EnemyManagerComponent;
class UOPAG_DoorManagerComponent;
class AOPAG_RoomStreamingVolume;
class AOPAG_RoomAttachPoint;
class UOPAG_LootManagerComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RoomManager : public AActor
{
	GENERATED_BODY()


	UPROPERTY(Category = "Door Manager", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UOPAG_DoorManagerComponent* DoorManager;

	UPROPERTY(Category = "Enemy Manager", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UOPAG_EnemyManagerComponent* EnemyManager;

	UPROPERTY(Category = "Enemy Manager", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UOPAG_LootManagerComponent* LootManager;

	uint8 bHasBroadcastedEntry : 1;
	uint8 bIsAwaitingDifficultyUpdate : 1;

public:	
	// Sets default values for this actor's properties
	AOPAG_RoomManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		AOPAG_RoomAttachPoint* Entrance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		TArray<AOPAG_RoomAttachPoint*> Exits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		TSoftObjectPtr<UWorld> Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		TSoftObjectPtr<UWorld> GeometryLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		AOPAG_RoomStreamingVolume* Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = ERoomExits), Category = "Room Manager")
		uint8 ExitSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		ERoomType RoomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Manager")
		uint8 bIsRoomCleared : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Manager", AdvancedDisplay)
		int AssignedRoomID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager", AdvancedDisplay)
		float OverriddenDifficulty = 0.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Room Manager")
		void UpdateData();

	UFUNCTION(BlueprintCallable, Category = "Room Manager")
		void CloseExits(bool bIsLastRoom, bool bHasSafeRoom) const;

	UFUNCTION(Category = "Room Manager")
		void OnStageDifficultyUpdated();

	UFUNCTION(BlueprintCallable, Category = "Room Manager")
		float GetCurrentRoomDifficulty() const;

	UFUNCTION(BlueprintCallable, Category = "Door Manager")
		void EnterRoom();

	UFUNCTION(BlueprintCallable, Category = "Enemy Manager")
		void RoomCleared();

};
