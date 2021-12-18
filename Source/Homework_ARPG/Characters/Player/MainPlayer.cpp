// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Homework_ARPG/Characters/Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Homework_ARPG/Gameplay/MyHomework_ARPGGameMode.h"
#include "Blueprint/UserWidget.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	LeftKatana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftKatana"));
	LeftKatana->SetupAttachment(GetMesh(), "LeftKatanaHandSocket");

	LeftKatanaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftKatanaBox"));
	LeftKatanaBox->SetupAttachment(LeftKatana);
	DeactivateLeftAttackCollision();

	RightKatana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightKatana"));
	RightKatana->SetupAttachment(GetMesh(), "RightKatanaHandSocket");

	RightKatanaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightKatanaBox"));
	RightKatanaBox->SetupAttachment(RightKatana);
	DeactivateRightAttackCollision();

	//EnemySpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemySpawnBox"));
	//EnemySpawnBox->SetupAttachment(GetRootComponent());
	//EnemySpawnBox->SetBoxExtent(FVector(1000.0f, 1000.0f, 32.0f));

	bIsSprinting = false;
	bIsInAir = false;
	bIsDefending = false;

	Health = 100.0f;
	MaxHealth = 100.0f;
	HealthRecoverRate = 3.0f;
	bIsAlive = true;
	PointsNum = 0;
	
	PlayerAttackStatus = EPlayerAttackStatus::EPAS_Normal;
	bInterpToController = false;
	InterpSpeed = 15.0f;
	Damage = 15.0f;

	SpawnTime = 0.0f;
	SpawnTimeThreshold = 5.0f;

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	LeftKatanaBox->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnAttackCollisionOverlapBegin);
	LeftKatanaBox->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::OnAttackCollisionOverlapEnd);
	
	RightKatanaBox->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnAttackCollisionOverlapBegin);
	RightKatanaBox->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::OnAttackCollisionOverlapEnd);

	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlive)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			bIsInAir = false;
		}

		if (bInterpToController)
		{
			const FRotator LookAtRotation = Controller->GetControlRotation();
			const FRotator TargetRotation(0.0f, LookAtRotation.Yaw, 0.0f);
			const FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpSpeed);
			SetActorRotation(InterpRotation);
		}

		IncreaseHealth(HealthRecoverRate * DeltaTime);

		if (SpawnTime > SpawnTimeThreshold)
		{
			SpawnTime = 0.0f;
			SpawnEnemy();
		}
		else
		{
			SpawnTime += DeltaTime;
		}
	}



	


	//UE_LOG(LogTemp, Warning, TEXT("Is Defending"));

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::StopSprint);

	PlayerInputComponent->BindAction("Defend", IE_Pressed, this, &AMainPlayer::Defend);
	PlayerInputComponent->BindAction("Defend", IE_Released, this, &AMainPlayer::StopDefend);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainPlayer::Attack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);
}


void AMainPlayer::MoveForward(float value)
{
	if ((Controller != nullptr) && (value != 0.0f) && bIsAlive)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && bIsAlive)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);//Pitch, Yaw, Roll
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUp(float Value)
{
	//if (!bIsAlive)
	//{
	//	return;
	//}
	if (GetControlRotation().Pitch >= 45.0f && GetControlRotation().Pitch <= 180 && Value < 0.0f)
	{
		return;
	}
	if (GetControlRotation().Pitch <= 270 && GetControlRotation().Pitch > 180 && Value > 0.0f)
	{
		return;
	}
	AddControllerPitchInput(Value);
}

void AMainPlayer::Jump()
{
	if (bIsAlive)
	{
		Super::Jump();
		bIsInAir = true;
	}
}



void AMainPlayer::Sprint()
{
	if (!bIsInAir && bIsAlive)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	}
}

void AMainPlayer::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}


void AMainPlayer::Attack()
{
	if (bIsAttacking || bIsInAir || bIsDefending || !bIsAlive)
	{
		return;
	}
	
	bIsAttacking = true;
	bInterpToController = true;
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	class UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (bIsSprinting && SprintingAttackMontage)
		{
			AnimInstance->Montage_Play(SprintingAttackMontage);
		}

		else if (!bIsSprinting && AttackMontage)
		{	
			int32 SectionNum = 1;
			
			switch (PlayerAttackStatus)
			{
			case EPlayerAttackStatus::EPAS_Normal:
				SectionNum = 1;
				PlayerAttackStatus = EPlayerAttackStatus::EPAS_First;
				break;
			case EPlayerAttackStatus::EPAS_First:
				SectionNum = 2;
				PlayerAttackStatus = EPlayerAttackStatus::EPAS_Second;
				break;
			case EPlayerAttackStatus::EPAS_Second:
				SectionNum = 3;
				PlayerAttackStatus = EPlayerAttackStatus::EPAS_Normal;
				break;
			default:
				break;
			}
			const FString SectionName = FString::FromInt(SectionNum);
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(*SectionName, AttackMontage);
		}
	}
}

void AMainPlayer::AttackEnd()
{
	PlayerAttackStatus = EPlayerAttackStatus::EPAS_Normal;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void AMainPlayer::ChangeAttackStatus()
{
	bIsAttacking = false;
	bInterpToController = false;
}

void AMainPlayer::OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && bIsAlive)
	{
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
		if (Enemy)
		{
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, GetController(), this, DamageTypeClass);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySound2D(this, HitSound);
			}
			//TODO Particle
		}
	}
}

void AMainPlayer::OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMainPlayer::ActivateLeftAttackCollision()
{
	LeftKatanaBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftKatanaBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftKatanaBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftKatanaBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AMainPlayer::DeactivateLeftAttackCollision()
{
	LeftKatanaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::ActivateRightAttackCollision()
{
	RightKatanaBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightKatanaBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightKatanaBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightKatanaBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AMainPlayer::DeactivateRightAttackCollision()
{
	RightKatanaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AMainPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDefending)
	{
		DamageAmount /= 2;
	}
	if (Health - DamageAmount <= 0)
	{
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	return Health;
}

void AMainPlayer::Die()
{
	bIsAlive = false;
	DeactivateLeftAttackCollision();
	DeactivateRightAttackCollision();
}

void AMainPlayer::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	
	FTimerHandle ReplaceTimerHandle;
	GetWorldTimerManager().ClearTimer(ReplaceTimerHandle);

	//FTimerHandle DeathTimerHandle;
	//auto Lambda = [this]()
	//{
	//	//RestartLevel();
	//};
	//GetWorldTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda(Lambda), 1.0f, false);

}

void AMainPlayer::GetPoints(int32 Value)
{
	PointsNum += Value;
}

void AMainPlayer::IncreaseHealth(float Value)
{
	Health = FMath::Clamp(Health + Value, 0.0f, MaxHealth);
}

void AMainPlayer::SpawnEnemy()
{
	const FVector Origin = GetActorLocation();
	const FVector BoxExtent(1000.0f, 1000.0f, 32.0f);
	const FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent);

	TSubclassOf<AActor> SpawnActorClass;
	if (SpawnActorClassesArray.Num() > 0)
	{
		const int index = FMath::RandRange(0, SpawnActorClassesArray.Num() - 1);
		SpawnActorClass = SpawnActorClassesArray[index];
	}

	if (SpawnActorClass)
	{
		GetWorld()->SpawnActor<AActor>(SpawnActorClass, SpawnPoint, FRotator(0.0f));
	}

	//GetWorld()->TimeSeconds;
}

void AMainPlayer::PlayDeathUI(UUserWidget* DeathUI)
{
	DeathUI->AddToViewport();
}

