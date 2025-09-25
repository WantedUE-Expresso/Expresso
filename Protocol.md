# UE5 MetaHuman 페이셜 데이터 수신 프로토콜

## 개요
외부 팀에서 전송하는 실시간 페이셜 트래킹 데이터를 UE5 MetaHuman에 적용하는 수신 시스템 구현 명세

---

## 1. UE5 수신 시스템 아키텍처

### 1.1 데이터 플로우
```
[외부 데이터팀] → [TCP/WebSocket] → [UE5 수신부] → [Live Link] → [MetaHuman]
```

### 1.2 UE5 내부 구조
```
NetworkReceiver → LiveLinkSource → AnimationBlueprint → MetaHuman
```

---

## 2. 수신 데이터 포맷

### 2.1 JSON 데이터 구조 (외부팀에서 전송)
```json
{
  "timestamp": 1640995200000,
  "frame_id": 12345,
  "blendshapes": {
    "eyeBlinkLeft": 0.0,
    "eyeBlinkRight": 0.0,
    "jawOpen": 0.3,
    "mouthSmileLeft": 0.7,
    "mouthSmileRight": 0.7
  },
  "head_rotation": {
    "x": 0.0,
    "y": 15.0,
    "z": -5.0
  }
}
```

### 2.2 UE5 수신 요구사항
```cpp
// 데이터 수신 포트
Port: 8888 (TCP) 또는 8889 (WebSocket)

// 수신 빈도
Target FPS: 30-60 Hz

// 데이터 검증
- timestamp 필수
- blendshapes 값 범위: 0.0 ~ 1.0
- head_rotation 범위: -180.0 ~ 180.0
```

---

## 3. UE5 구현 컴포넌트

### 3.1 필수 C++ 클래스
```cpp
// 1. 네트워크 수신 컴포넌트
class EXPRESSO_API UFacialDataReceiver : public UActorComponent
{
    UFUNCTION(BlueprintCallable)
    void StartListening(int32 Port = 8888);

    void ProcessIncomingData(const FString& JsonData);
};

// 2. Live Link Source 구현
class EXPRESSO_API FFacialDataLiveLinkSource : public ILiveLinkSource
{
    void PushBlendShapeData(const FFacialBlendShapes& Data);
    void Update() override;
};

// 3. 블렌드셰이프 데이터 구조체
USTRUCT(BlueprintType)
struct FFacialBlendShapes
{
    UPROPERTY(BlueprintReadWrite)
    float EyeBlinkLeft;

    UPROPERTY(BlueprintReadWrite)
    float EyeBlinkRight;

    // ... 모든 블렌드셰이프 필드
};
```

### 3.2 GameMode 통합
```cpp
// AExpressoGameModeBase.h
UCLASS()
class EXPRESSO_API AExpressoGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UFacialDataReceiver* FacialReceiver;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ListenPort = 8888;

public:
    virtual void BeginPlay() override;
};
```

---

## 4. Live Link 설정

### 4.1 Live Link Subject 구성
```cpp
// Live Link Subject 이름
static const FName FACIAL_SUBJECT_NAME = TEXT("ExternalFacialData");

// Animation Role
FLiveLinkSubjectKey SubjectKey;
SubjectKey.SubjectName = FACIAL_SUBJECT_NAME;
SubjectKey.Source = LiveLinkSourceGuid;

// 프레임 데이터 전송
FLiveLinkFrameDataStruct FrameDataStruct;
FLiveLinkAnimationFrameData* AnimationFrameData = FrameDataStruct.Cast<FLiveLinkAnimationFrameData>();
```

### 4.2 블렌드셰이프 매핑
```cpp
// MetaHuman 호환 블렌드셰이프 이름
static const TMap<FString, FString> BlendShapeMapping = {
    {TEXT("eyeBlinkLeft"), TEXT("CTRL_L_eye_blink")},
    {TEXT("eyeBlinkRight"), TEXT("CTRL_R_eye_blink")},
    {TEXT("jawOpen"), TEXT("CTRL_jawOpen")},
    {TEXT("mouthSmileLeft"), TEXT("CTRL_L_mouth_smile")},
    {TEXT("mouthSmileRight"), TEXT("CTRL_R_mouth_smile")}
};
```

---

## 5. Animation Blueprint 통합

### 5.1 Live Link Pose 노드 설정
```
Live Link Pose 노드:
- Subject Name: "ExternalFacialData"
- Role: Animation
- Retarget Asset: MetaHuman Face Retarget Asset
```

### 5.2 블렌드셰이프 적용
```cpp
// Animation Blueprint에서 사용할 함수
UFUNCTION(BlueprintPure, Category = "Facial Animation")
float GetBlendShapeValue(const FName& BlendShapeName);

// 실시간 블렌드셰이프 업데이트
void UpdateFacialBlendShapes(const FFacialBlendShapes& NewData);
```

---

## 6. 네트워크 수신 구현

### 6.1 TCP 소켓 수신
```cpp
class EXPRESSO_API FTCPFacialReceiver
{
    class FSocket* ListenerSocket;
    class FSocket* ConnectionSocket;

    void StartTCPListener(int32 Port);
    void HandleIncomingConnection();
    void ProcessReceivedData(const uint8* Data, int32 Size);
};
```

