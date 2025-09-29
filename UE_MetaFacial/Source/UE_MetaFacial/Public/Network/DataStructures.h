#pragma once

#include "CoreMinimal.h"
#include "DataStructures.generated.h"

// Client가 보낼 JSON
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

// 결과 JSON Parsing
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

// 결과로 받을 JSON
USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FNetPayload
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	TArray<FResultImageData> results;
};

/* 예시
{
	"code": "203",
	"result": "ON",
	"message": "라운드 1",
	"stage": [
		0,
		"분노"
	]
}
 */
USTRUCT(BlueprintType, Atomic)
struct UE_METAFACIAL_API FReceivedNetState
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	FString code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	FString status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	FString message;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload")
	FString info;
};
