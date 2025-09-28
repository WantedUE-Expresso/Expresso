
#include "Game/MetaPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMetaPlayerController::ShowScreen(EMatchPhase Phase)
{
	if (!WidgetMap.Contains(Phase)) return;

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClass = WidgetMap[Phase];
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (!NewWidget) return;

	CurrentWidget = NewWidget;
	CurrentWidget->AddToViewport();

	FInputModeUIOnly UIInputMode;
	SetInputMode(UIInputMode);
	SetShowMouseCursor(false);
}
