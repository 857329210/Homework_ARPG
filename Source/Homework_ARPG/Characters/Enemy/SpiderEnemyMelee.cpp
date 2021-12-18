// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderEnemyMelee.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"



ASpiderEnemyMelee::ASpiderEnemyMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ChaseVolume->InitSphereRadius(850.0f);
	
	AttackVolume->InitSphereRadius(125.0f);

	LeftAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftAttackCollision"));
	LeftAttackCollision->SetupAttachment(GetMesh(), "LeftAttackSocket");
	DeactivateLeftAttackCollision();

	RightAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightAttackCollision"));
	RightAttackCollision->SetupAttachment(GetMesh(), "RightAttackSocket");
	DeactivateRightAttackCollision();

	Damage = 2.0f;
}

void ASpiderEnemyMelee::BeginPlay()
{
	Super::BeginPlay();

	Damage += GetWorld()->GetTimeSeconds() / 10.0f;

	LeftAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpiderEnemyMelee::OnAttackCollisionOverlapBegin);
	LeftAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ASpiderEnemyMelee::OnAttackCollisionOverlapEnd);

	RightAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpiderEnemyMelee::OnAttackCollisionOverlapBegin);
	RightAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ASpiderEnemyMelee::OnAttackCollisionOverlapEnd);
}

void ASpiderEnemyMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ASpiderEnemyMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

}

void ASpiderEnemyMelee::OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(MainPlayer, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void ASpiderEnemyMelee::OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ASpiderEnemyMelee::ActivateLeftAttackCollision()
{
	LeftAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ASpiderEnemyMelee::DeactivateLeftAttackCollision()
{
	LeftAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpiderEnemyMelee::ActivateRightAttackCollision()
{
	RightAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ASpiderEnemyMelee::DeactivateRightAttackCollision()
{
	RightAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpiderEnemyMelee::Attack()
{
	if (IsAlive() && HasValidTarget())
	{
		if (AIController)
		{
			AIController->StopMovement();
		}
		if (EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
		{
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Attacking;
			bInterpToPlayer = true;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && AttackMontage)
			{
				const float PlayRate = FMath::RandRange(0.9f, 1.1f);
				const FString SectionName = FString::FromInt(FMath::RandRange(1, 3));
				AnimInstance->Montage_Play(AttackMontage, PlayRate);
				AnimInstance->Montage_JumpToSection(FName(*SectionName), AttackMontage);
			}
		}
	}
}
