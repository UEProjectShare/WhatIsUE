#pragma once

#include "CoreMinimal.h"
#include "SmartHTTPType.h"
#include "SmartHTTPFunctionLibrary.generated.h"

/**
 * A Smart set of HTTP interface functions can quickly perform HTTP code operations.
 * Only one interface is needed to interact with our HTTP server. Currently,
 * HTTP supports downloading, uploading, deleting and other operations.
 * See our API for details
*/
UCLASS(meta = (BlueprintThreadSafe, ScriptName = "HTTPLibrary"))
class SMARTHTTP_API USmartHTTPFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Submit form to server.
	 *
	 * @param InURL						Address to visit.
	 * @param InParam					Parameters passed.
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static bool PostRequest(const FString& InURL, const FString& InParam, const FSmartHTTPBPResponseDelegate& BPResponseDelegate);

	/**
	 *If platform is not turned PLATFORM_PROJECT macro, there is no need to manually put it in the tick of the project
	 * @See SmartHTTP.Build.cs
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static void Tick(float DeltaTime);

	/**
	* The data can be downloaded to local memory via the HTTP serverll .
	*
	* @param BPResponseDelegate	Proxy set relative to the blueprint.
	* @param URL					domain name .
	* @Return						Returns true if the request succeeds
	*/
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static bool GetObjectToMemory(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL);

	/**
	 * Download individual data locally.
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param SavePaths				Path to local storage .
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static bool GetObjectToLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& SavePaths);

	/**
	 * Upload single file from disk to server .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param LocalPaths			Specify the Path where you want to upload the file.
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static bool PutObjectFromLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths);
	
	/**
	 * Can upload byte data .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param Buffer				Byte code data.
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static bool PutObjectFromBuffer(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const TArray<uint8>& Buffer);

	/**
	 * Remove a single object from the server .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|SingleAction")
	static bool DeleteObject(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL);

	/**
	 * Upload duo files from disk to server  .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param LocalPaths			Specify the Path where you want to upload the file.
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|MultpleAction")
	static bool PutObjectsFromLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths);
	
	/**
	 * Download multiple data to local .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					Need domain name .
	 * @param SavePaths				Path to local storage .
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|MultpleAction")
	static void GetObjectsToLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL, const FString& SavePaths);
	
	/**
	 * The data can be downloaded to local memory via the HTTP serverll.
	 * Can download multiple at one time .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					Need domain name .
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|MultpleAction")
	static void GetObjectsToMemory(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL);
	
	/**
	 * Multiple URLs need to be specified to remove multiple objects from the server .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					Need domain name .
	 */
	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|MultpleAction")
	static void DeleteObjects(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL);
};
