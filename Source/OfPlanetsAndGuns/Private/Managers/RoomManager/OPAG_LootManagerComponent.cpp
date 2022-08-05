

#include "Managers/RoomManager/OPAG_LootManagerComponent.h"

UOPAG_LootManagerComponent::UOPAG_LootManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UOPAG_LootManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOPAG_LootManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UOPAG_LootManagerComponent::SpawnLoot(const float DifficultyValue) const
{

}