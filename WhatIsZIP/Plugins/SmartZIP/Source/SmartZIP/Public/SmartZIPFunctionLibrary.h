#pragma once

#include "CoreMinimal.h"
#include "ZIPType.h"
#include "SmartZIPFunctionLibrary.generated.h"

UCLASS()
class SMARTZIP_API USmartZIPFunctionLibrary : public UObject
{
	GENERATED_BODY()

	/**
	 * Using UE4 compression scheme to compress our files.
	 * @param UncompressedData	Data to be compressed.
	 * @param OutData			Compress the output data.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static bool Compress(const TArray<uint8> &UncompressedData,TArray<uint8> &OutData);

	/**
	 * Use UE4 to decompress.
	 * @param CompressedData	Data to be decompress.
	 * @param OutData			Decompress the output data.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static bool Decompress(const TArray<uint8> &CompressedData, TArray<uint8> &OutData);
	
	/**
	 * The compression scheme zip provided by windows can be used to compress and encrypt the scheme.
	 * @param SrcPath				File path to compress.  for example D:\Hello\Plugins\Ok\File\
	 * @param DestFilename			Compressed to target file plus file name. for example D:\Hello\Plugins\xxx.zip
	 * @param Password				Write a password so that others need a password to unzip it.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static bool ZipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password,bool bOverwrite = true);
	
	/**
	 * The compression scheme zip provided by windows can be used to compress and encrypt the scheme.
	 * @param SrcPath				File path to decompress.  for example  D:\Hello\Plugins\xxx.zip
	 * @param DestFilename			Decompressed to target file plus file name. for example D:\Hello\Plugins\Ok\File\
	 * @param Password				You need a password to unzip your zip.
	 * @param bOverwrite			WIf the file already exists, do you want to override it.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static bool UnzipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password, bool bOverwrite = true);

	//Async
//////////////////////////////////////////////////////////////////////////
	/**
	 * If you want to execute this compression asynchronously, you need to help that function.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static void BindZipFileCallbackFunction(FOnZipFileCallbackFunction OnFunction);

	/**
	 * Remember to clear the agent when you don't use it, otherwise it may collapse next time.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static void ClearZipFileCallbackFunction();

	/**
	 * The compression scheme zip provided by windows can be used to compress and encrypt the scheme as async.
	 * @param SrcPath				File path to compress.  for example D:\Hello\Plugins\Ok\File\
	 * @param DestFilename			Compressed to target file plus file name. for example D:\Hello\Plugins\xxx.zip
	 * @param Password				Write a password so that others need a password to unzip it.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static void AsyncZipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite = true);

	/**
	 * The compression scheme zip provided by windows can be used to compress and encrypt the scheme as async.
	 * @param SrcPath				File path to decompress.  for example  D:\Hello\Plugins\xxx.zip
	 * @param DestFilename			Decompressed to target file plus file name. for example D:\Hello\Plugins\Ok\File\
	 * @param Password				You need a password to unzip your zip.
	 * @param bOverwrite			WIf the file already exists, do you want to override it.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartZIP")
	static void AsyncUnzipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite = true);
};