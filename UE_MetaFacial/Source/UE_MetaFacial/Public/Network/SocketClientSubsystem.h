#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "DataStructures.h"
#include "SocketClientSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(NetSocketLog, Display, Log)

// 확장 가능한 연결 및 게임 세션 상태 관리 FSM
UENUM(BlueprintType)
enum class ESocketNetworkState : uint8
{
	// 기본 연결 상태들
	Disconnected		UMETA(DisplayName = "Disconnected"),
	Connecting			UMETA(DisplayName = "Connecting"),
	Connected			UMETA(DisplayName = "Connected"),
	ConnectionError		UMETA(DisplayName = "Connection Error"),

	// 게임 세션 확장 가능 상태들 (향후 추가 예정)
	Authenticating		UMETA(DisplayName = "Authenticating"),
	InLobby				UMETA(DisplayName = "In Lobby"),
	InGame				UMETA(DisplayName = "In Game"),
	GamePaused			UMETA(DisplayName = "Game Paused"),
	GameFinished		UMETA(DisplayName = "Game Finished"),

	// 오류 상태들
	AuthenticationFailed UMETA(DisplayName = "Authentication Failed"),
	GameSessionError	UMETA(DisplayName = "Game Session Error")
};

class FSocketReceiveWorker : public FRunnable
{
public:
	FSocketReceiveWorker(TSharedPtr<IWebSocket> InSocket, TQueue<FString>& InReceiveQueue, FThreadSafeBool& InRunThread);
	virtual ~FSocketReceiveWorker();

	// FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	void BindWebSocket();
	
	TSharedPtr<IWebSocket> webSocket;
	TQueue<FString>& receiveQueue;
	FThreadSafeBool& bRunThread;
	FString receivedJsonBuffer;

	// WebSocket 메시지 임시 저장용 (내부 관리)
	TQueue<FString> webSocketMessageQueue;
	FThreadSafeCounter pendingMessages;
};

UCLASS()
class UE_METAFACIAL_API USocketClientSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	USocketClientSubsystem();
	virtual TStatId GetStatId() const override;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	virtual void Tick(float deltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketNetwork")
	FString ServerIP = TEXT("172.16.20.152");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketNetwork")
	FString Protocol = TEXT("ws");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketNetwork")
	int32 ServerPort = 8000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketNetwork")
	FString Path = TEXT("chat");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketNetwork")
	FString ClientID = TEXT("aaa");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SocketNetwork")
	FReceivedNetData receivedData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SocketNetwork")
	ESocketNetworkState CurrentConnectionState = ESocketNetworkState::Disconnected;
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "SocketNetwork")
	void ConnectToServer();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "SocketNetwork")
	void DisconnectFromServer();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "SocketNetwork")
	bool SendMessage(const FString& Message);

	bool SendJsonMessage(const TSharedPtr<FJsonObject>& jsonObject);

	UFUNCTION(BlueprintCallable, Category = "SocketNetwork")
	bool SendJsonString(const FString& JsonString);

protected:
	void ProcessReceivedJson(const FString& jsonString);

	// JSON 처리 유연성 개선을 위한 헬퍼 함수들
	void ProcessSingleJsonObject(TSharedPtr<FJsonObject> jsonObject);
	void ProcessJsonArray(const TArray<TSharedPtr<FJsonValue>>& jsonArray);

	// URL 조립 함수
	FString BuildWebSocketURL() const;

	// 상태 관리 함수들
	UFUNCTION(BlueprintCallable, Category = "SocketNetwork")
	void SetConnectionState(ESocketNetworkState NewState);

	UFUNCTION(BlueprintPure, Category = "SocketNetwork")
	bool IsInState(ESocketNetworkState State) const;

	UFUNCTION(BlueprintPure, Category = "SocketNetwork")
	bool CanTransitionTo(ESocketNetworkState NewState) const;

	// WebSocket 이벤트 핸들러들
	UFUNCTION()
	void OnWebSocketConnected();

	UFUNCTION()
	void OnWebSocketError(const FString& ErrorMessage);

	UFUNCTION()
	void OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	
private:
	TSharedPtr<IWebSocket> webSocket;
	FRunnableThread* receiveThread;
	FThreadSafeBool bRunThread;
	TQueue<FString> jsonReceiveQueue;

	// 스레드 안전한 상태 관리를 위한 크리티컬 섹션
	mutable FCriticalSection ConnectionStateLock;

};
