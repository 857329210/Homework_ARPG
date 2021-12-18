// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"
#include "BaseEnemy.h"
#include "SpiderEnemyMelee.h"
#include "SpiderEnemyRanged.h"


void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	BaseEnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
	SpiderEnemyMeleeRef = Cast<ASpiderEnemyMelee>(TryGetPawnOwner());
	SpiderEnemyRangedRef = Cast<ASpiderEnemyRanged>(TryGetPawnOwner());
}

void UBaseEnemyAnimInstance::UpdateAnimationProperties()
{
	if (BaseEnemyRef == nullptr)
	{
		BaseEnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
	}

	if (BaseEnemyRef)
	{
		const FVector SpeedVector = BaseEnemyRef->GetVelocity();
		const FVector PlanarSpeed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f);
		Speed = PlanarSpeed.Size();
	}
}
