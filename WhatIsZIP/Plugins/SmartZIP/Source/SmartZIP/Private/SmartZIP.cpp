// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "SmartZIP.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "SmartZIPLibrary/xx/ZIP_SDK_LIBRARY/ZIP_SDK_Library.h"

#define LOCTEXT_NAMESPACE "FSmartZIPModule"

FString GetWindowsPlatform()
{
#if PLATFORM_32BITS
	return TEXT("Win32");
#elif PLATFORM_64BITS
	return TEXT("Win64"); 
#endif
}

FString GetSmartWindowsPlatform()
{
#if PLATFORM_32BITS 
	return TEXT("x32"); 
#elif PLATFORM_64BITS
	return TEXT("x64");
#endif
}
void FSmartZIPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("SmartZIP")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if PLATFORM_WINDOWS

#if WITH_EDITOR
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/SmartZIPLibrary")/ GetSmartWindowsPlatform() / ("Release/ZIP_SDK_LIBRARY.dll"));
#endif

#if !WITH_EDITOR
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("../../Binaries") / GetWindowsPlatform() / ("ZIP_SDK_LIBRARY.dll"));
#endif

#elif PLATFORM_MAC
   // LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/SmartZIPLibrary/Mac/Release/ZIP_SDK_Library.dylib"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (ExampleLibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
	//	ExampleLibraryFunction();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
}

void FSmartZIPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSmartZIPModule, SmartZIP)
