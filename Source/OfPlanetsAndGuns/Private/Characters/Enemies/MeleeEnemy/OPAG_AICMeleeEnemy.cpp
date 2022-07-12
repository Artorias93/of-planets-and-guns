#include "Characters/Enemies/MeleeEnemy/OPAG_AICMeleeEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

AOPAG_AICMeleeEnemy::AOPAG_AICMeleeEnemy()
{
	PerceptionSightConfig->SightRadius = SightSenseRadius;
	PerceptionSightConfig->LoseSightRadius = LoseSightSenseRadius;
	PerceptionSightConfig->PeripheralVisionAngleDegrees = SightPeripheralVisionDegrees;

	PerceptionComp->ConfigureSense(*PerceptionSightConfig);
	PerceptionComp->ConfigureSense(*PerceptionDamageConfig);
	PerceptionComp->SetDominantSense(PerceptionSightConfig->GetSenseImplementation());
}