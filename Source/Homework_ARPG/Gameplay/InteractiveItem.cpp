// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AInteractiveItem::AInteractiveItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());

	bNeedRotate = true;
	RotationRate = 15.0f;

}

// Called when the game starts or when spawned
void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveItem::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveItem::OnOverlapEnd);
}

// Called every frame
void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bNeedRotate)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += RotationRate * DeltaTime;
		SetActorRotation(NewRotation);
	}
}

void AInteractiveItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AInteractiveItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

