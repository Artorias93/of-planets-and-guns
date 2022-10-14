// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include <UObject/NoExportTypes.h>
#include "OPAG_UWGunView.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWGunView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UImage* GunIcon;

	UPROPERTY(meta = (BindWidget))
		UImage* HandgunIcon;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentAmmoText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* TotalAmmoText;

public:
	//Guns textures
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Textures")
		UTexture2D* RifleTexture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Textures")
		UTexture2D* SniperTexture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Textures")
		UTexture2D* BurstTexture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Textures")
		UTexture2D* ShotgunTexture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Textures")
		UTexture2D* HandgunTexture2D;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Colors")
		FLinearColor AmmoCurrentColorBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Colors")
		FLinearColor AmmoMaxColorBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG | Widgets | Colors")
		FLinearColor AmmoColorUnload;
	
public:
	void NativeConstruct();

	UFUNCTION() void UpdateCurrentAmmoText(const int CurrentValue);
	UFUNCTION() void UpdateTotalAmmoText(const int CurrentValue); 
	UFUNCTION() void UpdateAmmoColor(bool bIsAmmoCurrentUnload, bool bIsAmmoMaxUnload);
	UFUNCTION() void SwapGun(EFireTypes CurrentType);

};
