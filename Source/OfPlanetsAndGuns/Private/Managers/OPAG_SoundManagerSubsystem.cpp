// Fill out your copyright notice in the Description page of Project Settings.
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

/*
	CORE
*/

void UOPAG_SoundManagerSubsystem::Init() {
	BindEvents();
	studioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	//SetBusVolume(EBusNames::MASTER, dBToLinear(0));
	SetBusVolume(EBusNames::MASTER, 1);
	SetBusVolume(EBusNames::MUSIC, 0.65);
	SetBusVolume(EBusNames::AMBIENT, 0.7);
	SetBusVolume(EBusNames::SFX, 0.7);
	SetBusVolume(EBusNames::UI, 1);

}


void UOPAG_SoundManagerSubsystem::BindEvents(){
	EMS = GetWorld()->GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	EMS->MasterSliderChanged.AddDynamic(this,&UOPAG_SoundManagerSubsystem::MasterSliderChanged);
	EMS->MusicSliderChanged.AddDynamic(this, &UOPAG_SoundManagerSubsystem::MusicSliderChanged);
	EMS->SFXSliderChanged.AddDynamic(this, &UOPAG_SoundManagerSubsystem::SFXSliderChanged);
	EMS->AmbientSliderChanged.AddDynamic(this, &UOPAG_SoundManagerSubsystem::AmbientSliderChanged);
	EMS->SystemSliderChanged.AddDynamic(this, &UOPAG_SoundManagerSubsystem::SystemSliderChanged);
	EMS->UIButtonPressed.AddDynamic(this, &UOPAG_SoundManagerSubsystem::PlayUISounds);
}


void UOPAG_SoundManagerSubsystem::SetIntensity(float value) {
	if (value > 0)
		Intensity += value;
	else
		Intensity = 0;
	//InstanceSoundtrack.Instance->setParameterByName(TCHAR_TO_UTF8(*FName("Intensity").ToString()), Intensity , true);
	UFMODBlueprintStatics::EventInstanceSetParameter(InstanceSoundtrack, FName("Intensity"), Intensity);
	float intensityToLog = UFMODBlueprintStatics::EventInstanceGetParameter(InstanceSoundtrack, FName("Intensity"));
	UE_LOG(LogTemp, Warning, TEXT("Intensity %f"), intensityToLog);

}


void UOPAG_SoundManagerSubsystem::StopZiplineSound() {
	if (ZiplineAudioComponent) {
		ZiplineAudioComponent->Stop();
		ZiplineAudioComponent->Release();
	}	
}

void UOPAG_SoundManagerSubsystem::StopRewardSound() {
	if (RewardAudioComponent) {
		RewardAudioComponent->Stop();
		RewardAudioComponent->Release();
	}
}

float  UOPAG_SoundManagerSubsystem::dBToLinear(const float &dbValue) {
	return pow(10.0f, dbValue / 20.0f);
}

void UOPAG_SoundManagerSubsystem::SetBusVolume(EBusNames busName, const float& newValue) {
	switch (busName)
	{
	case EBusNames::MASTER:
		FMOD::Studio::Bus* masterBus;
		studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/").ToString()), &masterBus);
		masterBus->setVolume(newValue);
		break;
	case EBusNames::MUSIC:
		FMOD::Studio::Bus* musicBus;
		studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/Music").ToString()), &musicBus);
		musicBus->setVolume(newValue);
		break;
	case EBusNames::AMBIENT:
		FMOD::Studio::Bus* ambientBus;
		studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/Ambient").ToString()), &ambientBus);
		ambientBus->setVolume(newValue);
	case EBusNames::SFX:
		FMOD::Studio::Bus* sfxBus;
		studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/SFX").ToString()), &sfxBus);
		sfxBus->setVolume(newValue);
		break;
	case EBusNames::UI:
		FMOD::Studio::Bus* uiBus;
		studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/UI").ToString()), &uiBus);
		uiBus->setVolume(newValue);
		break;
	default:
		break;
	}
}

void UOPAG_SoundManagerSubsystem::MasterSliderChanged(float value) {
	//float newVolume = FMath::Lerp(dBToLinear(minVolumeDB), dBToLinear(maxVolumeDB), value);
	float newVolume = FMath::Lerp(minVolumeLinear, maxVolumeLinear, value);

	SetBusVolume(EBusNames::MASTER , newVolume);
}


void UOPAG_SoundManagerSubsystem::MusicSliderChanged(float value) {
	//float newVolume = FMath::Lerp(dBToLinear(minVolumeDB), dBToLinear(maxVolumeDB), value);
	float newVolume = FMath::Lerp(minVolumeLinear, maxVolumeLinear, value);
	SetBusVolume(EBusNames::MUSIC, newVolume);
}

void UOPAG_SoundManagerSubsystem::SFXSliderChanged(float value) {
	//float newVolume = FMath::Lerp(dBToLinear(minVolumeDB), dBToLinear(maxVolumeDB), value);
	float newVolume = FMath::Lerp(minVolumeLinear, maxVolumeLinear, value);

	SetBusVolume(EBusNames::SFX, newVolume);
}

