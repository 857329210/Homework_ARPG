// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthDrug.h"
#include "Components/SphereComponent.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AHealthDrug::AHealthDrug()
{
	TriggerVolume->SetSphereRadius(50.0f);
	HealRecoverey = 40.0f;
}


// Called when the game starts or when spawned
void AHealthDrug::BeginPlay()
{
	Super::BeginPlay();

	//TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AHealthDrug::OnOverlapBegin);
	//TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AHealthDrug::OnOverlapEnd);

}


// Called every frame
void AHealthDrug::Tick(float DeltaTime)
{

}

void AHealthDrug::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer && (MainPlayer->Health < MainPlayer->MaxHealth))
		{
			const FVector ActorLocation = GetActorLocation();
			if (OverlapParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticle, ActorLocation, FRotator(0.0f), true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
			MainPlayer->IncreaseHealth(HealRecoverey);
			
			Destroy();

			/****************
			Set to make the health drug reappear 40 seconds after being consumed
			******************/
			//DisplayMesh->SetVisibility(false);
			//TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			//if (RepalceTemp)
			//{
			//	FTimerHandle ReplaceTimerHandle;
			//	auto Lambda = [this]()
			//	{
			//		Destroy();
			//		GetWorld()->SpawnActor<AActor>(RepalceTemp, GetActorLocation(), FRotator(0.0f));
			//	};
			//	GetWorldTimerManager().SetTimer(ReplaceTimerHandle, FTimerDelegate::CreateLambda(Lambda), 40.0f, false);
			//}
		}
	}
}

void AHealthDrug::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
