// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EEMS_Idle			UMETA(DisplayName = "Idle"),
	EEMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EEMS_Escape			UMETA(DisplayName = "Escape"),
	EEMS_Attacking		UMETA(DisplayName = "Attacking"),
	EEMS_Dead			UMETA(DisplayName = "Dead")
};


UCLASS()
class HOMEWORK_ARPG_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* ChaseVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AttackVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bAttackVolumeOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bChaseVolumeOverlapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UAnimMontage* AttackMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bInterpToPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Status")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Status")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Status")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Status")
	class UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Status")
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Status")
	int32 EnemyValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainPlayer* TargetPlayer);

	FORCEINLINE bool IsAlive() { return EnemyMovementStatus != EEnemyMovementStatus::EEMS_Dead; }

	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	bool HasValidTarget();
};
