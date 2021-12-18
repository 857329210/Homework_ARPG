// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Homework_ARPG/Homework_ARPGGameModeBase.h"
#include "MyHomework_ARPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK_ARPG_API AMyHomework_ARPGGameMode : public AHomework_ARPGGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Widgets")
	TSubclassOf<class UUserWidget> MainUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI Widgets")
	UUserWidget* MainUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Widgets")
	TSubclassOf<class UUserWidget> DeathUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI Widgets")
	UUserWidget* DeathUI;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void PlayDeathUI();

};
