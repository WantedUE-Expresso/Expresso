// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaHumanSceneCapture.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


AMetaHumanSceneCapture::AMetaHumanSceneCapture()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> metaHumanRenderTargetRef(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/ScreenShot/MetaHumanScreenShot/RT_MetaHuman.RT_MetaHuman'"));
	if (metaHumanRenderTargetRef.Succeeded())
	{
		MetaHumanRenderTarget2D = metaHumanRenderTargetRef.Object;
	}
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->TextureTarget = MetaHumanRenderTarget2D;
	SceneCaptureComponent->CaptureSource = SCS_FinalColorLDR;
}

void AMetaHumanSceneCapture::BeginPlay()
{
	Super::BeginPlay();
	
}

