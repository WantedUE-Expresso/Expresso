#include "Flow/MetaFacialGameInstance.h"

#include "Game/MetaGameState.h"
#include "Kismet/GameplayStatics.h"

void UMetaFacialGameInstance::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MetaFacialMap"));
}

void UMetaFacialGameInstance::FinishGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuMap"));
}

void UMetaFacialGameInstance::AddToWinCnt(const FString& InID)
{
	FRoundWinCnt cnt;
	cnt.PlayerID = InID;
	cnt.WinCnt = 0;
	WinCnt.AddUnique(cnt);
}
