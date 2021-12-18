// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"



UENUM(BlueprintType)
enum class EPlayerAttackStatus : uint8
{
	EPAS_Normal		UMETA(Displayname = "Normal"),
	EPAS_First		UMETA(Displayname = "First"),
	EPAS_Second		UMETA(Displayname = "Second"),
};


UCLASS()
class HOMEWORK_ARPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* RightKatana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* LeftKatana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* LeftKatanaBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* RightKatanaBox;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Movement")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Movement")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Attack")
	bool bIsDefending;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Attack")
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Attack")
	EPlayerAttackStatus PlayerAttackStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Attack")
	bool bInterpToController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Attack")
	float InterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Attack")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Status | Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* SprintingAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Status | Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Status | Health")
	float HealthRecoverRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Status | Health")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Status | Points")
	int32 PointsNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Effect")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Effect")
	class USoundCue* HitSound;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Spawn")
	TArray<TSubclassOf<AActor>> SpawnActorClassesArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawn")
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Spawn")
	float SpawnTimeThreshold;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Sprint();
	void StopSprint();
	virtual void Jump() override;

	FORCEINLINE void Defend() { bIsDefending = true; }
	FORCEINLINE void StopDefend() { bIsDefending = false; }

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void ChangeAttackStatus();

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

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void GetPoints(int32 Value);

	void IncreaseHealth(float Value);
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void PlayDeathUI(UUserWidget* DeathUI);

};
