#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MetaGameState.generated.h"

class UDataExchangeComponent;

UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
	RoundReady,
	RoundBriefing,
	CameraCapture,
	ServerWaiting,
	RoundResult,
	WhoWin
};

enum class EMatchPhase : uint8;

USTRUCT(BlueprintType)
struct FRoundWinCnt
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString PlayerID;
	
	UPROPERTY(BlueprintReadOnly)
	float WinCnt;

	bool operator==(const FRoundWinCnt& Other) const
	{
		return PlayerID == Other.PlayerID;
	}
};

UCLASS()
class UE_METAFACIAL_API AMetaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMetaGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPhase(EMatchPhase InPhase);

	UFUNCTION(BlueprintCallable)
	void SetRound(int32 InRound);
	
	UFUNCTION(BlueprintCallable)
	int32 GetRound();

	UFUNCTION(BlueprintCallable)
	void AddToWinCnt(const FString& InID);

	UFUNCTION(BlueprintCallable)
	void IncreaseWinCnt(FString ID);
	
	
	UFUNCTION()
	void OnRep_Phase();

	UFUNCTION()
	void OnRep_Round();

	UFUNCTION()
	void OnRep_WinCnt();

	UPROPERTY(ReplicatedUsing=OnRep_WinCnt, BlueprintReadWrite)
	TArray<FRoundWinCnt> WinCnt;

	UPROPERTY(BlueprintReadWrite)
	FRoundWinCnt PlayerInfo;

	UPROPERTY(BlueprintReadWrite)
	FRoundWinCnt OtherInfo;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDataExchangeComponent* GetDataExchangeComp() {return DataExChangeComponent;}


protected:
	UPROPERTY(ReplicatedUsing=OnRep_Phase, BlueprintReadWrite)
	EMatchPhase Phase = EMatchPhase::RoundReady;

	UPROPERTY(ReplicatedUsing=OnRep_Round, BlueprintReadOnly)                                     
	int32 RoundIndex = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataExchangeComponent* DataExChangeComponent = nullptr;
	

	
	
};
