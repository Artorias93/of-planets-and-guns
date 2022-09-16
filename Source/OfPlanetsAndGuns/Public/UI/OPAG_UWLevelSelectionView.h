#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWLevelSelectionView.generated.h"

class UButton;
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWLevelSelectionView : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Planet1ButtonClicked();
	UFUNCTION()
	void BackButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* Planet1Button;
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
};
