#include "SmartZIPFunctionLibrary.h"
#include "SmartCompress.h"
#include "ZIPSDK.h"

bool USmartZIPFunctionLibrary::Compress(const TArray<uint8> &UncompressedData, TArray<uint8> &OutData)
{
	SmartZIP::FCompressAssist CompressAssist(OutData);
	return CompressAssist.Compress(UncompressedData);
}

bool USmartZIPFunctionLibrary::Decompress(const TArray<uint8> &CompressedData, TArray<uint8> &OutData)
{
	SmartZIP::FCompressAssist CompressAssist(OutData);
	return CompressAssist.Decompress(CompressedData);
}

bool USmartZIPFunctionLibrary::ZipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password, bool bOverwrite)
{
	return SmartZIP::ZipByLoacl(*SrcPath, *DestFilename, *Password, bOverwrite);
}

bool USmartZIPFunctionLibrary::UnzipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password, bool bOverwrite /*= true*/)
{
	return SmartZIP::UnzipByLoacl(*SrcPath, *DestFilename, *Password,bOverwrite);
}

void USmartZIPFunctionLibrary::BindZipFileCallbackFunction(FOnZipFileCallbackFunction OnFunction)
{
	SmartZIP::BindFunction([OnFunction](EZipType InType, int32 Num, int32 InProgress, const FString& InFileName)
	{
		OnFunction.ExecuteIfBound(InType, Num, InProgress, InFileName);
	});
}

void USmartZIPFunctionLibrary::ClearZipFileCallbackFunction()
{
	SmartZIP::ClearZipFileCallbackFunction();
}

void USmartZIPFunctionLibrary::AsyncZipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite)
{
	SmartZIP::AsyncZipByLoacl(SrcPath, DestFilename, Password, bOverwrite);
}

void USmartZIPFunctionLibrary::AsyncUnzipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite /*= true*/)
{
	SmartZIP::AsyncUnZipByLoacl(SrcPath, DestFilename, Password, bOverwrite);
}
