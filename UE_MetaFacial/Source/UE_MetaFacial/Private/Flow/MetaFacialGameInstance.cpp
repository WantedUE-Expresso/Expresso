#include "Flow/MetaFacialGameInstance.h"

#include "Game/MetaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Network/SocketClientSubsystem.h"

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

	PlayerInfo.PlayerID = InID;
	UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USocketClientSubsystem>()->ClientID = InID;
	PlayerInfo.WinCnt = 0;
	
	/*
	FRoundWinCnt cnt;
	UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USocketClientSubsystem>()->ClientID = InID;
	cnt.PlayerID = InID;
	cnt.WinCnt = 0;
	WinCnt.AddUnique(cnt);
	*/
}
