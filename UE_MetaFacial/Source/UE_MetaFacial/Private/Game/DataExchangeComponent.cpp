#include "Game/DataExchangeComponent.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
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
	
	// cleanup base64 string from web
	int32 index;
	FString base64String = inBase64String;
	if (inBase64String.FindChar(TEXT(','), index))
	{
		base64String = inBase64String.RightChop(index + 1);
	}

	// Decode
	TArray<uint8> imageBuffer;
	if (FBase64::Decode(base64String, imageBuffer))
	{
		UE_LOG(DataExchangeLog, Error, TEXT("Failed to decode image."));
		return nullptr;
	}

	// create compressed buffer
	IImageWrapperModule* imageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>("ImageWrapper");
	if (!imageWrapperModule)
	{
		UE_LOG(DataExchangeLog, Error, TEXT("Failed to load image wrapper module."));
		return nullptr;
	}
	TSharedPtr<IImageWrapper> pngWrapper = imageWrapperModule->CreateImageWrapper(EImageFormat::PNG);
	if (!pngWrapper.IsValid() || !pngWrapper->SetCompressed(imageBuffer.GetData(), imageBuffer.Num()))
	{
		UE_LOG(DataExchangeLog, Error, TEXT("Failed to create image wrapper or compressed buffer."));
		return nullptr;
	}

	// extract data and convert to UE format 
	TArray<uint8> extractedImageBuffer;
	if (!pngWrapper->GetRaw(ERGBFormat::BGRA, 8, extractedImageBuffer))
	{
		UE_LOG(DataExchangeLog, Error, TEXT("Failed to extract image buffer."));
	}
	UTexture2D* texture = UTexture2D::CreateTransient(pngWrapper->GetWidth(), pngWrapper->GetHeight(), EPixelFormat::PF_B8G8R8A8);
	if (!IsValid(texture))
	{
		UE_LOG(DataExchangeLog, Error, TEXT("Failed to import image."));
		return nullptr;
	}

	// copy image maps Directly
	if (texture->GetPlatformData() && texture->GetPlatformData()->Mips.Num() > 0)
	{
		FTexture2DMipMap& Mip = texture->GetPlatformData()->Mips[0];
		if (void* textureData = Mip.BulkData.Lock(LOCK_READ_WRITE))
		{
			const int32 bufferSize = extractedImageBuffer.Num();
			Mip.BulkData.Realloc(bufferSize);
			FMemory::Memcpy(textureData, extractedImageBuffer.GetData(), bufferSize);
			Mip.BulkData.Unlock();
			
			texture->UpdateResource();
			bReadyForResult = true;
		}
		
		UE_LOG(DataExchangeLog, Error, TEXT("Failed to lock texture bulk data for memory copy."));
	}
	UE_LOG(DataExchangeLog, Error, TEXT("Invalid PlatformData or Mips array."));
	// Directly import to Texture2D. It is Weak by Hitch.
	if (!bReadyForResult && FBase64::Decode(base64String, imageBuffer, EBase64Mode::Standard))
	{
		texture = FImageUtils::ImportBufferAsTexture2D(imageBuffer);
		if (!IsValid(texture))
		{
			UE_LOG(DataExchangeLog, Error, TEXT("Failed to decode image."));
			return nullptr;
		}
		bReadyForResult = true;
	}
	
	return texture;
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
	TArray<uint8> imageBuffer;
	if (!FFileHelper::LoadFileToArray(imageBuffer, *fullPath))
	{
		UE_LOG(DataExchangeLog, Display, TEXT("Can not Load Image File to Binary Data"));
		return false;
	}
	
	outBase64String = FBase64::Encode(imageBuffer, EBase64Mode::Standard);
	return true;
}
