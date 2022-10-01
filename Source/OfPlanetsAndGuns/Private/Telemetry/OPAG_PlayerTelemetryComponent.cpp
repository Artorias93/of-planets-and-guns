// Fill out your copyright notice in the Description page of Project Settings.


#include "Telemetry/OPAG_PlayerTelemetryComponent.h"

#include "Telemetry/OPAG_TelemetryFunctionLibrary.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

#include "RenderUtils.h"
#include "Characters/Player/OPAG_Player.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"

#pragma optimize("", off)

static TAutoConsoleVariable<bool> ConsoleVarTelemetry(
	TEXT("showdebug PlayerTelemetry"),
	false,
	TEXT("Toggles display of Player telemetry."),
	ECVF_Default);

static TAutoConsoleVariable<FString> ConsoleVarTelemetryList(
	TEXT("PlayerTelemetry.List"),
	TEXT(""),
	TEXT("List of telemetry queries."),
	ECVF_Default);

static TAutoConsoleVariable<FString> ConsoleVarTelemetryStats(
	TEXT("PlayerTelemetry.Stats"),
	TEXT("None"),
	TEXT("Get Weapon or specific Module stats"),
	ECVF_Default);



// Sets default values for this component's properties
UOPAG_PlayerTelemetryComponent::UOPAG_PlayerTelemetryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UOPAG_PlayerTelemetryComponent::UpdateTelemetryTargets()
{

	FString TelemetryList = ConsoleVarTelemetryList.GetValueOnGameThread();

	if (CurrentTelemetryList.Compare(TelemetryList) != 0)
	{
		TelemetryTargets.Reset();

		if (TelemetryList.Len() > 0)
		{
			TelemetryList.ParseIntoArray(TelemetryArray, TEXT(","));

			for (FString& Telemetry : TelemetryArray)
			{
				void* Object = nullptr;
				FProperty* Property = UOPAG_TelemetryFunctionLibrary::RetrieveProperty(GetOwner(), Telemetry, Object);

				FTelemetryTarget Target;
				Target.TargetObject = Object;
				Target.TargetProperty = Property;

				TelemetryTargets.Add(Target);
			}
		}

		CurrentTelemetryList = TelemetryList;
	}
}


// Called when the game starts
void UOPAG_PlayerTelemetryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOPAG_PlayerTelemetryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateTelemetryTargets();
}

void DrawLabel(UCanvas* InCanvas, const FString& InLabel, const FColor& InColor, float InX, float InY, float InMaxWidth, float& OutX, float& OutY)
{
	UFont* Font = GEngine->GetSmallFont();

	InCanvas->SetDrawColor(InColor);

	float XL, YL;
	InCanvas->TextSize(Font, InLabel, XL, YL);

	float ScaleX = 1.f;
	if (XL > InMaxWidth)
	{
		ScaleX = InMaxWidth / XL;
	}

	InCanvas->DrawText(Font, InLabel, InX, InY, ScaleX);

	OutX = InX;
	OutY = InY + YL;
}

void GetRarityColor(const EModuleRarity& InRarity, FColor& OutColor)
{
	switch (InRarity)
	{
	default: case EModuleRarity::Common: OutColor = FColor::White; break;
	case EModuleRarity::Uncommon: OutColor = FColor::Green; break;
	case EModuleRarity::Rare: OutColor = FColor::Cyan; break;
	case EModuleRarity::Epic: OutColor = FColor::Purple; break;
	case EModuleRarity::Legendary: OutColor = FColor::Yellow; break;
	}
}

void DrawStats(UCanvas* InCanvas, const FString& ModuleName, const FOPAG_GunStats& InStats, const FColor& InColor, float InX, float InY, float InMaxWidth, float& OutX, float& OutY)
{
	FString Label;
	Label.Append("[" + ModuleName + "]");
	Label.Append("   Damage: " + FString::SanitizeFloat(InStats.Damage, 2));
	Label.Append("   FireRate: " + FString::SanitizeFloat(InStats.FireRate, 2));
	Label.Append("   Accuracy: " + FString::SanitizeFloat(InStats.Accuracy, 2));
	Label.Append("   Stability: " + FString::SanitizeFloat(InStats.Stability, 2));
	Label.Append("   ReloadingTime: " + FString::SanitizeFloat(InStats.ReloadingTime, 2));
	Label.Append("   ClipSize: " + FString::FromInt(InStats.ClipSize));
	
	DrawLabel(InCanvas, Label, InColor, InX, InY, InMaxWidth, OutX, OutY);
}

