// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWModuleStatEntryView.h"
#include "UI/OPAG_ModuleStatItem.h"
#include "Components/TextBlock.h"

void UOPAG_UWModuleStatEntryView::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UOPAG_ModuleStatItem* Item = Cast<UOPAG_ModuleStatItem>(ListItemObject);
	if (Item)
	{
		Name->SetText(FText::FromString(Item->Name));
		Value->SetText(FText::FromString(Item->Value));
		if(Item->DeltaSign == EDeltaSign::Positive)
			Delta->SetText(FText::FromString("(+" + Item->Delta + ")"));
		else if(Item->DeltaSign == EDeltaSign::Negative)
			Delta->SetText(FText::FromString("(" + Item->Delta + ")"));
		else
			Delta->SetText(FText::FromString(Item->Delta));

		if(Item->bIsInverted)
		{
			FLinearColor temp = GreenColor;
			GreenColor = RedColor;
			RedColor = temp;
		}

		if (Item->DeltaSign == EDeltaSign::Negative) 
		{
			Name->SetColorAndOpacity(RedColor);
			//Value->SetColorAndOpacity(RedColor);
			Delta->SetColorAndOpacity(RedColor);
		}
		else if (Item->DeltaSign == EDeltaSign::Positive) 
		{
			Name->SetColorAndOpacity(GreenColor);
			//Value->SetColorAndOpacity(GreenColor);
			Delta->SetColorAndOpacity(GreenColor);
		}
		else
		{
			Name->SetColorAndOpacity(NeutralColor);
			//Value->SetColorAndOpacity(GreenColor);
			Delta->SetColorAndOpacity(NeutralColor);
		}
	}
}