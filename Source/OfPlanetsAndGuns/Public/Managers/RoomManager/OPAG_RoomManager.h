// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "OPAG_RoomManager.generated.h"

class AOPAG_Portal;
class UOPAG_EnemyManagerComponent;
class UOPAG_DoorManagerComponent;
class AOPAG_RoomStreamingVolume;
class AOPAG_RoomAttachPoint;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RoomManager : public AActor
{
	GENERATED_BODY()


	UPROPERTY(Category = "Door Manager", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UOPAG_DoorManagerComponent* DoorManager;

	UPROPERTY(Category = "Enemy Manager", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UOPAG_EnemyManagerComponent* EnemyManager;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = ERoomExit), Category = "Room Manager")
		int ExitSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Manager")
		uint8 bIsStartingRoom : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Manager")
		uint8 bIsRoomCleared : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Manager", AdvancedDisplay)
		int AssignedRoomID;


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

	UFUNCTION(BlueprintCallable, Category = "Door Manager")
		void EnterRoom();

	UFUNCTION(BlueprintCallable, Category = "Enemy Manager")
		void RoomCleared();
};
