#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWConfirmExitView.generated.h"

class UButton;
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWConfirmExitView : public UUserWidget
{
	GENERATED_BODY()

	//Variables

	UPROPERTY(meta = (BindWidget))
	UButton* ExitYesButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitNoButton;

	//Functions

public:
	
	void NativeConstruct();

private:
	
	void BindEvents();

	UFUNCTION()
	void ExitYesButtonClicked();
	UFUNCTION()
	void ExitNoButtonClicked();
};
