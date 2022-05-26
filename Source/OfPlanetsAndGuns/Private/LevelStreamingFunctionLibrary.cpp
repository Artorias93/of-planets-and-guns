// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingFunctionLibrary.h"


bool ULevelStreamingFunctionLibrary::AddToDataTable(UPARAM(Ref)FLevelOffset& levelOffsetRow, UDataTable* dataTable)
{
	if (dataTable == nullptr) return false;
	dataTable->AddRow(levelOffsetRow.LevelName, levelOffsetRow);


	return true;
}
