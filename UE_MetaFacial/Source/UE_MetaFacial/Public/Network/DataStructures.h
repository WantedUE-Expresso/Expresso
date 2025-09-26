#pragma once

#include "CoreMinimal.h"
#include "DataStructures.generated.h"

USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FSendImageField
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientKey")
	FString id;  // Client ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	FString base64RealHumanImage;  // 진짜 사람 얼굴

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	FString base64MetaHumanImage;  // 메타 휴먼 얼굴
};

USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FResultImageData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClientKey")
	FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	float scores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	FString base64MetaHumanImages;
};


USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FNetPayload
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	TArray<FResultImageData> results;
};

USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FReceivedNetData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	FString name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paylaod")
	FString url;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paylaod")
	//TArray<FNetPayload> payloads;
};
