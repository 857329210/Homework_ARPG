// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnimInstance.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
}



void UMainPlayerAnimInstance::UpdateAnimationProperties()
{
	if (PlayerRef == nullptr)
	{
		PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	if (PlayerRef)
	{
		//bIsInAir = PlayerRef->GetCharacterMovement()->IsFalling();
		bIsInAir = PlayerRef->GetMovementComponent()->IsFalling();

		const FVector Velocity = PlayerRef->GetVelocity();
		const FVector PlanarVelocity(Velocity.X, Velocity.Y, 0.0f);
		Speed = PlanarVelocity.Size();

		const FRotator PlayerRotation = PlayerRef->GetActorRotation();
		Directional = CalculateDirection(PlanarVelocity, PlayerRotation);
	}
}
