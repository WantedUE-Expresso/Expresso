#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MetaFacialGameInstance.generated.h"


UCLASS()
class UE_METAFACIAL_API UMetaFacialGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void FinishGame();

	UPROPERTY(EditAnywhere)
	bool ReadyToStart = false;
};
