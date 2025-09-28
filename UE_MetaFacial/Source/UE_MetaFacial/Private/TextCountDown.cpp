// Fill out your copyright notice in the Description page of Project Settings.


#include "TextCountDown.h"

#include "Components/TextBlock.h"

void UTextCountDown::StartCountdown_Implementation(const FString& InMessage, int32 InSeconds)
{
	Message = InMessage;
	CurrentSeconds = InSeconds;
	
	FString timerText = FString::Printf(TEXT("%d%s"), CurrentSeconds, *Message);
	CountText->SetText(FText::FromString(timerText));
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTextCountDown::UpdateCountdown, 1, true);
}

void UTextCountDown::UpdateCountdown_Implementation()
{
	CurrentSeconds--;

	FString timerText = FString::Printf(TEXT("%d%s"), CurrentSeconds, *Message);
	CountText->SetText(FText::FromString(timerText));
	
	if (CurrentSeconds <= 0){
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(); // 화면별로 다른 바인딩이 실행됨
	}
}