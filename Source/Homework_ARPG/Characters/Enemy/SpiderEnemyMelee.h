// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Homework_ARPG/Characters/Enemy/BaseEnemy.h"
#include "SpiderEnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK_ARPG_API ASpiderEnemyMelee : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpiderEnemyMelee();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class UBoxComponent* LeftAttackCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	class UBoxComponent* RightAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateLeftAttackCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateLeftAttackCollision();

	UFUNCTION(BlueprintCallable)
	void ActivateRightAttackCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateRightAttackCollision();

	virtual void Attack() override;

};
