
#include "Libarary/SnapBridgeLibrary.h"

bool USnapBridgeLibrary::RunProcessAndWait(const FString& ExecPath, const FString& Arguments, const FString& WorkingDir,
	int32 TimeoutMs, int32& OutReturnCode)
{
	OutReturnCode = -1;

	const FString SnapDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/ThirdParty/SnapCam"));
	const FString SnapExe = FPaths::Combine(SnapDir, TEXT("SnapCam.exe"));

	// 권장: Saved 폴더
	const FString OutAbs = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Webcam/snap.png"));   
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(OutAbs), true);

	const FString Args = FString::Printf(
	TEXT("--device 0 --width 1280 --height 720 --warmup 2 --timeout_ms 2000 --out \"%s\""), *OutAbs);

	FProcHandle Proc = FPlatformProcess::CreateProc(
		*SnapExe, *Args,
		/*Detached*/ false,
		/*Hidden*/ true,
		/*ReallyHidden*/ true,
		/*OutPID*/ nullptr,
		/*Priority*/ 0,
		/*WorkingDir*/ WorkingDir.IsEmpty() ? nullptr : *WorkingDir,
		/*PipeWrite*/ nullptr,
		/*PipeRead*/ nullptr);

	if (!Proc.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecPath : %s / WorkingDir : %s"), *ExecPath, *WorkingDir);
		
		return false;
	}

	const double Start = FPlatformTime::Seconds();

	while (FPlatformProcess::IsProcRunning(Proc))
	{
		if (TimeoutMs > 0)
		{
			const double ElapsedMs = (FPlatformTime::Seconds() - Start) * 1000.0;
			if (ElapsedMs > TimeoutMs)
			{
				FPlatformProcess::TerminateProc(Proc, true);
				break;
			}
		}
		
		FPlatformProcess::Sleep(0.01f);
	}

	bool bGotCode = FPlatformProcess::GetProcReturnCode(Proc, &OutReturnCode);
	FPlatformProcess::CloseProc(Proc);
	
	
	
	return bGotCode;
	
}