void UOPAG_SoundManagerSubsystem::AmbientSliderChanged(float value) {
	//float newVolume = FMath::Lerp(dBToLinear(minVolumeDB), dBToLinear(maxVolumeDB), value);
	float newVolume = FMath::Lerp(minVolumeLinear, maxVolumeLinear, value);

	SetBusVolume(EBusNames::AMBIENT, newVolume);
}

void UOPAG_SoundManagerSubsystem::SystemSliderChanged(float value) {
	//float newVolume = FMath::Lerp(dBToLinear(minVolumeDB), dBToLinear(maxVolumeDB), value);
	float newVolume = FMath::Lerp(minVolumeLinear, maxVolumeLinear, value);

	SetBusVolume(EBusNames::UI, newVolume);
}

float UOPAG_SoundManagerSubsystem::GetBusVolume(EBusNames busName){
	//return valore volume tra 0 e 1
	FMOD::Studio::Bus* bus = nullptr;
	float* currentVol = new float(0);
	if (studioSystem) {
		switch (busName)
		{
		case EBusNames::MASTER:
			studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/").ToString()), &bus);
			break;
		case EBusNames::MUSIC:
			studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/Music").ToString()), &bus);
			break;
		case EBusNames::AMBIENT:
			studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/Ambient").ToString()), &bus);
			break;
		case EBusNames::SFX:
			studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/SFX").ToString()), &bus);
			break;
		case EBusNames::UI:
			studioSystem->getBus(TCHAR_TO_UTF8(*FName("bus:/UI").ToString()), &bus);
			break;
		default:
			break;
		}
	}
	if (bus) {
		bus->getVolume(currentVol);
	}
	float returnValue;
	if (currentVol) {
		//returnValue = *currentVol;
		//returnValue = (*currentVol - dBToLinear(minVolumeDB)) / dBToLinear(maxVolumeDB) - dBToLinear(minVolumeDB);
		returnValue = (*currentVol - minVolumeLinear) / maxVolumeLinear - minVolumeLinear;
	}
	//return (*currentVol * 100)/ dBToLinear(maxVolumeDB);
	delete currentVol;
	UE_LOG(LogTemp, Warning, TEXT("Text, %f "), returnValue);
	return returnValue;
}


/*
	PLAY SOUND
*/
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

void UOPAG_SoundManagerSubsystem::PlayTurretSounds(AActor* weapon) {
	FVector position = weapon->GetActorLocation();
	FAudioEvent* libraryRow = nullptr;
	if (WeaponsAudioLibrary) {
		libraryRow = WeaponsAudioLibrary->FindRow<FAudioEvent>(*FString("Single_Fire"), "", false);
	}
	if (libraryRow) {
		UFMODEvent* currentEvent = libraryRow->AudioContent;
		AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, weapon->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, true, true, true);
	}
}

void UOPAG_SoundManagerSubsystem::PlayAbiltySounds(const AActor* actor, EAbilitySoundAction ability) {
	FVector position = actor->GetActorLocation();
	if (AbilitiesAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (ability)
		{
		case EAbilitySoundAction::GL_Fire:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Ability_GL_Fire"), "", false);
			break;
		case EAbilitySoundAction::GL_Explosion:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Ability_GL_Explosion"), "", false);
			break;
		case EAbilitySoundAction::GL_Reload:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Ability_GL_Reload"), "", false);
			break;
		case EAbilitySoundAction::Ability_Ready:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Ability_Ready"), "", false);
			break;
		case EAbilitySoundAction::Shield_Bullets:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Ability_Shield_Bullet"), "", false);
			break;
		case EAbilitySoundAction::Shield_Charged:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Ability_Shield_Charged"), "", false);
			break;
		case EAbilitySoundAction::Module_PickUp:
			libraryRow = AbilitiesAudioLibrary->FindRow<FAudioEvent>(*FString("Module_Pickup"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, actor->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, false, true, true);
		}
	}
}

void UOPAG_SoundManagerSubsystem::PlayUISounds( EUISoundAction action) {
	if (UIAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (action)
		{
		case EUISoundAction::UI_Confirm:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("UI_Confirm"), "", false);
			break;
		case EUISoundAction::UI_Move:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("UI_Move"), "", false);
			break;
		case EUISoundAction::UI_Back:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("UI_Back"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			InstanceUI = UFMODBlueprintStatics::PlayEvent2D( GetWorld(), currentEvent, false);
		}
	}
}

