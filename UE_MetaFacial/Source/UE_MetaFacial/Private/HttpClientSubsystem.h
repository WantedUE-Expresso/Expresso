#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpClientSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(NetHttpLog, Display, Log)

USTRUCT(BlueprintType)
struct UE_METAFACIAL_API FHttpPayload
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpPayload")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpPayload")
	int32 Age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpPayload")
	TArray<FString> Item;
};

UCLASS()
class UE_METAFACIAL_API UHttpClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
};
