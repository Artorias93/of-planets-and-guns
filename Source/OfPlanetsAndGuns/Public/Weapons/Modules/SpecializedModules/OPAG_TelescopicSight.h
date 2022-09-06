// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_SightBase.h"
#include "OPAG_TelescopicSight.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_TelescopicSight : public AOPAG_SightBase
{
	GENERATED_BODY()

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ScopeView;
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* ScopeCapture;

public:
	AOPAG_TelescopicSight();

protected:
	virtual void BeginPlay() override;
	virtual void OnAttach(const EModuleRarity NewRarity, const EModuleBrand NewBrand,
	                      const FOPAG_GunStats NewStats, const int NewEquippableFireTypes, const bool bIsAttachedToPlayer) override;
};