void UOPAG_SoundManagerSubsystem::PlayEnviromentSounds(AActor* actor, EEnviromentSoundAction event){
	FVector position = actor->GetActorLocation();
	if (EnviromentAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (event)
		{
		case EEnviromentSoundAction::Chest_Open:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Chest_Open"), "", false);
			break;
		case EEnviromentSoundAction::Doors_Open:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Door_Open"), "", false);
			break;
		case EEnviromentSoundAction::Doors_Close:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Door_Close"), "", false);
			break;
		case EEnviromentSoundAction::Zipline_On:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Zipline_On"), "", false);
			break;
		case EEnviromentSoundAction::Zipline_Out:
			libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Zipline_Out"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			if(event == EEnviromentSoundAction::Zipline_On)
				ZiplineAudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, actor->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, false, true, true);
			else
				AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, actor->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, false, true, true);
		}
	}
}

// Sbatti quando è empty
void UOPAG_SoundManagerSubsystem::PlayWeaponsFireSound(AOPAG_GunBase* weapon) {
	FVector position = weapon->GetActorLocation();
	EFireTypes weaponType = weapon->GetFireType();
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
	EFireTypes weaponType = weapon->GetFireType();
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
	EFireTypes weaponType = weapon->GetFireType();
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

void UOPAG_SoundManagerSubsystem::PlayCharacterSounds(AActor* actor, ECharacterSoundAction event) {
	FVector position = actor->GetActorLocation();
	if (CharacterAudioLibrary) {
		FAudioEvent* libraryRow;
		switch (event)
		{
		case ECharacterSoundAction::Player_Damage:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Damage"), "", false);
			break;
		case ECharacterSoundAction::Player_Death:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Death"), "", false);
			break;
		case ECharacterSoundAction::Player_Health_Gain:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Health_Gain"), "", false);
			break;
		case ECharacterSoundAction::Player_Health_Low:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Health_Low"), "", false);
			break;
		case ECharacterSoundAction::Movement_Dash:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Movement_Dash"), "", false);
			break;
		case ECharacterSoundAction::Movement_Jump:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Movement_Jump"), "", false);
			break;
		case ECharacterSoundAction::Movement_Step:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Movement_FootStep"), "", false);
			break;
		case ECharacterSoundAction::Player_Shield_Broken:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Shield_Broken"), "", false);
			break;
		case ECharacterSoundAction::Player_Shield_Charged:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Shield_Charged"), "", false);
			break;
		case ECharacterSoundAction::Player_Shield_Hit:
			libraryRow = CharacterAudioLibrary->FindRow<FAudioEvent>(*FString("Player_Shield_Hit"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			AudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, actor->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, true, true, true);
		}
	}
}

void UOPAG_SoundManagerSubsystem::PlayRewardSound(AActor* actor) {
	FVector position = actor->GetActorLocation();
	if (EnviromentAudioLibrary) {
		//if (InstanceAmbient.Instance->isValid())
			//StopAmbient();
		FAudioEvent* libraryRow;
		libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Chest_Near"), "", false);
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			RewardAudioComponent = UFMODBlueprintStatics::PlayEventAttached(currentEvent, actor->GetRootComponent(), NAME_None, position, EAttachLocation::KeepWorldPosition, false, true, true);
		}
	}
}

void UOPAG_SoundManagerSubsystem::PlayAmbient() {
	if (EnviromentAudioLibrary) {
		//if (InstanceAmbient.Instance->isValid())
			//StopAmbient();
		FAudioEvent* libraryRow;
		libraryRow = EnviromentAudioLibrary->FindRow<FAudioEvent>(*FString("Ambient"), "", false);
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;
			InstanceAmbient = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), currentEvent, true);
		}
	}
}
void UOPAG_SoundManagerSubsystem::PlaySoundTrack(ESoundtrackType type) {
	if (SoundtracksAudioLibrary) {
		if (InstanceSoundtrack.Instance->isValid()) {
			StopAmbient();
			StopSoundtrack();
		}
		FAudioEvent* libraryRow;
		switch (type)
		{
		case ESoundtrackType::Gameplay:
			libraryRow = SoundtracksAudioLibrary->FindRow<FAudioEvent>(*FString("Soundtrack_Gameplay"), "", false);
			break;
		case ESoundtrackType::Menu:
			libraryRow = SoundtracksAudioLibrary->FindRow<FAudioEvent>(*FString("SoundTrack_Menu"), "", false);
			break;
		default:
			libraryRow = nullptr;
			break;
		}
		if (libraryRow) {
			UFMODEvent* currentEvent = libraryRow->AudioContent;		
			InstanceSoundtrack = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), currentEvent, true);
			if(type== ESoundtrackType::Gameplay)
				UFMODBlueprintStatics::EventInstanceSetParameter(InstanceSoundtrack, FName("Intensity"), Intensity);
		}
	}
}
void UOPAG_SoundManagerSubsystem::StopSoundtrack() {
	if (InstanceSoundtrack.Instance->isValid()) {
		InstanceSoundtrack.Instance->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
		InstanceSoundtrack.Instance->release();
	}
}

void UOPAG_SoundManagerSubsystem::StopAmbient() {
	if (InstanceAmbient.Instance->isValid()) {
		InstanceAmbient.Instance->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
		InstanceAmbient.Instance->release();
	}
}