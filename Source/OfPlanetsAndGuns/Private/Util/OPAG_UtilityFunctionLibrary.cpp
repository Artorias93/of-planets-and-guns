// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/OPAG_UtilityFunctionLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

void UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(const UObject* WorldContextObject, const FString& Message,
                                                              const EMessageSeverity::Type Severity)
{
	UKismetSystemLibrary::PrintString(WorldContextObject,
		Message,
		true, true,
		Severity == EMessageSeverity::Warning ? FColor::Yellow :
		(Severity == EMessageSeverity::Error || Severity == EMessageSeverity::CriticalError ? FColor::Red :
			FColor::Green),
		5.f);
}