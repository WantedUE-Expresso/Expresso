#pragma once

namespace Data
{
	USTRUCT(BlueprintType)
	struct UE_METAFACIAL_API FHttpPayload
	{
		GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		FString name;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
		TArray<float> scores;
	};

	USTRUCT(BlueprintType)
	struct UE_METAFACIAL_API FFacialData
	{
		GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpPayload")
		FString name;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpPayload")
		TArray<float> scores;
	};
};
