// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextCountDown.generated.h"

/**
 * 
 */
UCLASS()
class UE_METAFACIAL_API UTextCountDown : public UUserWidget
{
	GENERATED_BODY()

public:	// Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalSeconds = 5.0f;

	int32 CurrentSeconds = 0.0f;
	FTimerHandle TimerHandle;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> CountText;

public:	// Functions
	UFUNCTION(BlueprintNativeEvent)
	void InitializeCountdown(int32 InSeconds = 5.0f);

	UFUNCTION(BlueprintNativeEvent)
	void StartCountdown();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateCountdown();
};
