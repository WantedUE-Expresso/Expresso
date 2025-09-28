#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpClientSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(NetHttpLog, Display, Log)

UCLASS()
class UE_METAFACIAL_API UHttpClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// TODO: HTTP 요청 함수들 구현 예정
	// UFUNCTION(BlueprintCallable, Category = "HTTP")
	// bool SendGetRequest(const FString& URL);

	// UFUNCTION(BlueprintCallable, Category = "HTTP")
	// bool SendPostRequest(const FString& URL, const FString& JsonData);
};
