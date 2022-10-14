#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWCreditsView.generated.h"

class UButton;
class UCanvasPanel;
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWCreditsView : public UUserWidget
{
	GENERATED_BODY()
	
	//Variables

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CreditsPage;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	//Functions
	
	void NativeConstruct();
	
	UFUNCTION()
	void BackButtonClicked();	
};