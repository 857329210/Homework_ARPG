// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredExplosion.generated.h"

UCLASS()
class HOMEWORK_ARPG_API ATriggeredExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredExplosion();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* TriggerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* ExplosionRangeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | Explosion")
	TSet<AActor*> ActorsInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Explosion")
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Explosion")
	TSubclassOf<UDamageType> DamageTypeClass;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnExplosionRangeBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplosionRangeBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Triggered Explosion|Trigger Switch")
	void Explode();

	UFUNCTION(BlueprintCallable, Category = "Triggered Explosion|Trigger Switch")
	void ApplyExplodeDamage();


};
