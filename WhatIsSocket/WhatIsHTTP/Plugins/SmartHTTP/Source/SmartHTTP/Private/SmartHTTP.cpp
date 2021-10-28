#include "SmartHTTP.h"
#include "SmartHttpManage.h"

#define LOCTEXT_NAMESPACE "FSmartHTTPModule"

void FSmartHTTPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSmartHTTPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSmartHttpManage::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSmartHTTPModule, SmartHTTP)