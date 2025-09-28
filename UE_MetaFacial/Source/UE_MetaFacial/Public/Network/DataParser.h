// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DataStructures.h"
#include "DataParser.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DataParserLog, Display, Log)

UCLASS()
class UE_METAFACIAL_API UDataParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========================================================================================
	// JSON Serialize Functions (Client → Server)
	// ========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Network|Serialize")
	static FString SerializeSendImageField(const FSendImageField& Data);

	UFUNCTION(BlueprintCallable, Category = "Network|Serialize")
	static bool SerializeToJsonString(const FSendImageField& Data, FString& OutJsonString);

	// ========================================================================================
	// JSON Deserialize Functions (Server → Client)
	// ========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Network|Deserialize")
	static bool DeserializeReceivedNetState(const FString& JsonString, FReceivedNetState& OutData);

	UFUNCTION(BlueprintCallable, Category = "Network|Deserialize")
	static bool DeserializeNetPayload(const FString& JsonString, FNetPayload& OutData);

	UFUNCTION(BlueprintCallable, Category = "Network|Deserialize")
	static bool DeserializeResultImageData(const FString& JsonString, FResultImageData& OutData);

	// ========================================================================================
	// JSON Array Deserialize Functions
	// ========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Network|Deserialize|Array")
	static bool DeserializeResultImageDataArray(const FString& JsonString, TArray<FResultImageData>& OutDataArray);

	// ========================================================================================
	// Utility Functions
	// ========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Network|Utility")
	static bool IsValidJsonString(const FString& JsonString);

	UFUNCTION(BlueprintCallable, Category = "Network|Utility")
	static bool IsValidJsonObject(const FString& JsonString);

	UFUNCTION(BlueprintCallable, Category = "Network|Utility")
	static bool IsValidJsonArray(const FString& JsonString);

	// C++ 전용 헬퍼 함수들
	static TSharedPtr<FJsonObject> ParseJsonObject(const FString& inJsonString);
	static TArray<TSharedPtr<FJsonValue>> ParseJsonArray(const FString& inJsonString);
	

private:
	// 내부 헬퍼 함수들
	static bool SerializeStructToJson(const void* StructPtr, const UStruct* StructDef, FString& outJsonString);
	static bool DeserializeJsonToStruct(const FString& JsonString, void* StructPtr, const UStruct* StructDef);
};
