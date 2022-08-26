// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"

#include "OPAG_SoundManagerSubsystem.generated.h"

USTRUCT(Blueprintable)
struct FAudioEvent : public FTableRowBase {

	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFMODEvent* AudioContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLoop;
	
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EWeaponSoundAction : uint8
{
	Fire = 0x00,
	Reload = 0x01,
	Pickup = 0x02,
};
ENUM_CLASS_FLAGS(EWeaponSoundAction);


UCLASS()
class OFPLANETSANDGUNS_API UOPAG_SoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* WeaponsAudioLibrary;

	UPROPERTY(EditAnywhere, Category = "FMOD")
		class UFMODAudioComponent* AudioComponent;

	FFMODEventInstance InstanceWrapper;

public: 
	UFUNCTION(BlueprintCallable)
		void PlayWeaponSounds(AOPAG_GunBase* weapon, EWeaponSoundAction action);
	UFUNCTION(BlueprintCallable)
		void StopSound();
private:
	UFUNCTION(BlueprintCallable)
		void PlayWeaponsFireSound(AOPAG_GunBase* weapon);
	UFUNCTION(BlueprintCallable)
		void PlayWeaponsReloadSound(AOPAG_GunBase* weapon);
	UFUNCTION(BlueprintCallable)
		void PlayWeaponsPickupSound(AOPAG_GunBase* weapon);

};
