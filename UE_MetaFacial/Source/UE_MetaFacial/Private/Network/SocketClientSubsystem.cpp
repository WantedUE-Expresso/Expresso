#include "Network/SocketClientSubsystem.h"

#include "IWebSocket.h"
#include "SocketSubsystem.h"
#include "Async/Async.h"
#include "Flow/MetaFacialGameInstance.h"
#include "Game/DataExchangeComponent.h"
#include "Game/MetaGameState.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(NetSocketLog)

/*FSocketReceiveWorker::FSocketReceiveWorker(TSharedPtr<IWebSocket> InSocket, TQueue<FString>& InReceiveQueue, FThreadSafeBool& InRunThread)
		:webSocket(InSocket), receiveQueue(InReceiveQueue), bRunThread(InRunThread)
{
	BindWebSocket();
}

FSocketReceiveWorker::~FSocketReceiveWorker()
{
	bRunThread = false;
}

bool FSocketReceiveWorker::Init()
{
	receivedJsonBuffer.Empty();
	return true;
}

uint32 FSocketReceiveWorker::Run()
{
	while (bRunThread)
	{
		FString message;
		while (webSocketMessageQueue.Dequeue(message))
		{
			receiveQueue.Enqueue(message);
			pendingMessages.Decrement();
		}

		FPlatformProcess::Sleep(0.01f);
	}
	return 0;
}

void FSocketReceiveWorker::Stop()
{
	bRunThread = false;
}

void FSocketReceiveWorker::Exit()
{
	FRunnable::Exit();
}

void FSocketReceiveWorker::BindWebSocket()
{
	if (!webSocket.IsValid())
	{
		return;
	}

	// 안전한 람다 캡처: 필요한 변수들만 명시적으로 캡처
	webSocket->OnMessage().AddLambda([this](const FString& Message) -> void
	{
		// 객체가 여전히 유효한지 확인
		if (this && bRunThread)
		{
			webSocketMessageQueue.Enqueue(Message);
			pendingMessages.Increment();
		}
	});
}*/


USocketClientSubsystem::USocketClientSubsystem()
	:webSocket(nullptr), receiveThread(nullptr), bRunThread(false)
{
	CurrentConnectionState = ESocketNetworkState::Disconnected;
}

TStatId USocketClientSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UWebSubSystem, STATGROUP_Tickables);
}

void USocketClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USocketClientSubsystem::Deinitialize()
{
	DisconnectFromServer();
	Super::Deinitialize();
}

void USocketClientSubsystem::Tick(float deltaTime)
{
	if (jsonReceiveQueue.IsEmpty())
	{
		return;
	}
	FString jsonMessage;
	while (jsonReceiveQueue.Dequeue(jsonMessage))
	{
		UE_LOG(NetSocketLog, Log, TEXT("Processing JSON: %s"), *jsonMessage);
		ProcessReceivedJson(jsonMessage);
	}
}

FString USocketClientSubsystem::BuildWebSocketURL() const
{
	// Protocol://IP:Port/Path/{ClientID} 형태로 조립
	return FString::Printf(TEXT("%s://%s:%d/%s/%s"),
		*Protocol,
		*ServerIP,
		ServerPort,
		*Path,
		*ClientID
	);
}

void USocketClientSubsystem::SetConnectionState(ESocketNetworkState NewState)
{
	FScopeLock Lock(&ConnectionStateLock);

	if (CurrentConnectionState != NewState)
	{
		ESocketNetworkState PreviousState = CurrentConnectionState;
		CurrentConnectionState = NewState;

		UE_LOG(NetSocketLog, Log, TEXT("Connection state changed: %d -> %d"),
			(int32)PreviousState, (int32)NewState);
	}
}

bool USocketClientSubsystem::IsInState(ESocketNetworkState State) const
{
	FScopeLock Lock(&ConnectionStateLock);
	return CurrentConnectionState == State;
}

