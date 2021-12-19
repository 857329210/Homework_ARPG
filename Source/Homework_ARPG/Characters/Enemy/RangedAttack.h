// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedAttack.generated.h"

UCLASS()
class HOMEWORK_ARPG_API ARangedAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* RangedVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	FVector StartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	FVector EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float MoveDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	class AMainPlayer* TargetPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* OverlapParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class USoundCue* OverlapSound;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnRangedVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRangedVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