### 6.1 WebSocket 수신 (대안)
```cpp
class EXPRESSO_API FWebSocketFacialReceiver
{
    TSharedPtr<IWebSocket> WebSocket;

    void ConnectWebSocket(const FString& URL);
    void OnMessage(const FString& Message);
    void OnConnected();
    void OnConnectionError(const FString& Error);
};
```

---

## 7. MetaHuman 캐릭터 설정

### 7.1 MetaHuman Blueprint 수정
```cpp
// BP_MetaHuman_Base 상속 클래스
UCLASS()
class EXPRESSO_API AMetaHumanWithFacialData : public APawn
{
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USkeletalMeshComponent* FaceMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UFacialDataReceiver* FacialReceiver;

    UFUNCTION(BlueprintImplementableEvent)
    void OnFacialDataReceived(const FFacialBlendShapes& Data);
};
```

### 7.2 Animation Blueprint 연결
```
Class Settings:
- Parent Class: UAnimInstance
- Target Skeleton: MetaHuman Face Skeleton

Live Link 설정:
- Live Link Evaluate → Live Link Pose
- Subject Name: ExternalFacialData
```

---

## 8. 성능 최적화

### 8.1 스레딩
```cpp
// 백그라운드 스레드에서 네트워크 수신
class FFacialDataReceiverRunnable : public FRunnable
{
    virtual uint32 Run() override;
    void ProcessNetworkData();

    // 메인 스레드로 데이터 전달
    DECLARE_DELEGATE_OneParam(FOnFacialDataReceived, const FFacialBlendShapes&);
};
```

### 8.2 데이터 버퍼링
```cpp
// 지터 방지를 위한 버퍼
class FFacialDataBuffer
{
    TArray<FFacialBlendShapes> DataBuffer;
    float BufferTimeSeconds = 0.1f; // 100ms 버퍼

    FFacialBlendShapes GetSmoothedData();
    void AddNewData(const FFacialBlendShapes& NewData);
};
```

---

## 9. 디버깅 및 모니터링

### 9.1 디버그 정보 표시
```cpp
UFUNCTION(BlueprintCallable, Category = "Debug")
void ShowFacialDataDebugInfo(bool bShow = true);

// 화면에 실시간 데이터 표시
void DrawDebugFacialData(class UCanvas* Canvas);
```

### 9.2 연결 상태 모니터링
```cpp
UENUM(BlueprintType)
enum class EConnectionStatus : uint8
{
    Disconnected,
    Connecting,
    Connected,
    Error
};

UFUNCTION(BlueprintPure, Category = "Network")
EConnectionStatus GetConnectionStatus() const;
```

---

## 10. 블루프린트 인터페이스

### 10.1 블루프린트 이벤트
```cpp
// 블루프린트에서 바인딩 가능한 이벤트들
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFacialDataUpdate, const FFacialBlendShapes&, FacialData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionEstablished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionLost, const FString&, Reason);

UPROPERTY(BlueprintAssignable, Category = "Facial Animation")
FOnFacialDataUpdate OnFacialDataUpdate;

UPROPERTY(BlueprintAssignable, Category = "Network")
FOnConnectionEstablished OnConnectionEstablished;

UPROPERTY(BlueprintAssignable, Category = "Network")
FOnConnectionLost OnConnectionLost;
```

---

## 11. 프로젝트 설정 체크리스트

### 11.1 Build.cs 모듈 추가
```cpp
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "LiveLink",
    "LiveLinkInterface",
    "LiveLinkAnimationCore",
    "Networking",
    "Sockets",
    "Json",
    "JsonObjectConverter"
});
```

### 11.2 플러그인 활성화 확인
- [x] Live Link
- [x] MetaHuman
- [x] Animation Blueprint
- [x] Control Rig

---

## 12. 테스트 및 검증

### 12.1 단위 테스트
```cpp
// 데이터 수신 테스트
UTEST_TRUE("TCP Connection", FacialReceiver->IsConnected());
UTEST_EQUAL("Data Parsing", ParsedData.EyeBlinkLeft, 0.5f);

// Live Link 전송 테스트
UTEST_TRUE("LiveLink Active", ILiveLinkClient::Get().IsSubjectEnabled(FACIAL_SUBJECT_NAME));
```

### 12.2 성능 측정
```cpp
// Stat 그룹 정의
DECLARE_STATS_GROUP(TEXT("FacialData"), STATGROUP_FacialData, STATCAT_Advanced);
DECLARE_DWORD_COUNTER_STAT(TEXT("Packets Received"), STAT_PacketsReceived, STATGROUP_FacialData);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Processing Time MS"), STAT_ProcessingTime, STATGROUP_FacialData);
```

---

## 13. 배포 및 패키징

### 13.1 패키징 설정
```
Project Settings > Packaging:
- Include Prerequisites: True
- Full Rebuild: True
- Include Debug Info: False (릴리즈 빌드)

추가 Non-Asset Directories:
- Config/
```

### 13.2 런타임 설정
```ini
; DefaultEngine.ini
[/Script/Engine.NetworkSettings]
n.VerifyPeer=False

[LiveLink]
DefaultFrameRate=30
```