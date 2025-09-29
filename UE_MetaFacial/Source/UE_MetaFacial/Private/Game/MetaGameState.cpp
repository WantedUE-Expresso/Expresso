
#include "Game/MetaGameState.h"

#include "Game/DataExchangeComponent.h"
#include "Game/MetaPlayerController.h"
#include "Net/UnrealNetwork.h"


AMetaGameState::AMetaGameState()
{
	DataExChangeComponent = CreateDefaultSubobject<UDataExchangeComponent>(TEXT("DataExchange"));
}

void AMetaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMetaGameState, Phase);
}

void AMetaGameState::SetPhase(EMatchPhase InPhase)
{
	Phase = InPhase;
	
	ForceNetUpdate();

	if (!GetWorld()->IsNetMode(NM_Client))
	{
		OnRep_Phase();
	}
}

void AMetaGameState::SetRound(int32 InRound)
{
	RoundIndex = InRound;
}

int32 AMetaGameState::GetRound()
{
	return RoundIndex;
}

void AMetaGameState::AddToWinCnt(const FString& InID)
{
	/*
	FRoundWinCnt cnt;
	cnt.PlayerID = InID;
	cnt.WinCnt = 0;
	WinCnt.AddUnique(cnt);
	*/
}

void AMetaGameState::IncreaseWinCnt(FString ID)
{
	
}

void AMetaGameState::OnRep_Phase()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (auto* MyPC = Cast<AMetaPlayerController>(PC))
	{
		MyPC->ShowScreen(Phase);
	}
}

void AMetaGameState::OnRep_Round()
{
}

void AMetaGameState::OnRep_WinCnt()
{
}
