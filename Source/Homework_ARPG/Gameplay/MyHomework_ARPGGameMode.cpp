// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHomework_ARPGGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Homework_ARPG/Characters/Player/MainPlayer.h"

void AMyHomework_ARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIClass)
	{
		MainUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), MainUIClass);
		if (MainUI)
		{
			MainUI->AddToViewport();
		}
	}
}

void AMyHomework_ARPGGameMode::PlayDeathUI()
{
	if (DeathUIClass)
	{
		DeathUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), DeathUIClass);
		if (DeathUI)
		{
			DeathUI->AddToViewport();
		}
	}
}
