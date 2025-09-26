#pragma once

#include "CoreMinimal.h"
#include "DataStructures.generated.h"

USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FHttpPost
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientKey")
	FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post")
	FString base64Image;
};

USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FHttpPayload
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientKey")
	TArray<FString> keys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	TArray<float> scores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	TArray<FString> base64Image;
};
