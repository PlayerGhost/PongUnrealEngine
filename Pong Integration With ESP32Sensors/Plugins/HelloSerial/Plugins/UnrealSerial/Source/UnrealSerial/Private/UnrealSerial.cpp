// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IUnrealSerial.h"


class FUnrealSerial : public IUnrealSerial
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FUnrealSerial, UnrealSerial )

DEFINE_LOG_CATEGORY(ModuleLog)

void FUnrealSerial::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	UE_LOG(ModuleLog, Warning, TEXT("UnrealSerial: Plugin Started"));
}


void FUnrealSerial::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(ModuleLog, Warning, TEXT("UnrealSerial: Plugin Ended"));
}



