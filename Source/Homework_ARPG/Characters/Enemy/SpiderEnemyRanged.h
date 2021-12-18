// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Homework_ARPG/Characters/Enemy/BaseEnemy.h"
#include "SpiderEnemyRanged.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK_ARPG_API ASpiderEnemyRanged : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpiderEnemyRanged();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Attack")
	class TSubclassOf<AActor> RangedAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Attack")
	class USphereComponent* SpawnRangedAttackLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranged Attack")
	class USphereComponent* EscapeVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranged Attack")
	bool bEscapeVolumeOverlapping;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnEscapeVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEscapeVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Attack() override;

	virtual void AttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void SpawnRangedAttack();

	void EscapeFromTarget(class AMainPlayer* TargetPlayer);
};
