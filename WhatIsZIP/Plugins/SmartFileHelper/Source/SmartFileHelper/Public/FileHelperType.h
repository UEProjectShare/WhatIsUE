#pragma once

#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "FileHelperType.generated.h"


UENUM()
enum class ESmartHashOptions : uint8
{
	None = 0,
	/** Enable the async task for verifying the hash for the file being loaded */
	EnableVerify,
	/** A missing hash entry should trigger an error */
	ErrorMissingHash,
};

UENUM()
enum class ESmartEncodingOptions : uint8
{
	AutoDetect,
	ForceAnsi,
	ForceUnicode,
	ForceUTF8,
	ForceUTF8WithoutBOM
};

namespace SmartFileHelper
{
	FFileHelper::EHashOptions ToHashOptions(ESmartHashOptions HashOptions);
};