#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "OPAG_PlayerController.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AOPAG_PlayerController();

private:

	virtual void BeginPlay() override;

	UPROPERTY()
	UInputMappingContext* MappingContext;
};
