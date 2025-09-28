#include "Flow/MetaFacialGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMetaFacialGameInstance::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MetaFacialMap"));
}

void UMetaFacialGameInstance::FinishGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuMap"));
}
