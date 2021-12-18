// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAttack.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"


// Sets default values
ARangedAttack::ARangedAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangedVolume = CreateDefaultSubobject<USphereComponent>(TEXT("RangedVolume"));
	RootComponent = RangedVolume;
	RangedVolume->SetSphereRadius(25.0f);
	RangedVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RangedVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RangedVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RangedVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RangedVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

//	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComponent"));
//	ParticleComponent->SetupAttachment(GetRootComponent());

	StartPoint = FVector(0.0f);
	EndPoint = FVector(0.0f);
	InterpSpeed = 300.0f;
	MoveDistance = 1200.0f;
	Damage = 5.0f;

	
}

// Called when the game starts or when spawned
void ARangedAttack::BeginPlay()
{
	Super::BeginPlay();
	
	TargetPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));


	StartPoint = GetActorLocation();
	const FRotator MoveRotation = UKismetMathLibrary::FindLookAtRotation(StartPoint, TargetPlayer->GetActorLocation());
	const FVector Direction = FRotationMatrix(MoveRotation).GetUnitAxis(EAxis::X);
	EndPoint = StartPoint + Direction * MoveDistance;

	InterpSpeed += GetWorld()->GetTimeSeconds() * 5;
	Damage += GetWorld()->GetTimeSeconds() / 10;

	RangedVolume->OnComponentBeginOverlap.AddDynamic(this, &ARangedAttack::OnRangedVolumeOverlapBegin);
	RangedVolume->OnComponentEndOverlap.AddDynamic(this, &ARangedAttack::OnRangedVolumeOverlapEnd);

}

// Called every frame
void ARangedAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector CurrentLocation = GetActorLocation();
	const FVector NextLocation = FMath::VInterpConstantTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
	SetActorLocation(NextLocation);

	const float NowDistance = (EndPoint - CurrentLocation).Size();
	if (NowDistance <= 0.5f)
	{
		Destroy();
	}

}

void ARangedAttack::OnRangedVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			if (OverlapParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticle, GetActorLocation(), FRotator(0.0f), false);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			Destroy();
			UGameplayStatics::ApplyDamage(MainPlayer, Damage, nullptr, this, DamageTypeClass);
		}
	}
}

void ARangedAttack::OnRangedVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

