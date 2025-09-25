# UE5 MetaHuman 페이셜 데이터 수신 시스템 요구사항

## 프로젝트 개요
- **목적**: 외부 팀에서 전송하는 실시간 페이셜 트래킹 데이터를 UE5 MetaHuman에 적용
- **역할**: 데이터 수신 및 MetaHuman 애니메이션 처리 (데이터 생성은 외부 팀 담당)

## 언리얼 엔진 버전
- **Unreal Engine 5.4+** (필수: 최신 MetaHuman 지원)
- **권장**: UE 5.6.1 이상 (웹캠 페이셜 애니메이션 지원, MetaHuman Plugin 별도 적용 불필요)

## Epic Games Launcher (팹) 필수 라이브러리

### 1. MetaHuman 관련
```
- MetaHuman Plugin (Epic Games 공식)
- MetaHuman Creator Integration
- MetaHuman Sample Content (테스트용)
```

### 2. Live Link 시스템
```
- Live Link Plugin (기본 포함)
- Live Link Animation Core
- Live Link Face (iOS ARKit 호환)
```

### 3. 애니메이션 시스템
```
- Animation Blueprint Utilities
- Control Rig Plugin
- Facial Animation Bulk Importer (선택적)
```

### 4. 개발 지원 도구
```
- Animation Tools Plugin
- Sequencer Plugin (기본 포함)
- Take Recorder Plugin (테스트/녹화용)
```

---

## 프로젝트 내 활성화 필요 플러그인

### 필수 플러그인
- [x] **Live Link** - 실시간 데이터 스트리밍
- [x] **MetaHuman** - 메타휴먼 캐릭터 지원
- [x] **Control Rig** - 고급 애니메이션 제어
- [x] **Animation Blueprint** - 애니메이션 로직
- [x] **Niagara** - 파티클 시스템 (선택적)

### 네트워크/통신 플러그인 (데이터 수신용)
- [x] **TCP Socket Plugin** - TCP 통신 (메인 수신 방식)
- [x] **WebSocket** - WebSocket 통신 (대안)
- [ ] **UDP Messaging** - UDP 통신 (고속 전송시)
- [ ] **HTTP** - REST API 통신 (설정용)

### 개발 지원 플러그인
- [x] **Developer Tools** - 디버깅 도구
- [x] **Stat Commands** - 성능 모니터링
- [x] **Console Variables** - 런타임 설정 변경

---

## C++ 모듈 의존성

### 필수 모듈 (데이터 수신 및 처리용)
```cpp
// Build.cs 파일에 추가 필요
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "LiveLink",
    "LiveLinkInterface",
    "LiveLinkAnimationCore",
    "ControlRig",
    "AnimationBlueprintLibrary",
    "Networking",
    "Sockets",
    "Json",
    "JsonObjectConverter"
});
```

### 선택적 모듈
```cpp
PrivateDependencyModuleNames.AddRange(new string[] {
    "UnrealEd",
    "ToolMenus",
    "Slate",
    "SlateCore",
    "EditorStyle",
    "HTTP",
    "Json",
    "JsonObjectConverter"
});
```

---

## UE5 수신 데이터 요구사항

### 외부팀으로부터 수신할 데이터 포맷
```json
{
  "timestamp": 1640995200000,
  "frame_id": 12345,
  "blendshapes": {
    "eyeBlinkLeft": 0.0,
    "eyeBlinkRight": 0.0,
    "jawOpen": 0.3,
    "mouthSmileLeft": 0.7
  },
  "head_rotation": {
    "x": 0.0, "y": 15.0, "z": -5.0
  }
}
```

### MetaHuman 블렌드셰이프 매핑 (수신 후 변환)
```cpp
// 외부 데이터 → MetaHuman 컨트롤 이름 매핑
static const TMap<FString, FString> BlendShapeMapping = {
    {TEXT("eyeBlinkLeft"), TEXT("CTRL_L_eye_blink")},
    {TEXT("eyeBlinkRight"), TEXT("CTRL_R_eye_blink")},
    {TEXT("jawOpen"), TEXT("CTRL_jawOpen")},
    {TEXT("mouthSmileLeft"), TEXT("CTRL_L_mouth_smile")},
    {TEXT("mouthSmileRight"), TEXT("CTRL_R_mouth_smile")}
};
```

### 지원 블렌드셰이프 목록 (52개)
```
얼굴 표현 (외부팀에서 전송):
- eyeBlinkLeft, eyeBlinkRight
- eyeSquintLeft, eyeSquintRight
- eyeWideLeft, eyeWideRight
- jawForward, jawLeft, jawRight, jawOpen
- mouthClose, mouthFunnel, mouthPucker
- mouthLeft, mouthRight, mouthSmileLeft, mouthSmileRight
- mouthFrownLeft, mouthFrownRight
- mouthDimpleLeft, mouthDimpleRight
- mouthStretchLeft, mouthStretchRight
- mouthRollLower, mouthRollUpper
- mouthShrugLower, mouthShrugUpper
- mouthPressLeft, mouthPressRight
- mouthLowerDownLeft, mouthLowerDownRight
- mouthUpperUpLeft, mouthUpperUpRight
- browDownLeft, browDownRight
- browInnerUp, browOuterUpLeft, browOuterUpRight
- cheekPuff, cheekSquintLeft, cheekSquintRight
- noseSneerLeft, noseSneerRight
- tongueOut
```

---

## 개발 환경 설정

### Visual Studio 설정
- **Visual Studio 2022** (권장)
- **C++ 게임 개발** 워크로드
- **Windows 10/11 SDK** 최신 버전

### 추가 도구
- **Git LFS** (대용량 파일 관리)
- **Perforce** (선택적, 팀 개발 시)

---

## 성능 요구사항

### 최소 사양
- **CPU**: Intel i5-8400 / AMD Ryzen 5 2600
- **GPU**: GTX 1060 6GB / RX 580 8GB
- **RAM**: 16GB
- **Storage**: SSD 권장

### 권장 사양
- **CPU**: Intel i7-10700K / AMD Ryzen 7 3700X
- **GPU**: RTX 3070 / RX 6700 XT 이상
- **RAM**: 32GB
- **Storage**: NVMe SSD

---

## 테스트 환경

### 테스트용 메타휴먼
- **샘플 메타휴먼 캐릭터** (남성/여성 각 1개)
- **기본 애니메이션 세트**
- **테스트 레벨 씬**

### UE5 구현 요구사항

#### 핵심 C++ 클래스
```cpp
1. UFacialDataReceiver - 네트워크 데이터 수신
2. FFacialDataLiveLinkSource - Live Link 데이터 전송
3. AMetaHumanWithFacialData - MetaHuman 액터
4. FFacialBlendShapes - 데이터 구조체
```

#### 네트워크 설정
```
- 수신 포트: 8888 (TCP) / 8889 (WebSocket)
- 데이터 빈도: 30-60 FPS
- 프로토콜: TCP Socket 또는 WebSocket
- 데이터 포맷: JSON
```

### 디버깅 도구
- **Live Link Debug Panel** - 수신 데이터 모니터링
- **Animation Blueprint Debugger** - 블렌드셰이프 확인
- **Network Connection Monitor** - 연결 상태 추적
- **Performance Profiler** - FPS 및 지연시간 측정
- **Custom Debug UI** - 실시간 데이터 표시