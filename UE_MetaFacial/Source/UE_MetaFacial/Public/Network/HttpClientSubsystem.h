#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpClientSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(NetHttpLog, Display, Log)

UCLASS()
class UE_METAFACIAL_API UHttpClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
