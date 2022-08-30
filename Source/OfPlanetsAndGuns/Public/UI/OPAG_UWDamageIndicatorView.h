#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Player/OPAG_Player.h"
#include "OPAG_UWDamageIndicatorView.generated.h"

class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWDamageIndicatorView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* CurrentThreatActor;

	UPROPERTY()
	AOPAG_Player* Player;
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* DamageIndicatorImage;

	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentThreatActor() const;
	UFUNCTION(BlueprintCallable)
	void SetCurrentThreatActor(AActor* const Causer);
	UFUNCTION(BlueprintCallable)
	AOPAG_Player* GetPlayer() const;	
};
