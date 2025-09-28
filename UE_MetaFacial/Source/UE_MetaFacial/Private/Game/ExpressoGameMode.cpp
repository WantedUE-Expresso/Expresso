
#include "Game/ExpressoGameMode.h"

#include "LiveLinkCompression.h"
#include "Game/MetaGameState.h"
#include "Game/MetaPlayerController.h"

AExpressoGameMode::AExpressoGameMode()
{
	GameStateClass = AMetaGameState::StaticClass();
	PlayerControllerClass = AMetaPlayerController::StaticClass();
}

void AExpressoGameMode::SwitchPhase(EMatchPhase Phase)
{
	AMetaGameState* GS = GetGameState<AMetaGameState>();
	if (!GS) return;

	GS->SetPhase(Phase);
}

bool AExpressoGameMode::HasReceivedResult()
{
	return bReceiveResult;
}

