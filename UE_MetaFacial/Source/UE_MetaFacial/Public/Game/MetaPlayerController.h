
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MetaPlayerController.generated.h"

enum class EMatchPhase : uint8;
class UUserWidget; 

UCLASS()
class UE_METAFACIAL_API AMetaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowScreen(EMatchPhase Phase);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EMatchPhase, TSubclassOf<UUserWidget>> WidgetMap;

private:

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
