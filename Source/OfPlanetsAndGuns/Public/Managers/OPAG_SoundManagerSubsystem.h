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

UENUM(BlueprintType, meta = (Bitflags))
enum class EEnviromentSoundAction : uint8
{
	Chest_Open = 0x00,
	Doors_Open = 0x01,
	Doors_Close = 0x02,
	Zipline_On = 0x03,
	Zipline_Out = 0x04,
	Ambient = 0x05,
};
ENUM_CLASS_FLAGS(EEnviromentSoundAction);

UENUM(BlueprintType, meta = (Bitflags))
enum class ECharacterSoundAction : uint8
{
	Player_Damage = 0x00,
	Player_Death = 0x01,
	Player_Health_Gain = 0x02,
	Player_Health_Low = 0x03,
	Movement_Dash = 0x04,
	Movement_Jump = 0x05,
	Movement_Step = 0x06,
	Player_Shield_Broken = 0x07,
	Player_Shield_Charged = 0x08,
	Player_Shield_Hit = 0x09,
};
ENUM_CLASS_FLAGS(ECharacterSoundAction);

UENUM(BlueprintType, meta = (Bitflags))
enum class EUISoundAction : uint8
{
	UI_Confirm = 0x00,
	UI_Move = 0x01,
	UI_Back = 0x02,
};
ENUM_CLASS_FLAGS(EUISoundAction);

UENUM(BlueprintType, meta = (Bitflags))
enum class ESoundtrackType : uint8
{
	Gameplay = 0x00,
	Menu = 0x01,
};
ENUM_CLASS_FLAGS(ESoundtrackType);


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Libraries")
		UDataTable* SoundtracksAudioLibrary;

	UPROPERTY(EditAnywhere, Category = "FMOD")
		class UFMODAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, Category = "FMOD | Zipline")
		class UFMODAudioComponent* ZiplineAudioComponent;


	FFMODEventInstance InstanceUI;
	FFMODEventInstance InstanceAmbient;
	FFMODEventInstance InstanceSoundtrack;

public: 
	UFUNCTION(BlueprintCallable)
		void PlayWeaponSounds(AOPAG_GunBase* weapon, EWeaponSoundAction action);
	UFUNCTION(BlueprintCallable)
		void PlayAbiltySounds(const AActor* actor , EAbilitySoundAction ability);
	UFUNCTION(BlueprintCallable)
		void PlayEnviromentSounds(AActor* actor, EEnviromentSoundAction event);
	UFUNCTION(BlueprintCallable)
		void PlayCharacterSounds(AActor* actor, ECharacterSoundAction event);
	UFUNCTION(BlueprintCallable)
		void PlayUISounds(EUISoundAction event);
	UFUNCTION(BlueprintCallable)
		void PlaySoundTrack(ESoundtrackType type);
	UFUNCTION(BlueprintCallable)
		void PlayAmbient();
	UFUNCTION(BlueprintCallable)
		void StopSound();
	UFUNCTION(BlueprintCallable)
		void StopSoundtrack();
	UFUNCTION(BlueprintCallable)
		void StopAmbient();
private:
	UFUNCTION(BlueprintCallable)
		void PlayWeaponsFireSound(AOPAG_GunBase* weapon);
	UFUNCTION(BlueprintCallable)
		void PlayWeaponsReloadSound(AOPAG_GunBase* weapon);
	UFUNCTION(BlueprintCallable)
		void PlayWeaponsPickupSound(AOPAG_GunBase* weapon);

};
