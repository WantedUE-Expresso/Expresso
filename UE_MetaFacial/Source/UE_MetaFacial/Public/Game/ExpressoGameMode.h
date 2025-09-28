
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ExpressoGameMode.generated.h"


class ULiveLinkPreset;
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

	// UFUNCTION()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULiveLinkPreset* LiveLinkPreset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULiveLinkPreset* UnlinkPreset;
};
