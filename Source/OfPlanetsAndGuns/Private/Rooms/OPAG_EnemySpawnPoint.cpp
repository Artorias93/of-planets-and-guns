// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_EnemySpawnPoint.h"

#include "Characters/Enemies/OPAG_Enemy.h"
#include "Components/BillboardComponent.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"


// Sets default values
AOPAG_EnemySpawnPoint::AOPAG_EnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Gizmo"));
	RootComponent = Billboard;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = -0.002f;

	const auto SpriteFinder = ConstructorHelpers::FObjectFinder<UTexture2D>(
		TEXT("Texture2D'/Game/OfPlanetsAndGuns/UI/Billboards/EnemySpawnerBillboard.EnemySpawnerBillboard'"));
	if (SpriteFinder.Succeeded()) Billboard->SetSprite(SpriteFinder.Object);
}

// Called when the game starts or when spawned
void AOPAG_EnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_EnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TSoftObjectPtr<AOPAG_Enemy>> AOPAG_EnemySpawnPoint::SpawnEnemies(
	TMap<TSubclassOf<AOPAG_Enemy>, float>& EnemyMap, int32 Amount) const
{
	const TSubclassOf<AOPAG_Enemy> EnemyToSpawn = UOPAG_UtilityFunctionLibrary::GetRandomItem(EnemyMap);
	const FActorSpawnParameters SpawnInfo;
	TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnedEnemies;
	TSoftObjectPtr<AOPAG_Enemy> Enemy = GetWorld()->SpawnActor<AOPAG_Enemy>(EnemyToSpawn.Get(), 
		GetActorLocation(), FRotator::ZeroRotator, SpawnInfo);
	if (Enemy.IsValid())
	{
		SpawnedEnemies.Add(Enemy);
	}
	return SpawnedEnemies;
}

int32 AOPAG_EnemySpawnPoint::GetSpawnerCount() const
{
	return 1;
}

