#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "OPAG_UWHowToPlayView.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWHowToPlayView : public UUserWidget
{
	GENERATED_BODY()
	
	//Variables

	UPROPERTY(meta = (BindWidget))
	UButton* PreviousButton;
	UPROPERTY(meta = (BindWidget))
	UButton* NextButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	//Functions
	
	void NativeConstruct();

	UFUNCTION()
	void PreviousButtonClicked();
	UFUNCTION()
	void NextButtonClicked();
	UFUNCTION()
	void BackButtonClicked();	
};