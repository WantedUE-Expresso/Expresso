#include "Network/DataParser.h"
#include "Json.h"
#include "JsonUtilities.h"

DEFINE_LOG_CATEGORY(DataParserLog)

// ========================================================================================
// JSON Serialize Functions (Client → Server)
// ========================================================================================

FString UDataParser::SerializeSendImageField(const FSendImageField& Data)
{
	FString OutJsonString;
	if (SerializeToJsonString(Data, OutJsonString))
	{
		return OutJsonString;
	}
	return FString();
}

bool UDataParser::SerializeToJsonString(const FSendImageField& Data, FString& OutJsonString)
{
	UE_LOG(DataParserLog, Warning, TEXT("SerializeToJsonString called with id: %s"), *Data.id);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField(TEXT("id"), Data.id);
	JsonObject->SetStringField(TEXT("base64RealHumanImage"), Data.base64RealHumanImage);
	JsonObject->SetStringField(TEXT("base64MetaHumanImage"), Data.base64MetaHumanImage);

	UE_LOG(DataParserLog, Warning, TEXT("JsonObject created with fields - id: %s, realImage length: %d, metaImage length: %d"),
		*Data.id, Data.base64RealHumanImage.Len(), Data.base64MetaHumanImage.Len());

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(DataParserLog, Warning, TEXT("Successfully serialized FSendImageField: %s"), *OutJsonString);
		return true;
	}

	UE_LOG(DataParserLog, Error, TEXT("Failed to serialize FSendImageField"));
	return false;
}

// ========================================================================================
// JSON Deserialize Functions (Server → Client)
// ========================================================================================

bool UDataParser::DeserializeReceivedNetState(const FString& JsonString, FReceivedNetState& OutData)
{
	UE_LOG(DataParserLog, Warning, TEXT("DeserializeReceivedNetState called with: %s"), *JsonString);

	TSharedPtr<FJsonObject> JsonObject = ParseJsonObject(JsonString);
	if (!JsonObject.IsValid())
	{
		UE_LOG(DataParserLog, Error, TEXT("Failed to parse FReceivedNetState JSON: %s"), *JsonString);
		return false;
	}

	UE_LOG(DataParserLog, Warning, TEXT("JsonObject parsed successfully"));

	bool bCodeResult = JsonObject->TryGetStringField(TEXT("code"), OutData.code);
	UE_LOG(DataParserLog, Warning, TEXT("Code extraction result: %s, Value: %s"), bCodeResult ? TEXT("Success") : TEXT("Failed"), *OutData.code);

	bool bStatusResult = JsonObject->TryGetStringField(TEXT("status"), OutData.status);
	UE_LOG(DataParserLog, Warning, TEXT("Status extraction result: %s, Value: %s"), bStatusResult ? TEXT("Success") : TEXT("Failed"), *OutData.status);

	bool bMessageResult = JsonObject->TryGetStringField(TEXT("message"), OutData.message);
	UE_LOG(DataParserLog, Warning, TEXT("Message extraction result: %s, Value: %s"), bMessageResult ? TEXT("Success") : TEXT("Failed"), *OutData.message);
	const TSharedPtr<FJsonObject>* infoObject;
	JsonObject->TryGetObjectField(TEXT("info"), infoObject);

	if (!infoObject->IsValid())
	{
		return true;
	}
	UE_LOG(DataParserLog, Log, TEXT("Successfully deserialized FReceivedNetState - Code: %s"), *OutData.code);
	const TArray<TSharedPtr<FJsonValue>>* StageArray;
	if ((*infoObject)->TryGetArrayField(TEXT("stage"), StageArray) && StageArray->Num() > 0)
	{
		if (StageArray->Num() > 1)
		{
			FString StageValue;
			if ((*StageArray)[1]->TryGetString(StageValue))
			{
				OutData.info = StageValue;
			}
		}
	}
	const TArray<TSharedPtr<FJsonValue>>* ResultArray;
	if ((*infoObject)->TryGetArrayField(TEXT("stage"), ResultArray) && ResultArray->Num() > 0)
	{
		if (ResultArray->Num() > 1)
		{
			FString StageValue;
			if ((*ResultArray)[1]->TryGetString(StageValue))
			{
				OutData.info = StageValue;
			}
		}
	}

	return true;
}

