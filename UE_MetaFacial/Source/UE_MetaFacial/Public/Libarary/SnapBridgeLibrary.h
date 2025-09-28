
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SnapBridgeLibrary.generated.h"

UCLASS()
class UE_METAFACIAL_API USnapBridgeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "SnapCam")
	static bool RunProcessAndWait(int32 TimeoutMs, int32& OutReturnCode);
	
};
