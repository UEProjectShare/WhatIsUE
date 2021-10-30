#include "SmartFileHelperBPLibrary.h"
#include "SmartFileHelper.h"
#include "Misc/FileHelper.h"

USmartFileHelperBPLibrary::USmartFileHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{}

FFileHelper::EHashOptions SmartFileHelper::ToHashOptions(ESmartHashOptions HashOptions)
{
	switch (HashOptions)
	{
	case ESmartHashOptions::EnableVerify:
		/** Enable the async task for verifying the hash for the file being loaded */
		return FFileHelper::EHashOptions::EnableVerify;
	case ESmartHashOptions::ErrorMissingHash:
		/** A missing hash entry should trigger an error */
		return FFileHelper::EHashOptions::ErrorMissingHash;
	}

	return FFileHelper::EHashOptions::None;
}

bool USmartFileHelperBPLibrary::LoadFileToArray(TArray<uint8>& Result, const FString &Filename, int32 Flags /*= 0*/)
{
	return FFileHelper::LoadFileToArray(Result, *Filename, Flags);
}

bool USmartFileHelperBPLibrary::LoadFileToString(FString& Result, const FString &Filename, ESmartHashOptions VerifyFlags /*= ESmartHashOptions::None*/)
{
	return FFileHelper::LoadFileToString(Result, *Filename, SmartFileHelper::ToHashOptions(VerifyFlags));
}

bool USmartFileHelperBPLibrary::LoadFileToStringArray(TArray<FString>& Result, const FString &Filename, ESmartHashOptions VerifyFlags /*= ESmartHashOptions::None*/)
{
	return FFileHelper::LoadFileToStringArray(Result, *Filename, SmartFileHelper::ToHashOptions(VerifyFlags));
}

bool USmartFileHelperBPLibrary::SaveArrayToFile(const TArray<uint8> &Array, const FString &Filename, int32 WriteFlags /*= 0*/)
{
	return FFileHelper::SaveArrayToFile(Array, *Filename, &IFileManager::Get(), WriteFlags);
}

bool USmartFileHelperBPLibrary::SaveStringToFile(const FString& String, const FString &Filename, ESmartEncodingOptions EncodingOptions /*= ESmartEncodingOptions::AutoDetect*/, int32 WriteFlags /*= 0*/)
{
	return FFileHelper::SaveStringToFile(String, *Filename, (FFileHelper::EEncodingOptions)EncodingOptions, &IFileManager::Get(), WriteFlags);
}

bool USmartFileHelperBPLibrary::SaveStringArrayToFile(const TArray<FString>& Lines, const FString &Filename, ESmartEncodingOptions EncodingOptions /*= ESmartEncodingOptions::AutoDetect*/, int32 WriteFlags /*= 0*/)
{
	return FFileHelper::SaveStringArrayToFile(Lines, *Filename, (FFileHelper::EEncodingOptions)EncodingOptions, &IFileManager::Get(), WriteFlags);
}

bool USmartFileHelperBPLibrary::GenerateNextBitmapFilename(const FString& Pattern, const FString& Extension, FString& OutFilename)
{
	return FFileHelper::GenerateNextBitmapFilename(*Pattern, Extension, OutFilename);
}

bool USmartFileHelperBPLibrary::CreateBitmap(const FString &Pattern, int32 DataWidth, int32 DataHeight, const FColor &Data, FVector4 SubRectangle, FString &OutFilename, bool bInWriteAlpha /*= false*/)
{
	TSharedPtr<FIntRect> Rect = nullptr;
	if (!SubRectangle.IsNearlyZero3(0))
	{
		Rect = MakeShareable(new FIntRect);
		Rect->Min.X = SubRectangle.X;
		Rect->Min.Y = SubRectangle.Y;
		Rect->Max.X = SubRectangle.Z;
		Rect->Max.Y = SubRectangle.W;
	}
	 
	return FFileHelper::CreateBitmap(
		*Pattern, 
		DataWidth, 
		DataHeight, 
		&Data, 
		Rect.Get(),
		&IFileManager::Get(),
		(OutFilename.IsEmpty() ? NULL : &OutFilename),
		bInWriteAlpha);
}

bool USmartFileHelperBPLibrary::LoadANSITextFileToStrings(const FString &InFilename, TArray<FString>& OutStrings)
{
	return FFileHelper::LoadANSITextFileToStrings(*InFilename, &IFileManager::Get(), OutStrings);
}

bool USmartFileHelperBPLibrary::IsFilenameValidForSaving(const FString& Filename, FText& OutError)
{
	return FFileHelper::IsFilenameValidForSaving(Filename, OutError);
}
