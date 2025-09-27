// Fill out your copyright notice in the Description page of Project Settings.


#include "TextCountDown.h"

#include "Components/TextBlock.h"


void UTextCountDown::InitializeCountdown_Implementation(int32 InSeconds)
{
	TotalSeconds = InSeconds;
	CurrentSeconds = TotalSeconds;
}

void UTextCountDown::StartCountdown_Implementation()
{
	
	FString timerText = FString::Printf(TEXT("%d"), CurrentSeconds);
	CountText->SetText(FText::FromString(timerText));
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTextCountDown::UpdateCountdown, 1, false);
}

void UTextCountDown::UpdateCountdown_Implementation()
{
	CurrentSeconds--;
	
	FString timerText = FString::Printf(TEXT("%d"), CurrentSeconds);
	CountText->SetText(FText::FromString(timerText));

	if (CurrentSeconds > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTextCountDown::UpdateCountdown, 1, false);
	}
}