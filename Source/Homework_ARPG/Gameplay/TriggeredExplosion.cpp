// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggeredExplosion.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"
#include "Homework_ARPG/Characters/Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
//#include "NiagaraComponent.h"

// Sets default values
ATriggeredExplosion::ATriggeredExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	RootComponent = TriggerMesh;
	

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	ExplosionRangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ExplosionRange"));
	ExplosionRangeBox->SetupAttachment(GetRootComponent());

	ExplosionDamage = 50.0f;

}

// Called when the game starts or when spawned
void ATriggeredExplosion::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggeredExplosion::OnTriggerBoxOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggeredExplosion::OnTriggerBoxOverlapEnd);

	ExplosionRangeBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggeredExplosion::OnExplosionRangeBoxOverlapBegin);
	ExplosionRangeBox->OnComponentEndOverlap.AddDynamic(this, &ATriggeredExplosion::OnExplosionRangeBoxOverlapEnd);
}

// Called every frame
void ATriggeredExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATriggeredExplosion::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			Explode();
		}
	}
}

void ATriggeredExplosion::OnTriggerBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ATriggeredExplosion::OnExplosionRangeBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
		if (MainPlayer || Enemy)
		{
			ActorsInRange.Add(OtherActor, nullptr);
		}
	}
}

void ATriggeredExplosion::OnExplosionRangeBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
		if (MainPlayer || Enemy)
		{
			ActorsInRange.Remove(OtherActor);
		}
	}
}

void ATriggeredExplosion::ApplyExplodeDamage()
{
	for (auto& ActorInRange : ActorsInRange)
	{
		UGameplayStatics::ApplyDamage(ActorInRange, ExplosionDamage, nullptr, this, DamageTypeClass);
	}
}


