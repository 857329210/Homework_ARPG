// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK_ARPG_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class ABaseEnemy* BaseEnemyRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class ASpiderEnemyMelee* SpiderEnemyMeleeRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class ASpiderEnemyRanged* SpiderEnemyRangedRef;

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BluePrintCallable, Category = "Animation Properties")
	void UpdateAnimationProperties();
	
};
