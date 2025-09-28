#include "Game/DataExchangeComponent.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Network/SocketClientSubsystem.h"

DEFINE_LOG_CATEGORY(DataExchangeLog);

UDataExchangeComponent::UDataExchangeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDataExchangeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDataExchangeComponent::StartRound()
{
}

bool UDataExchangeComponent::ReceiveRoundResultData(TArray<FRoundResultData>& outData)
{
	// Receiving
	
	
	bReadyForResult = false;
	return false;
}

void UDataExchangeComponent::SendRoundData()
{
}

UTexture2D* UDataExchangeComponent::DecodeBase64ToImage(const FString& inBase64String)
{
	IImageWrapperModule* imageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>("ImageWrapper");
	if (!imageWrapperModule)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load image wrapper module."));
		return nullptr;
	}
	TSharedPtr<IImageWrapper> pngWrapper = imageWrapperModule->CreateImageWrapper(EImageFormat::PNG);
	if (!pngWrapper.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create image wrapper."));
		return nullptr;
	}

	FImage OutImage;


	// Image Decoding Finished
	bReadyForResult = true;
	return nullptr;
}

bool UDataExchangeComponent::EncodeImageToBase64(const UTexture2D* inPngImage, FString& outBase64String)
{
	if (myFaceImageName.IsEmpty() || myMetaHumanImageName.IsEmpty())
	{
		UE_LOG(DataExchangeLog, Display, TEXT("Image Path is not Valid"));
		return false;
	}
	
	FString fullPath = FPaths::Combine(FPaths::ProjectSavedDir(), myFaceImageName);
	if (!IFileManager::Get().FileExists(*fullPath))
	{
		UE_LOG(DataExchangeLog, Display, TEXT("Image File is not existed"));
		return false;
	}

	// 압축 파일로 보내게 되는데.. 이거 어떻게하지.. 원시 데이터는 좀 클거같은데..
	TArray<uint8> compressedData;
	if (!FFileHelper::LoadFileToArray(compressedData, *fullPath))
	{
		UE_LOG(DataExchangeLog, Display, TEXT("Can not Load Image File to Binary Data"));
		return false;
	}
	
	outBase64String = FBase64::Encode(compressedData, EBase64Mode::Standard);
	return true;
}
