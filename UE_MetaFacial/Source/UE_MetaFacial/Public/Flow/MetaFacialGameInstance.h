#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MetaFacialGameInstance.generated.h"


struct FRoundWinCnt;

UCLASS()
class UE_METAFACIAL_API UMetaFacialGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void FinishGame();
	
	UFUNCTION(BlueprintCallable)
	void AddToWinCnt(const FString& InID);

	UPROPERTY(BlueprintReadOnly)
	TArray<FRoundWinCnt> WinCnt;
	

	UPROPERTY(EditAnywhere)
	bool ReadyToStart = false;
};