bool USocketClientSubsystem::CanTransitionTo(ESocketNetworkState NewState) const
{
	FScopeLock Lock(&ConnectionStateLock);

	// 기본적인 상태 전환 규칙들
	switch (CurrentConnectionState)
	{
		case ESocketNetworkState::Disconnected:
			return (NewState == ESocketNetworkState::Connecting);

		case ESocketNetworkState::Connecting:
			return (NewState == ESocketNetworkState::Connected ||
					NewState == ESocketNetworkState::ConnectionError ||
					NewState == ESocketNetworkState::Disconnected);

		case ESocketNetworkState::Connected:
			return (NewState == ESocketNetworkState::Disconnected ||
					NewState == ESocketNetworkState::Authenticating ||
					NewState == ESocketNetworkState::GameStarted ||
					NewState == ESocketNetworkState::ConnectionError);

		case ESocketNetworkState::ConnectionError:
			return (NewState == ESocketNetworkState::Disconnected ||
					NewState == ESocketNetworkState::Connecting);

		// 게임 세션 상태들 (향후 확장 가능)
		case ESocketNetworkState::Authenticating:
		case ESocketNetworkState::GameStarted:
		case ESocketNetworkState::RoundStarted:
		case ESocketNetworkState::RoundFinished:
		case ESocketNetworkState::GameFinished:
		case ESocketNetworkState::AuthenticationFailed:
		case ESocketNetworkState::GameSessionError:
		default:
			return true; // 게임 세션 상태들은 향후 로직에 따라 확장
	}
}

void USocketClientSubsystem::ConnectToServer()
{
	// 상태 확인 및 전환 가능 여부 검사
	if (!CanTransitionTo(ESocketNetworkState::Connecting))
	{
		UE_LOG(NetSocketLog, Warning, TEXT("Cannot connect from current state: %d"), (int32)CurrentConnectionState);
		return;
	}

	SetConnectionState(ESocketNetworkState::Connecting);

	//FString ServerURL = FString::Printf(TEXT("%s://%s:%d"), *Protocol, *ServerIP, ServerPort);
	FString ServerURL = BuildWebSocketURL();
	FString SubProtocol = TEXT(""); // 빈 문자열 또는 필요시 웹소켓 서브프로토콜

	UE_LOG(NetSocketLog, Warning, TEXT("Connecting to WebSocket - Server: %s, Path: /%s/%s"), *ServerURL, *Path, *ClientID);

	// WebSocket 생성
	webSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, SubProtocol);

	if (!webSocket.IsValid())
	{
		UE_LOG(NetSocketLog, Error, TEXT("Failed to create WebSocket with URL: %s"), *ServerURL);
		SetConnectionState(ESocketNetworkState::ConnectionError);
		return;
	}

	// WebSocket 이벤트 바인딩
	webSocket->OnConnected().AddUObject(this, &USocketClientSubsystem::OnWebSocketConnected);
	webSocket->OnConnectionError().AddUObject(this, &USocketClientSubsystem::OnWebSocketError);
	webSocket->OnClosed().AddUObject(this, &USocketClientSubsystem::OnWebSocketClosed);
	webSocket->OnMessage().AddUObject(this, &USocketClientSubsystem::OnWebSocketMessage);

	// 연결 시도
	webSocket->Connect();
}

void USocketClientSubsystem::OnWebSocketConnected()
{
	UE_LOG(NetSocketLog, Warning, TEXT("WebSocket connected successfully to: %s"), *BuildWebSocketURL());

	SetConnectionState(ESocketNetworkState::Connected);
	
	// Worker 스레드 시작
	// bRunThread = true;
	//receiveThread = FRunnableThread::Create(
	//	new FSocketReceiveWorker(webSocket, jsonReceiveQueue, bRunThread),
	//	TEXT("WebSocketReceiveWorker"),
	//	true,   // bAutoDeleteSelf = true (Thread와 Worker 모두 자동 삭제)
	//	TPri_Normal
	//);
}

void USocketClientSubsystem::OnWebSocketError(const FString& ErrorMessage)
{
	UE_LOG(NetSocketLog, Error, TEXT("WebSocket connection error: %s"), *ErrorMessage);

	SetConnectionState(ESocketNetworkState::ConnectionError);
}

void USocketClientSubsystem::OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(NetSocketLog, Warning, TEXT("WebSocket connection closed. Code: %d, Reason: %s, Clean: %s"),
		StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));

	SetConnectionState(ESocketNetworkState::Disconnected);
}

void USocketClientSubsystem::OnWebSocketMessage(const FString& Message)
{
	UE_LOG(NetSocketLog, Warning, TEXT("Received WebSocket Message: %s"), *Message);
	jsonReceiveQueue.Enqueue(Message);
	UE_LOG(NetSocketLog, Warning, TEXT("Message enqueued successfully"));
}

