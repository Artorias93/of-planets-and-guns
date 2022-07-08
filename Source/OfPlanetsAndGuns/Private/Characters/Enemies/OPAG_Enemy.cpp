#include "Characters/Enemies/OPAG_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void AOPAG_Enemy::UpdateSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
