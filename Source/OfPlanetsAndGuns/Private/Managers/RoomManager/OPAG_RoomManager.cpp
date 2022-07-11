// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RoomManager/OPAG_RoomManager.h"

#include "GameFramework/GameStateBase.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"

// Sets default values
AOPAG_RoomManager::AOPAG_RoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const auto world = AActor::GetWorld();
	if (world != nullptr)
	{
		const auto gameState = world->GetGameState();;
		if (gameState != nullptr && gameState->HasBegunPlay())
		{
			UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
			if (GI)
			{
				GI->GetEventManager()->RoomSpawned.Broadcast(this);
				return;
			}
		}
	}
	
	UpdateData();
}

// Called when the game starts or when spawned
void AOPAG_RoomManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_RoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_RoomManager::UpdateData()
{
	
}

