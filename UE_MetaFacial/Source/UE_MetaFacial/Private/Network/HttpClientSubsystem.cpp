#include "Network/HttpClientSubsystem.h"

DEFINE_LOG_CATEGORY(NetHttpLog)

void UHttpClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// TODO: HTTP 클라이언트 초기화 로직 구현 예정
	UE_LOG(NetHttpLog, Log, TEXT("HttpClientSubsystem initialized (placeholder)"));
}
