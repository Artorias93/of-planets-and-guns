#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_EnemyHealthBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnemyProgressBar;

public:
	void NativeConstruct();
	UProgressBar* GetProgressBar();
	UFUNCTION(BlueprintCallable) 
		void SetHealthPercent(const float Value);
};
