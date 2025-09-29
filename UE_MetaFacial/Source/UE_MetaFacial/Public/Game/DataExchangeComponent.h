#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataExchangeComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DataExchangeLog, Display, All);

USTRUCT(BlueprintType)
struct UE_METAFACIAL_API FRoundStartData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ID")
	FString id;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ID")
	//FString 
	
	
};


USTRUCT(BlueprintType)
struct UE_METAFACIAL_API FRoundResultData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ID")
	FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float score;

	// metaHuman Texture
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* texture;
};


// Attach to GameState Class
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_METAFACIAL_API UDataExchangeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDataExchangeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartRound();
	UFUNCTION(BlueprintCallable)
	bool ReceiveRoundResultData(TArray<FRoundResultData>& outData);
	UFUNCTION(BlueprintCallable)
	void SendRoundData();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsReady() {return bReadyForResult;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetStageInfo() {return stage;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UTexture2D* GetMyMetaHumanImage() const {return myMetaHumanTexture;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UTexture2D* GetOpponentMetaHumanImage() const {return opponentMetaHumanTexture;}
	

protected:
	UFUNCTION(BlueprintCallable)
	UTexture2D* DecodeBase64ToImage(const FString& inBase64String);
	UFUNCTION(BlueprintCallable)
	bool EncodeImageToBase64(FString& outBase64String, FString& outRealHumanBase64String);

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image|Path")
	FString myFaceImageName = "Webcam/snap.png";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image|Path")
	FString myMetaHumanImageName = "PictureMetaHuman/MetaHuman.png";
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Image|Texture2D")
	UTexture2D* myMetaHumanTexture = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Image|Texture2D")
	UTexture2D* opponentMetaHumanTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString stage;

private:
	bool bReadyForResult = false;
};

