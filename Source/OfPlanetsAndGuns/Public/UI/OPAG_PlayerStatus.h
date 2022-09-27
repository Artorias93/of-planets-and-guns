#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_PlayerStatus.generated.h"

class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_PlayerStatus : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UImage* ShieldDamageImage;

	UPROPERTY(Category = "OPAG | Widget | Timer", EditDefaultsOnly, BlueprintReadWrite)
	float StartClearTimerAmount = 1.f;

	UPROPERTY(Category = "OPAG | Widget | Timer", EditDefaultsOnly, BlueprintReadWrite)
	float ClearIntepAmount = .1f;

private:
	FTimerHandle ClearShieldDamageHanlde;

public:
	void NativeConstruct();

private:
	void BindEvents();
	UFUNCTION() void UpdateShiedlDamage(const float Value);
	UFUNCTION() void ClearShieldDamageTimer();
};