bool UDataParser::DeserializeNetPayload(const FString& JsonString, FNetPayload& OutData)
{
	TSharedPtr<FJsonObject> JsonObject = ParseJsonObject(JsonString);
	if (!JsonObject.IsValid())
	{
		UE_LOG(DataParserLog, Error, TEXT("Failed to parse FNetPayload JSON: %s"), *JsonString);
		return false;
	}

	JsonObject->TryGetStringField(TEXT("name"), OutData.name);

	const TArray<TSharedPtr<FJsonValue>>* ResultsArray;
	if (JsonObject->TryGetArrayField(TEXT("results"), ResultsArray))
	{
		OutData.results.Empty();
		for (const auto& ResultValue : *ResultsArray)
		{
			TSharedPtr<FJsonObject>* ResultObject;
			if (ResultValue->TryGetObject(ResultObject) && ResultObject->IsValid())
			{
				FResultImageData ResultData;
				(*ResultObject)->TryGetStringField(TEXT("id"), ResultData.id);
				(*ResultObject)->TryGetNumberField(TEXT("scores"), ResultData.scores);
				(*ResultObject)->TryGetStringField(TEXT("base64MetaHumanImages"), ResultData.base64MetaHumanImages);
				OutData.results.Add(ResultData);
			}
		}
	}

	UE_LOG(DataParserLog, Log, TEXT("Successfully deserialized FNetPayload - Name: %s, Results Count: %d"),
		*OutData.name, OutData.results.Num());
	return true;
}

bool UDataParser::DeserializeResultImageData(const FString& JsonString, FResultImageData& OutData)
{
	TSharedPtr<FJsonObject> JsonObject = ParseJsonObject(JsonString);
	if (!JsonObject.IsValid())
	{
		return false;
	}

	JsonObject->TryGetStringField(TEXT("id"), OutData.id);
	JsonObject->TryGetNumberField(TEXT("scores"), OutData.scores);
	JsonObject->TryGetStringField(TEXT("base64MetaHumanImages"), OutData.base64MetaHumanImages);

	return true;
}

// ========================================================================================
// JSON Array Deserialize Functions
// ========================================================================================

bool UDataParser::DeserializeResultImageDataArray(const FString& JsonString, TArray<FResultImageData>& OutDataArray)
{
	TArray<TSharedPtr<FJsonValue>> JsonArray = ParseJsonArray(JsonString);
	if (JsonArray.Num() == 0)
	{
		UE_LOG(DataParserLog, Warning, TEXT("Empty or invalid JSON array for FResultImageData"));
		return false;
	}

	OutDataArray.Empty();
	for (const auto& JsonValue : JsonArray)
	{
		TSharedPtr<FJsonObject>* JsonObject;
		if (JsonValue->TryGetObject(JsonObject) && JsonObject->IsValid())
		{
			FResultImageData ResultData;
			(*JsonObject)->TryGetStringField(TEXT("id"), ResultData.id);
			(*JsonObject)->TryGetNumberField(TEXT("scores"), ResultData.scores);
			(*JsonObject)->TryGetStringField(TEXT("base64MetaHumanImages"), ResultData.base64MetaHumanImages);
			OutDataArray.Add(ResultData);
		}
	}

	UE_LOG(DataParserLog, Log, TEXT("Successfully deserialized FResultImageData array - Count: %d"), OutDataArray.Num());
	return OutDataArray.Num() > 0;
}

// ========================================================================================
// Utility Functions
// ========================================================================================

bool UDataParser::IsValidJsonString(const FString& JsonString)
{
	return IsValidJsonObject(JsonString) || IsValidJsonArray(JsonString);
}

bool UDataParser::IsValidJsonObject(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	return FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid();
}

bool UDataParser::IsValidJsonArray(const FString& JsonString)
{
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	return FJsonSerializer::Deserialize(Reader, JsonArray);
}

TSharedPtr<FJsonObject> UDataParser::ParseJsonObject(const FString& inJsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(inJsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		return JsonObject;
	}

	return nullptr;
}

TArray<TSharedPtr<FJsonValue>> UDataParser::ParseJsonArray(const FString& inJsonString)
{
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(inJsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		return JsonArray;
	}

	return TArray<TSharedPtr<FJsonValue>>();
}

// ========================================================================================
// Private Helper Functions
// ========================================================================================

bool UDataParser::SerializeStructToJson(const void* StructPtr, const UStruct* StructDef, FString& outJsonString)
{
	if (!StructPtr || !StructDef)
	{
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	if (FJsonObjectConverter::UStructToJsonObject(StructDef, StructPtr, JsonObject.ToSharedRef()))
	{
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outJsonString);
		return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	}

	return false;
}

bool UDataParser::DeserializeJsonToStruct(const FString& JsonString, void* StructPtr, const UStruct* StructDef)
{
	if (!StructPtr || !StructDef)
	{
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject = ParseJsonObject(JsonString);
	if (!JsonObject.IsValid())
	{
		return false;
	}

	return FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StructDef, StructPtr);
}
