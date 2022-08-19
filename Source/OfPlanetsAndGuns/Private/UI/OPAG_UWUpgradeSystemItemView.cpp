// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWUpgradeSystemItemView.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Characters/OPAG_AttributeSetBase.h"

void UOPAG_UWUpgradeSystemItemView::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UOPAG_AttributeSetBase* Item = Cast<UOPAG_AttributeSetBase>(ListItemObject);	

}
