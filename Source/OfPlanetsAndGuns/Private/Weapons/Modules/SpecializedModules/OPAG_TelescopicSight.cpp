// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Modules/SpecializedModules/OPAG_TelescopicSight.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

AOPAG_TelescopicSight::AOPAG_TelescopicSight()
{


	ScopeView = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope View"));
	ScopeView->SetupAttachment(RootComponent);
	ScopeView->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	ScopeView->SetRelativeScale3D(FVector::OneVector * 0.04f);
	ScopeView->SetCollisionProfileName(TEXT("NoCollision"));

	const auto Plane = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (Plane.Succeeded())
	{
		ScopeView->SetStaticMesh(Plane.Object);
		const auto RenderTargetMat = ConstructorHelpers::FObjectFinder<UMaterial>(
			TEXT("Material'/Game/OfPlanetsAndGuns/Weapons/Modules/Mesh/Sight/M_ScopeView.M_ScopeView'"));
		if (RenderTargetMat.Succeeded())
		{
			ScopeView->SetMaterial(0, RenderTargetMat.Object);
		}
	}

	ScopeCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scope Capture"));
	ScopeCapture->SetupAttachment(ScopeView);
	ScopeCapture->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	ScopeCapture->SetRelativeScale3D(FVector::OneVector * 3.f);
	ScopeCapture->FOVAngle = 25.f;
	ScopeCapture->SetComponentTickEnabled(false);

	const auto RenderTarget = ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>(
		TEXT("TextureRenderTarget2D'/Game/OfPlanetsAndGuns/Weapons/Modules/Mesh/Sight/RT_ScopeView.RT_ScopeView'"));
	if (RenderTarget.Succeeded())
	{
		ScopeCapture->TextureTarget = RenderTarget.Object;
	}
}

void AOPAG_TelescopicSight::BeginPlay()
{
	Super::BeginPlay();

	ScopeCapture->SetComponentTickEnabled(false);
	ScopeView->SetVisibility(false);
}

void AOPAG_TelescopicSight::OnAttach(const EModuleBrand NewBrand,
                                     const FOPAG_GunStats NewStats, const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	Super::OnAttach(NewBrand, NewStats, NewEquippableFireTypes, bIsAttachedToPlayer);

	if (!bIsAttachedToPlayer) return;

	ScopeCapture->SetComponentTickEnabled(true);
	ScopeView->SetVisibility(true);
}
