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

UENUM(BlueprintType, meta = (Bitflags))
enum class EAbilitySoundAction : uint8
{
	GL_Fire = 0x00,
	GL_Explosion = 0x01,
	GL_Reload = 0x02,
	Ability_Ready = 0x03,
	Shield_Bullets = 0x04,
};
ENUM_CLASS_FLAGS(EAbilitySoundAction);


UCLASS()
class OFPLANETSANDGUNS_API UOPAG_SoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* WeaponsAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* AbilitiesAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* EnviromentAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* CharacterAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* UIAudioLibrary;

	UPROPERTY(EditAnywhere, Category = "FMOD")
		class UFMODAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, Category = "FMOD")
		class UFMODAudioComponent* AudioAbilityComponent;

	FFMODEventInstance InstanceWrapper;

public: 
	UFUNCTION(BlueprintCallable)
		void PlayWeaponSounds(AOPAG_GunBase* weapon, EWeaponSoundAction action);
	UFUNCTION(BlueprintCallable)
		void PlayAbiltySounds(AActor* actor , EAbilitySoundAction ability);
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