void USocketClientSubsystem::DisconnectFromServer()
{
	// 이미 연결이 끊어진 상태라면 종료
	if (IsInState(ESocketNetworkState::Disconnected))
	{
		UE_LOG(NetSocketLog, Warning, TEXT("Already disconnected."));
		return;
	}

	// Runnable 스레드 정리 (UE 권장 패턴)
	if (receiveThread)
	{
		bRunThread = false;
		receiveThread->WaitForCompletion();
		// bAutoDeleteSelf = true로 생성했으므로 Thread와 Worker가 자동 삭제됨
		receiveThread = nullptr;
	}

	// WebSocket 연결 정리
	if (webSocket.IsValid())
	{
		webSocket->Close();
		webSocket.Reset();
		UE_LOG(NetSocketLog, Warning, TEXT("Disconnected from WebSocket server."));
	}

	SetConnectionState(ESocketNetworkState::Disconnected);
}

void USocketClientSubsystem::ProcessReceivedJson(const FString& jsonString)
{
	UE_LOG(NetSocketLog, Log, TEXT("%s"), *jsonString);
	if (!IsInGameThread())
    {
		UE_LOG(NetSocketLog, Error, TEXT("Async Task"));
        AsyncTask(ENamedThreads::GameThread, [this, jsonString]()
        {
            ProcessReceivedJson(jsonString);
        });
        return;
    }

    if (jsonString.IsEmpty())
    {
        UE_LOG(NetSocketLog, Warning, TEXT("Received empty JSON string"));
        return;
    }

    UE_LOG(NetSocketLog, Log, TEXT("Processing JSON: %s"), *jsonString);

    // 상태 기반 JSON 파싱으로 위임
    ProcessJsonByState(jsonString);
}

void USocketClientSubsystem::ProcessJsonByState(const FString& jsonString)
{
    // JSON 유효성 먼저 검사
	UE_LOG(NetSocketLog, Warning, TEXT("Receive JSON: %s"), *jsonString);
    if (!UDataParser::IsValidJsonString(jsonString))
    {
        UE_LOG(NetSocketLog, Error, TEXT("Invalid JSON string received: %s"), *jsonString);
        return;
    }

	UE_LOG(NetSocketLog, Warning, TEXT("Start Parse - Valid JSON"));
    UE_LOG(NetSocketLog, Log, TEXT("Processing JSON by current state: %d"), (int32)CurrentConnectionState);

    switch (CurrentConnectionState)
    {
        case ESocketNetworkState::Connected:
        {
            if (UDataParser::IsValidJsonObject(jsonString))
            {
                if (UDataParser::DeserializeReceivedNetState(jsonString, receivedData))
                {
	                SetConnectionState(ESocketNetworkState::Authenticating);
                }
            }
            break;
        }
        case ESocketNetworkState::Authenticating:
        {
            if (UDataParser::IsValidJsonObject(jsonString))
            {
                UDataParser::DeserializeReceivedNetState(jsonString, receivedData);
            	if (receivedData.code == "202")
            	{
            		Cast<UMetaFacialGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ReadyToStart = true;
            		SetConnectionState(ESocketNetworkState::RoundStarted);
            	}
            }
            break;
        }

        case ESocketNetworkState::GameStarted:
	        {
		        
	        }
    		break;
        case ESocketNetworkState::RoundStarted:
        {
            // 게임 데이터 처리 (배열 또는 단일 객체)
            if (UDataParser::IsValidJsonArray(jsonString))
            {
                // 작성 요망: FResultImageData 배열 파싱
                // TArray<FResultImageData> resultArray;
                // UDataParser::DeserializeResultImageDataArray(jsonString, resultArray);
                UE_LOG(NetSocketLog, Warning, TEXT("TODO: Parse FResultImageData Array"));
            }
            else if (UDataParser::IsValidJsonObject(jsonString))
            {
                // 작성 요망: FNetPayload 파싱
                // FNetPayload payloadData;
                // UDataParser::DeserializeNetPayload(jsonString, payloadData);
                UE_LOG(NetSocketLog, Warning, TEXT("TODO: Parse FNetPayload"));
            }
            break;
        }
		case ESocketNetworkState::Capturing:
        {
        	GetWorld()->GetGameState<AMetaGameState>()->GetDataExchangeComp()->SendRoundData();
			break;	        
        }
		case ESocketNetworkState::RoundFinished:
        {
        	if (UDataParser::IsValidJsonObject(jsonString))
        	{
        		if (UDataParser::DeserializeReceivedNetState(jsonString, receivedData))
        		{
        			TArray<FRoundResultData> roundResults;
        			roundResults.AddDefaulted(2);
        			//roundResults[0] = receivedData.info;
        			//GetWorld()->GetGameState<AMetaGameState>()->GetDataExchangeComp()->ReceiveRoundResultData(receivedData);
        		}
        	}
			break;   
        }
        case ESocketNetworkState::GameFinished:
        {
        		if (UMetaFacialGameInstance* gi = Cast<UMetaFacialGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
        		{
        			gi->ReadyToStart = false;
        		}
        		//gi->FinishGame();
        		
            break;
        }

        case ESocketNetworkState::ConnectionError:
        case ESocketNetworkState::AuthenticationFailed:
        case ESocketNetworkState::GameSessionError:
        {
            // 에러 상태에서는 에러 메시지만 로깅
            UE_LOG(NetSocketLog, Warning, TEXT("Received JSON in error state (%d): %s"),
                (int32)CurrentConnectionState, *jsonString);
            break;
        }

        default:
        {
            // 연결되지 않은 상태에서 JSON 수신 (이상 상황)
            UE_LOG(NetSocketLog, Warning, TEXT("Received JSON in unexpected state (%d): %s"),
                (int32)CurrentConnectionState, *jsonString);
            break;
        }
    }
}

bool USocketClientSubsystem::SendMessage(const FString& Message)
{
    UE_LOG(NetSocketLog, Warning, TEXT("SendMessage called with: %s"), *Message);

    if (!webSocket.IsValid())
    {
        UE_LOG(NetSocketLog, Error, TEXT("Cannot send message: WebSocket is invalid"));
        return false;
    }

    /*if (!IsInState(ESocketNetworkState::Connected))
    {
        UE_LOG(NetSocketLog, Error, TEXT("Cannot send message: Not connected (State: %d)"), (int32)CurrentConnectionState);
        return false;
    }*/

    if (Message.IsEmpty())
    {
        UE_LOG(NetSocketLog, Warning, TEXT("Attempting to send empty message"));
        return false;
    }

    UE_LOG(NetSocketLog, Warning, TEXT("Sending message via WebSocket: %s"), *Message);
    webSocket->Send(Message);
    UE_LOG(NetSocketLog, Warning, TEXT("Message sent successfully: %s"), *Message);
    return true;
}

bool USocketClientSubsystem::SendJsonMessage(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        UE_LOG(NetSocketLog, Error, TEXT("Cannot send JSON: JsonObject is invalid"));
        return false;
    }

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);

    if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        return SendMessage(JsonString);
    }
    else
    {
        UE_LOG(NetSocketLog, Error, TEXT("Failed to serialize JSON object"));
        return false;
    }
}

