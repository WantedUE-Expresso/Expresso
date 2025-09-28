
#include "Game/MetaGameState.h"
#include "Game/MetaPlayerController.h"
#include "Net/UnrealNetwork.h"


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

void AMetaGameState::OnRep_Scores()
{
}
