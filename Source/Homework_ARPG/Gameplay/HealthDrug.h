// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "HealthDrug.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK_ARPG_API AHealthDrug : public AInteractiveItem
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AHealthDrug();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealRecoverey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TSubclassOf<AActor> RepalceTemp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
