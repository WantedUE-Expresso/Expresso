#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MetaGameState.generated.h"

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
struct FRoundScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString PlayerID;
	
	UPROPERTY(BlueprintReadOnly)
	float Score;
};

UCLASS()
class UE_METAFACIAL_API AMetaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPhase(EMatchPhase InPhase);
	
	UFUNCTION()
	void OnRep_Phase();

	UFUNCTION()
	void OnRep_Round();

	UFUNCTION()
	void OnRep_Scores();

	

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Phase, BlueprintReadWrite)
	EMatchPhase Phase = EMatchPhase::RoundReady;

	UPROPERTY(ReplicatedUsing=OnRep_Round, BlueprintReadOnly)                                     
	int32 RoundIndex = 0;

	UPROPERTY(ReplicatedUsing=OnRep_Scores, BlueprintReadOnly)
	TArray<FRoundScore> Scores;
};