void UOPAG_PlayerTelemetryComponent::DrawTelemetry(UCanvas* InCanvas, float& InOutYL, float& InOutYPos)
{
	static constexpr float GraphW = 800.0f;

	static constexpr int32 NumValues = 128;

	static const TArray<FColor> Colors = { FColor::Yellow, FColor::Red, FColor::Blue, FColor::Cyan, FColor::Magenta, FColor::Orange, FColor::Purple, FColor::White };

	static const TArray<float> EmptyArray;

	static constexpr float XPos = 4.f;
	static constexpr float YSep = 4.f;

	constexpr float CurrX = XPos;
	float CurrY = InOutYPos + 16.f;

	//title
	FString Label = "PLAYER TELEMETRY";

	float OutX;
	DrawLabel(InCanvas, Label, FColor::White, CurrX, CurrY, GraphW, OutX, CurrY);

	int32 TelemetryIdx = 0;


	//get values from list and draw
	for (const FTelemetryTarget& Target : TelemetryTargets)
	{
		float Value = 0.f;

		if (Target.TargetObject && Target.TargetProperty)
		{
			const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Target.TargetProperty);
			if (FloatProperty)
			{
				Value = FloatProperty->GetPropertyValue_InContainer(Target.TargetObject);
			}
			else
			{

				const FIntProperty* IntProperty = CastField<FIntProperty>(Target.TargetProperty);
				if (IntProperty)
				{
					Value = IntProperty->GetPropertyValue_InContainer(Target.TargetObject);
				}
				else
				{
					const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Target.TargetProperty);
					if (BoolProperty)
					{
						Value = BoolProperty->GetPropertyValue_InContainer(Target.TargetObject) ? 1.f : 0.f;
					}
				}
			}
		}

		static constexpr int32 NumColors = 8;
		Label = TelemetryArray[TelemetryIdx];
		Label = Label.Append(" : ");
		Label = Label.Append(FString::Printf(TEXT("%0.3f"), Value));

		FColor CurrentColor = Colors[TelemetryIdx % NumColors];

		DrawLabel(InCanvas, Label, CurrentColor, CurrX, CurrY, GraphW, OutX, CurrY);

		++TelemetryIdx;
	}


	CurrY += YSep;

	//display stats for weapon + all attached modules
	const FString StatTelemetry = ConsoleVarTelemetryStats.GetValueOnGameThread();
	const AOPAG_Player* Player = GetOwner<AOPAG_Player>();
	if (StatTelemetry.Equals("Weapon", ESearchCase::IgnoreCase))
	{
		const AOPAG_ModularGun* PrimaryGun = Player->GetPrimaryGun();
		const FOPAG_GunStats Stats = PrimaryGun->GetStatsTotal();
		FColor RarityColor;

		GetRarityColor(PrimaryGun->GetGunRarity(), RarityColor);
		DrawStats(InCanvas, StatTelemetry, Stats, RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);

		const AOPAG_ModuleBase* Module = PrimaryGun->GetModuleBySocketType(EModuleTypes::Barrel);
		if (Module)
		{
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, "Barrel", Module->GetStats(), RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);
		}
		Module = PrimaryGun->GetModuleBySocketType(EModuleTypes::Stock);
		if (Module)
		{
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, "Stock", Module->GetStats(), RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);
		}

		Module = PrimaryGun->GetModuleBySocketType(EModuleTypes::Grip);
		if (Module)
		{
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, "Grip", Module->GetStats(), RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);
		}

		Module = PrimaryGun->GetModuleBySocketType(EModuleTypes::Magazine);
		if (Module)
		{
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, "Magazine", Module->GetStats(), RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);
		}

		Module = PrimaryGun->GetModuleBySocketType(EModuleTypes::Sight);
		if (Module)
		{
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, "Sight", Module->GetStats(), RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);
		}

		Module = PrimaryGun->GetModuleBySocketType(EModuleTypes::Accessory);
		if (Module)
		{
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, "Accessory", Module->GetStats(), RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);
		}


	}
	//display stats for singular module
	else
	{
		const AOPAG_ModuleBase* Module = nullptr;
		if (StatTelemetry.Equals("Barrel", ESearchCase::IgnoreCase))
		{
			Module = Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Barrel);
		}
		else if (StatTelemetry.Equals("Stock", ESearchCase::IgnoreCase))
		{
			Module = Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Stock);
		}
		else if (StatTelemetry.Equals("Grip", ESearchCase::IgnoreCase))
		{
			Module = Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Grip);
		}
		else if (StatTelemetry.Equals("Magazine", ESearchCase::IgnoreCase))
		{
			Module = Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Magazine);
		}
		else if (StatTelemetry.Equals("Sight", ESearchCase::IgnoreCase))
		{
			Module = Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Sight);
		}
		else if (StatTelemetry.Equals("Accessory", ESearchCase::IgnoreCase))
		{
			Module = Player->GetPrimaryGun()->GetModuleBySocketType(EModuleTypes::Accessory);
		}

		if (Module)
		{
			const FOPAG_GunStats Stats = Module->GetStats();
			FColor RarityColor;
			GetRarityColor(Module->GetRarity(), RarityColor);
			DrawStats(InCanvas, StatTelemetry, Stats, RarityColor, CurrX, CurrY, GraphW, OutX, CurrY);

		}
	}
}

#pragma optimize("", on)
