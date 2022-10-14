#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "OPAG_UWHowToPlayView.generated.h"

class UButton;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWHowToPlayView : public UUserWidget
{
	GENERATED_BODY()
	
	//Variables

	//int CurrentPage;

	// UPROPERTY(meta = (BindWidget))
	// UCanvasPanel* Pag0;
	// UPROPERTY(meta = (BindWidget))
	// UCanvasPanel* Pag1;
	// UPROPERTY(meta = (BindWidget))
	// UCanvasPanel* Pag2;
	//
	// UPROPERTY()
	// TArray<UCanvasPanel*> PagesList;
	
private:
	// UPROPERTY(meta = (BindWidget))
	// UButton* PreviousButton;
	// UPROPERTY(meta = (BindWidget))
	// UButton* NextButton;
	// UPROPERTY(meta = (BindWidget))
	// UButton* BackButton;

	//Functions
	
	void NativeConstruct();

	// UFUNCTION()
	// void PreviousButtonClicked();
	// UFUNCTION()
	// void NextButtonClicked();
	// UFUNCTION()
	// void BackButtonClicked();	
};