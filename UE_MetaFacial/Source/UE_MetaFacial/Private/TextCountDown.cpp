// Fill out your copyright notice in the Description page of Project Settings.


#include "TextCountDown.h"

#include "Components/TextBlock.h"

void UTextCountDown::StartCountdown_Implementation(int32 InSeconds)
{
	FString timerText = FString::Printf(TEXT("%d"), CurrentSeconds);
	CountText->SetText(FText::FromString(timerText));
	
	CurrentSeconds = InSeconds;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTextCountDown::UpdateCountdown, 1, true);
}

void UTextCountDown::UpdateCountdown_Implementation()
{
	CurrentSeconds--;
	
	if (CurrentSeconds <= 0){
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(); // 화면별로 다른 바인딩이 실행됨
	}
}