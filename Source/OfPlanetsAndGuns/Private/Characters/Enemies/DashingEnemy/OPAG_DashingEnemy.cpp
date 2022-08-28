#include "Characters/Enemies/DashingEnemy/OPAG_DashingEnemy.h"

// void AOPAG_DashingEnemy::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	//Sets delegate functions that disable pawn rotation at the start of the attack animation and disables it at the end.
// 	//Collision checks and damage computations are only executed when the notify is encountered in the animation.
// 	AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
// 	if (MeleeAttackMontage && AnimInstance)
// 	{
// 		AnimInstance->OnMontageStarted.AddDynamic(this, &AOPAG_DashingEnemy::OnAttackBegin);
// 		AnimInstance->OnMontageEnded.AddDynamic(this, &AOPAG_DashingEnemy::OnAttackEnd);
// 	}
// }
//
// //Stops the enemy's ability to turn in the direction of the focus actor during the attack animation 
// void AOPAG_DashingEnemy::OnAttackBegin(UAnimMontage* Montage)
// {
// 	//Only the dashing enemy uses the AIController's rotation
// 	//Locks the horizontal rotation for the duration of the attack animation
// 	bUseControllerRotationYaw = false;
// 	
// 	//Locks the Y axis of the rotation so that the enemy is never rotated upwards or downwards while attacking
// 	SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, GetActorRotation().Roll));
// }
//
// //Resumes the enemy's ability to turn in the direction of the focus actor after the attack animation is done
// void AOPAG_DashingEnemy::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
// {
// 	//Resumes the horizontal rotation following the controller's
// 	bUseControllerRotationYaw = true;
// }