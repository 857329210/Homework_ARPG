// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderEnemyRanged.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"


ASpiderEnemyRanged::ASpiderEnemyRanged()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = true;

	EscapeVolume = CreateDefaultSubobject<USphereComponent>(TEXT("EscapeVolume"));
	EscapeVolume->SetupAttachment(GetRootComponent());
	EscapeVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EscapeVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	EscapeVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EscapeVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	EscapeVolume->InitSphereRadius(300.0f);

	ChaseVolume->InitSphereRadius(1200.0f);

	AttackVolume->InitSphereRadius(600.0f);

	SpawnRangedAttackLocation = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnRangedAttackLocation"));
	SpawnRangedAttackLocation->SetupAttachment(GetRootComponent());
	SpawnRangedAttackLocation->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bEscapeVolumeOverlapping = false;

}

void ASpiderEnemyRanged::BeginPlay()
{
	Super::BeginPlay();

	EscapeVolume->OnComponentBeginOverlap.AddDynamic(this, &ASpiderEnemyRanged::OnEscapeVolumeOverlapBegin);
	EscapeVolume->OnComponentEndOverlap.AddDynamic(this, &ASpiderEnemyRanged::OnEscapeVolumeOverlapEnd);
}

void ASpiderEnemyRanged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpiderEnemyRanged::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

}

void ASpiderEnemyRanged::OnEscapeVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bEscapeVolumeOverlapping = true;
			if (EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
			{
				EscapeFromTarget(MainPlayer);
			}
		}
	}
}

void ASpiderEnemyRanged::OnEscapeVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bEscapeVolumeOverlapping = false;
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
			if (AIController)
			{
				AIController->StopMovement();
			}
			Attack();
		}
	}
}


void ASpiderEnemyRanged::Attack()
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
				AnimInstance->Montage_Play(AttackMontage);
			}
		}
	}
}

void ASpiderEnemyRanged::AttackEnd()
{
	bInterpToPlayer = false;
	if (IsAlive())
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

		if (bEscapeVolumeOverlapping)
		{
			EscapeFromTarget(Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0)));
		}
		else if (bAttackVolumeOverlapping)
		{
			Attack();
		}
		else if (bChaseVolumeOverlapping)
		{
			MoveToTarget(Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0)));
		}
		else
		{
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
		}
	}
}

void ASpiderEnemyRanged::SpawnRangedAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("SpawnRangedAttack()"));
	if (RangedAttack)
	{
		GetWorld()->SpawnActor<AActor>(RangedAttack, SpawnRangedAttackLocation->GetComponentLocation(), FRotator(0.0f));
	}
}

void ASpiderEnemyRanged::EscapeFromTarget(class AMainPlayer* TargetPlayer)
{
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Escape;

	const FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, GetActorLocation());
	const FRotator MoveRotation(0.0f, LookAtRotation.Yaw, 0.0f);

	const FVector GoalLocation = FRotationMatrix(MoveRotation).GetUnitAxis(EAxis::X) * 1000.0f;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(GoalLocation);

	AIController->MoveTo(MoveRequest);
}


