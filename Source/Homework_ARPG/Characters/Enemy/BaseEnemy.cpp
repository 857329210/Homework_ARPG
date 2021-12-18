// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChaseVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseVolume"));
	ChaseVolume->SetupAttachment(GetRootComponent());
	ChaseVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ChaseVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ChaseVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ChaseVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(125.0f, 10.0f));


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bAttackVolumeOverlapping = false;
	bChaseVolumeOverlapping = false;
	bInterpToPlayer = false;
	InterpSpeed = 15.0f;

	MaxHealth = 20.0f;
	Health = MaxHealth;
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

	
	EnemyValue = 1;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	ChaseVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapBegin);
	ChaseVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapEnd);

	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapBegin);
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapEnd);


	MaxHealth += GetWorld()->GetTimeSeconds();
	Health = MaxHealth;
	

	HealthBar = Cast<UProgressBar>(HealthBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName("HealthBar"));
	HealthBar->SetPercent(Health / MaxHealth);
	HealthBar->SetVisibility(ESlateVisibility::Hidden);

	AIController = Cast<AAIController>(GetController());


}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterpToPlayer)
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation());
		const FRotator TargetRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
		const FRotator InterpRorarion = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRorarion);
	}


}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bChaseVolumeOverlapping = true;
			HealthBar->SetVisibility(ESlateVisibility::Visible);
			MoveToTarget(MainPlayer);
		}
	}
}

void ABaseEnemy::OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bChaseVolumeOverlapping = false;
			HealthBar->SetVisibility(ESlateVisibility::Hidden);
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bAttackVolumeOverlapping = true;
			Attack();
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && IsAlive())
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bAttackVolumeOverlapping = false;
			if (EnemyMovementStatus != EEnemyMovementStatus::EEMS_Attacking)
			{
				MoveToTarget(MainPlayer);
			}
		}
	}
}

void ABaseEnemy::MoveToTarget(AMainPlayer* TargetPlayer)
{
	if (AIController && TargetPlayer && IsAlive())
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_MoveToTarget;
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(TargetPlayer);
		MoveRequest.SetAcceptanceRadius(10.0f);
		FNavPathSharedPtr NavPath;


		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

void ABaseEnemy::Attack()
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

void ABaseEnemy::AttackEnd()
{
	bInterpToPlayer = false;
	if (IsAlive())
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

		if (bAttackVolumeOverlapping)
		{
			Attack();
		}
		else if(bChaseVolumeOverlapping)
		{
			MoveToTarget(Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0)));
		}
		else
		{
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
		}
	}
}



float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0)
	{
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	HealthBar->SetPercent(Health / MaxHealth);

	return Health;
}

void ABaseEnemy::Die()
{
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Dead;
	HealthBar->SetVisibility(ESlateVisibility::Hidden);

	ChaseVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0))->GetPoints(EnemyValue);
}



void ABaseEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	FTimerHandle DeathTimerHandle;
	auto Lambda = [this]()
	{
		Destroy();
		//Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0))->SpawnEnemy();
	};
	GetWorldTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda(Lambda), 1.5f, false);

}

bool ABaseEnemy::HasValidTarget()
{
	return (Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0))->bIsAlive);
}
