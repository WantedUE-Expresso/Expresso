#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "DataStructures.h"
#include "DataParser.h"
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
	Authenticating		UMETA(DisplayName = "Authenticating"),	// 내 ID 주기
	GameStarted			UMETA(DisplayName = "In Lobby"),		// 상대 아이디 받기
	RoundStarted		UMETA(DisplayName = "In Game"),			// 제시어 받기
	Capturing			UMETA(DisplayName = "Capturing"),		// 이미지 주기
	RoundFinished		UMETA(DisplayName = "Game Paused"),		// 라운드 결과 받기
	GameFinished		UMETA(DisplayName = "Game Finished"),	// Disconnect

	// 오류 상태들
	AuthenticationFailed UMETA(DisplayName = "Authentication Failed"),
	GameSessionError	UMETA(DisplayName = "Game Session Error")
};

/*class FSocketReceiveWorker : public FRunnable
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
};*/

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
	FReceivedNetState receivedData;

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

	// DataParser를 이용한 구조체 전송 편의 함수
	UFUNCTION(BlueprintCallable, Category = "SocketNetwork")
	bool SendImageData(const FSendImageField& ImageData);

	// 테스트용 함수들
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "SocketNetwork")
	bool SendTestMessage();

protected:
	void ProcessReceivedJson(const FString& jsonString);

	// 상태 기반 JSON 파싱 디스패치 함수
	UFUNCTION(BlueprintCallable, Category = "SocketNetwork")
	void ProcessJsonByState(const FString& jsonString);

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

	UFUNCTION()
	void OnWebSocketMessage(const FString& Message);
	
private:
	TSharedPtr<IWebSocket> webSocket;
	FRunnableThread* receiveThread = nullptr;
	FThreadSafeBool bRunThread;
	TQueue<FString> jsonReceiveQueue;

	// 스레드 안전한 상태 관리를 위한 크리티컬 섹션
	mutable FCriticalSection ConnectionStateLock;

};
