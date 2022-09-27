#include "Weapons/Guns/OPAG_Pistol.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Items/OPAG_Barrel.h"

AOPAG_Pistol::AOPAG_Pistol()
{
	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);
}

void AOPAG_Pistol::BeginPlay()
{
	Super::BeginPlay();

}

void AOPAG_Pistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Shooting Behavior
USceneComponent* AOPAG_Pistol::GetMuzzle() const
{
	if(MuzzleComponent)
		return MuzzleComponent;

	return nullptr;
}
#pragma endregion Shooting Behavior