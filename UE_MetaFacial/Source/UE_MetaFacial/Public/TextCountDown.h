// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextCountDown.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerFinished);

UCLASS()
class UE_METAFACIAL_API UTextCountDown : public UUserWidget
{
	GENERATED_BODY()

public:	// Variables
	int32 CurrentSeconds = 0.0f;
	FString Message = TEXT("");
	FTimerHandle TimerHandle;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> CountText;

	UPROPERTY(BlueprintAssignable)
	FOnTimerFinished OnTimerFinished;

public:	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartCountdown(const FString& InMessage, int32 InSeconds = 5);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateCountdown();
};
