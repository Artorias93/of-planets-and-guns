// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/OPAG_TextDamage.h"
#include "Components/WidgetComponent.h"
#include "UI/OPAG_UWTextDamage.h"

AOPAG_TextDamage::AOPAG_TextDamage()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->SetupAttachment(RootComponent);	
}

void AOPAG_TextDamage::Init(float Damage, bool bCrit)
{	
	IncomingDamage = Damage;
	bCritic = bCrit;
}

void AOPAG_TextDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewPosition = FMath::Lerp(InitialPosition, FinalPosition, DeltaTime);
	SetActorLocation(NewPosition);
}

void AOPAG_TextDamage::BeginPlay()
{
	Super::BeginPlay();

	float YOffset = FMath::RandRange(-1 * YOffsetRange, YOffsetRange);
	float ZOffset = FMath::RandRange(-1 * ZOffsetRange, ZOffsetRange);

	InitialPosition = GetActorLocation() + FVector(0, YOffset, 0);
	FinalPosition = InitialPosition + FVector(0, 0, ZOffset);

	SetActorLocation(InitialPosition);

	if (UOPAG_UWTextDamage* const TextDamage = Cast<UOPAG_UWTextDamage>(WidgetComponent->GetUserWidgetObject()))
	{
		TextDamage->Init(IncomingDamage, bCritic);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_TextDamage::RemoveFromScene, 2.f, false);
}

void AOPAG_TextDamage::RemoveFromScene()
{
	Destroy();
}


