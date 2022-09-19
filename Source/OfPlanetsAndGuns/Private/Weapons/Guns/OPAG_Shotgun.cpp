#include "Weapons/Guns/OPAG_Shotgun.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Items/OPAG_Barrel.h"
#include "Abilities/OPAG_ForceField.h"
#include <Kismet/KismetMathLibrary.h>

AOPAG_Shotgun::AOPAG_Shotgun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOPAG_Shotgun::ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	for (int i = 0; i < ConsecutiveProjectile; i++)
	{
		Super::ShootLineTrace(StartShootLocation, Direction, ShooterCharacter);
	}
}