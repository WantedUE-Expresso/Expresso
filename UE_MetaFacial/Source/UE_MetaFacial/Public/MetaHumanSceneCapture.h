// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetaHumanSceneCapture.generated.h"

UCLASS()
class UE_METAFACIAL_API AMetaHumanSceneCapture : public AActor
{
	GENERATED_BODY()

public:
	AMetaHumanSceneCapture();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY()
	TObjectPtr<class UTextureRenderTarget2D> MetaHumanRenderTarget2D;
};
