
#include "Game/ExpressoGameMode.h"

#include "LiveLinkCompression.h"
#include "MetaHumanSceneCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Game/MetaGameState.h"
#include "Game/MetaPlayerController.h"
#include "Kismet/KismetRenderingLibrary.h"

AExpressoGameMode::AExpressoGameMode()
{
	GameStateClass = AMetaGameState::StaticClass();
	PlayerControllerClass = AMetaPlayerController::StaticClass();
}

void AExpressoGameMode::SwitchPhase(EMatchPhase Phase)
{
	AMetaGameState* GS = GetGameState<AMetaGameState>();
	if (!GS) return;

	GS->SetPhase(Phase);
}

void AExpressoGameMode::CaptureMetaHuman()
{
	SceneCapture->SceneCaptureComponent->CaptureScene();
	
	FString path = FPaths::ProjectSavedDir() / TEXT("PictureMetaHuman/");
	FString fileName = TEXT("MetaHuman.png");

	IFileManager::Get().MakeDirectory(*path, true);
	UKismetRenderingLibrary::ExportRenderTarget(this, SceneCapture->MetaHumanRenderTarget2D, path, fileName);
}

void AExpressoGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector transform(-640.000000,-180.000000,150.000000);
	FRotator rotator(0.000000,-89.999999,0.000000);
	SceneCapture = GetWorld()->SpawnActor<AMetaHumanSceneCapture>(AMetaHumanSceneCapture::StaticClass(), transform, rotator);
}

bool AExpressoGameMode::HasReceivedResult()
{
	return bReceiveResult;
}

