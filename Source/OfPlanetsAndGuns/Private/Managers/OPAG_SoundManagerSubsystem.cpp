// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/OPAG_SoundManagerSubsystem.h"

void UOPAG_SoundManagerSubsystem::PlayWeaponSounds(AOPAG_GunBase* weapon, EWeaponSoundAction action) {
	switch (action)
	{
	case EWeaponSoundAction::Fire:
		PlayWeaponsFireSound(weapon);
		break;
	case EWeaponSoundAction::Reload:
		PlayWeaponsReloadSound(weapon);
		break;
	case EWeaponSoundAction::Pickup:
		PlayWeaponsPickupSound(weapon);
		break;
	default:
		break;
	}

}

// TYPE PER LA PISTOL??
// Sbatti quando è empty
void UOPAG_SoundManagerSubsystem::PlayWeaponsFireSound(AOPAG_GunBase* weapon) {
	FVector position = weapon->GetActorLocation();
	EFireTypes weaponType = weapon->FireType;
	if (WeaponsAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (weaponType)
		{
		case EFireTypes::Burst3 :
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("3Burst_Fire"), "", false);
			break;
		case EFireTypes::FullyAutomatic:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("FullAuto_Fire"), "", false);
			break;
		case EFireTypes::Shotgun:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Shotgun_Fire"), "", false);
			break;
		case EFireTypes::SingleShot:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Single_Fire"), "", false);
			break;
		case EFireTypes::Pistol:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Pistol_Fire"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, weapon->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, true, true, true);
		}

	}
}

void UOPAG_SoundManagerSubsystem::PlayWeaponsReloadSound(AOPAG_GunBase* weapon) {
	FVector position = weapon->GetActorLocation();
	EFireTypes weaponType = weapon->FireType;
	if (WeaponsAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (weaponType)
		{
		case EFireTypes::Burst3:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("3Burst_Reload"), "", false);
			break;
		case EFireTypes::FullyAutomatic:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("FullAuto_Reload"), "", false);
			break;
		case EFireTypes::Shotgun:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Shotgun_Reload"), "", false);
			break;
		case EFireTypes::SingleShot:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Single_Reload"), "", false);
			break;
		case EFireTypes::Pistol:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Pistol_Reload"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, weapon->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, true, true, true);
		}

	}
}

void UOPAG_SoundManagerSubsystem::PlayWeaponsPickupSound(AOPAG_GunBase* weapon) {
	FVector position = weapon->GetActorLocation();
	EFireTypes weaponType = weapon->FireType;
	if (WeaponsAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (weaponType)
		{
		case EFireTypes::Burst3:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("3Burst_Pickup"), "", false);
			break;
		case EFireTypes::FullyAutomatic:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("FullAuto_Pickup"), "", false);
			break;
		case EFireTypes::Shotgun:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Shotgun_Pickup"), "", false);
			break;
		case EFireTypes::SingleShot:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Single_Pickup"), "", false);
			break;
		case EFireTypes::Pistol:
			libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Pistol_Pickup"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, weapon->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, true, true, true);

		}

	}
}

void UOPAG_SoundManagerSubsystem::StopSound() {
	/*
	if (UFMODBlueprintStatics::EventInstanceIsValid(InstanceWrapper)) {
		InstanceWrapper.Instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
		InstanceWrapper.Instance->release();
	}
	*/
}