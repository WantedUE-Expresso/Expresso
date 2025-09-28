
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ExpressoGameMode.generated.h"


class ULiveLinkPreset;
class AMetaHumanSceneCapture;
enum class EMatchPhase : uint8;

UCLASS()
class UE_METAFACIAL_API AExpressoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AExpressoGameMode();

	UFUNCTION(BlueprintCallable)
	void SwitchPhase(EMatchPhase Phase);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateCamera();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DeactivateCamera();

	UFUNCTION(BlueprintCallable)
	void CaptureMetaHuman();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AMetaHumanSceneCapture> SceneCapture;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULiveLinkPreset* LiveLinkPreset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULiveLinkPreset* UnlinkPreset;
};
