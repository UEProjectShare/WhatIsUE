#pragma once

#include "CoreMinimal.h"

namespace SmartZIP
{
	SMARTZIP_API bool ZipByLoacl(const TCHAR* SrcPath, const TCHAR* DestFilename, const TCHAR* Password, bool bOverwrite = true);
	SMARTZIP_API bool UnzipByLoacl(const TCHAR* SrcPath, const TCHAR* DestFilename, const TCHAR* Password, bool bOverwrite = true);
	
	SMARTZIP_API void BindFunction(TFunction<void(EZipType, int32, int32, const FString&)> InZipFunction);
	SMARTZIP_API void ClearZipFileCallbackFunction();

	SMARTZIP_API void AsyncZipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite);
	SMARTZIP_API void AsyncUnZipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite = true);
}