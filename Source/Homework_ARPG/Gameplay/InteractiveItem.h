// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.generated.h"

UCLASS()
class HOMEWORK_ARPG_API AInteractiveItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UMeshComponent* DisplayMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Item | Particles")
	class UParticleSystem* OverlapParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Item | Sounds")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Item | Item Properties")
	bool bNeedRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Item | Item Properties")
	float RotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
