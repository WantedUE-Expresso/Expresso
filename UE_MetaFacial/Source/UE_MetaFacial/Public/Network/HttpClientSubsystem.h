#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpClientSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(NetHttpLog, Display, Log)

// TODO: HTTP 클라이언트 서브시스템 구현 예정
// 용도: REST API 통신, 파일 업로드/다운로드, 웹 서비스 연동 등
// 현재 프로젝트에서는 WebSocket 통신만 사용하므로 미구현 상태
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
