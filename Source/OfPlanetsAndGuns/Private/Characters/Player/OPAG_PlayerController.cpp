#include "Characters/Player/OPAG_PlayerController.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_PlayerController::AOPAG_PlayerController()
{
	
}

void AOPAG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(const AOPAG_Player* const PlayerPawn = Cast<AOPAG_Player>(GetPawn()))
	{
		MappingContext = PlayerPawn->MappingContext;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSubsystem->AddMappingContext(MappingContext, 0);
}