bool USocketClientSubsystem::SendJsonString(const FString& JsonString)
{
    UE_LOG(NetSocketLog, Warning, TEXT("SendJsonString called with: %s"), *JsonString);

    if (JsonString.IsEmpty())
    {
        UE_LOG(NetSocketLog, Error, TEXT("Cannot send empty JSON string"));
        return false;
    }

    // DataParser를 이용한 JSON 유효성 검사
    bool bIsValid = UDataParser::IsValidJsonString(JsonString);
    UE_LOG(NetSocketLog, Warning, TEXT("JSON validation result: %s for string: %s"),
        bIsValid ? TEXT("Valid") : TEXT("Invalid"), *JsonString);

    if (!bIsValid)
    {
        UE_LOG(NetSocketLog, Warning, TEXT("Sending potentially invalid JSON string: %s"), *JsonString);
    }

    return SendMessage(JsonString);
}

bool USocketClientSubsystem::SendImageData(const FSendImageField& ImageData)
{
    FString jsonString = UDataParser::SerializeSendImageField(ImageData);
    if (jsonString.IsEmpty())
    {
        UE_LOG(NetSocketLog, Error, TEXT("Failed to serialize FSendImageField"));
        return false;
    }

    return SendMessage(jsonString);
}

bool USocketClientSubsystem::SendTestMessage()
{
    UE_LOG(NetSocketLog, Warning, TEXT("SendTestMessage called"));

    // 간단한 테스트 JSON 메시지
    FString testJson = TEXT("{\"type\":\"test\",\"message\":\"Hello from UE Client\",\"timestamp\":\"2024-09-29\"}");

    UE_LOG(NetSocketLog, Warning, TEXT("Sending test JSON: %s"), *testJson);
    bool result = SendJsonString(testJson);

    UE_LOG(NetSocketLog, Warning, TEXT("SendTestMessage result: %s"), result ? TEXT("Success") : TEXT("Failed"));
    return result;
}
