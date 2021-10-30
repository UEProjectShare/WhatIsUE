#pragma once
#include "CoreMinimal.h"


namespace SmartZIP
{
	struct SMARTZIP_API FCompressAssist
	{
		FCompressAssist(TArray<uint8> &InData);

		bool Compress(const TArray<uint8> &UncompressedData);
		bool Decompress(const TArray<uint8> &CompressedData);
	private:
		TArray<uint8> &Data;
	};
}