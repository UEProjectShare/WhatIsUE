#pragma once

#include "CoreMinimal.h"
#include "HTTP/SmartHTTPHandle.h"
#include "SmartHTTPType.h"
#ifdef PLATFORM_PROJECT
#include "Tickable.h"
#endif

struct FSmartHttpActionRequest;

/*
 * A Smart set of HTTP interface functions can quickly perform HTTP code operations. 
 * Only one interface is needed to interact with our HTTP server. Currently, 
 * HTTP supports downloading, uploading, deleting and other operations. 
 * See our API for details
*/
class SMARTHTTP_API FSmartHttpManage 
#ifdef PLATFORM_PROJECT
	: public FTickableGameObject
#endif
{
	/**
	 * Used to determine whether the object should be ticked in the editor.  Defaults to false since
	 * that is the previous behavior.
	 *
	 * @return	true if this tickable object can be ticked in the editor
	 */
	virtual bool IsTickableInEditor() const;

	/** return the stat id to use for this tickable **/
	virtual TStatId GetStatId() const;

	/** Get HTTP function collection  **/
	struct SMARTHTTP_API FHTTP
	{
		/*We want to have direct access to the commands in http .*/
		friend class FSmartHttpManage;

		/**
		 * Submit form to server.
		 *
		 * @param InURL						Address to visit.
		 * @param InParam					Parameters passed.
		 * @param BPResponseDelegate		Proxy for site return.
		 */
		bool PostRequest(const TCHAR* InURL, const TCHAR* InParam, const FSmartHTTPBPResponseDelegate& BPResponseDelegate);

		/**
		 * The data can be downloaded to local memory via the HTTP serverll .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds 
		 */
		bool GetObjectToMemory(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL);
		
		/**
		 * The data can be downloaded to local memory via the HTTP serverll.
		 * Can download multiple at one time .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					Need domain name .
		 */
		void GetObjectsToMemory(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL);

		/**
		 * Download individual data locally.
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param SavePaths				Path to local storage .
		 * @Return						Returns true if the request succeeds 
		 */
		bool GetObjectToLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& SavePaths);
		
		/**
		 * Download multiple data to local .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					Need domain name .
		 * @param SavePaths				Path to local storage .
		 */
		void GetObjectsToLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL, const FString& SavePaths);

		/**
		 * Upload single file from disk to server .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths);
		
		/**
		 * Upload multiple files from disk to server.
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectsFromLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths);

		/**
		 * Can upload byte data .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param Buffer				Byte code data.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromBuffer(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const TArray<uint8>& Buffer);
		
		/**
		 * Stream data upload supported by UE4 .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param Stream				UE4 storage structure .
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromStream(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);

		/**
		 * Remove a single object from the server .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds
		 */
		bool DeleteObject(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL);
		
		/**
		 * Multiple URLs need to be specified to remove multiple objects from the server .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					Need domain name .
		 */
		void DeleteObjects(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL);
		
		//////////////////////////////////////////////////////////////////////////

		/**
		 * Submit form to server.
		 *
		 * @param InURL						Address to visit.
		 * @param InParam					Parameters passed.
		 * @param BPResponseDelegate		Proxy for site return.
		 */
		bool PostRequest(const TCHAR* InURL, const TCHAR* InParam, const FSmartHTTPResponseDelegate& BPResponseDelegate);

		/**
		 * The data can be downloaded to local memory via the HTTP serverll .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds
		 */
		bool GetObjectToMemory(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL);

		/**
		 * The data can be downloaded to local memory via the HTTP serverll.
		 * Can download multiple at one time .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					Need domain name .
		 */
		void GetObjectsToMemory(const FSmartHTTPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL);

		/**
		 * Download individual data locally.
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param SavePaths				Path to local storage .
		 * @Return						Returns true if the request succeeds
		 */
		bool GetObjectToLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& SavePaths);

		/**
		 * Download multiple data to local .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					Need domain name .
		 * @param SavePaths				Path to local storage .
		 */
		void GetObjectsToLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const TArray<FString>& , const FString& SavePaths);

		/**
		 * Upload single file from disk to server .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths);

		/**
		 * Upload duo files from disk to server  .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectsFromLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths);

		/**
		 * Can upload byte data .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param Buffer				Byte code data.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromBuffer(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const TArray<uint8>& Buffer);

		/**
		 * Stream data upload supported by UE4 .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param Stream				UE4 storage structure .
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromStream(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);

		/**
		 * Remove a single object from the server .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds
		 */
		bool DeleteObject(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL);

		/**
		 * Multiple URLs need to be specified to remove multiple objects from the server .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					Need domain name .
		 */
		void DeleteObjects(const FSmartHTTPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL);

	private:

		/**
		 * Register our agent BP for internal use .
		 *
		 * @param return	Use handle find PRequest.
		 */
		FSmartHTTPHandle RegisteredHttpRequest(EHTTPRequestType RequestType = EHTTPRequestType::SINGLE,
			FSmartHttpSingleRequestCompleteDelegate SmartHttpRequestCompleteDelegate = FSmartHttpSingleRequestCompleteDelegate(),
			FSmartHttpSingleRequestProgressDelegate	SmartHttpRequestProgressDelegate = FSmartHttpSingleRequestProgressDelegate(),
			FSmartHttpSingleRequestHeaderReceivedDelegate SmartHttpRequestHeaderReceivedDelegate = FSmartHttpSingleRequestHeaderReceivedDelegate(), 
			FAllRequestCompleteDelegate AllRequestCompleteDelegate = FAllRequestCompleteDelegate());

		/**
		 * Register our agent BP for internal use .
		 *
		 * @param return	Use handle find PRequest.
		 */
		FSmartHTTPHandle RegisteredHttpRequest(EHTTPRequestType RequestType = EHTTPRequestType::SINGLE,
			FSmartSingleCompleteDelegate SmartHttpRequestCompleteDelegate = nullptr,
			FSmartSingleRequestProgressDelegate	SmartHttpRequestProgressDelegate = nullptr,
			FSmartSingleRequestHeaderReceivedDelegate SmartHttpRequestHeaderReceivedDelegate = nullptr,
			FSimpleDelegate AllRequestCompleteDelegate = nullptr);

		/** 
		 * Refer to the previous API for internal use details only 
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool GetObjectToMemory(const FSmartHTTPHandle& Handle, const FString& URL);
		
		/** 
		 * Refer to the previous API for internal use details only 
		 *
		 * @param Handle	Easy to find requests .
		 */
		void GetObjectsToMemory(const FSmartHTTPHandle& Handle, const TArray<FString>& URL);
	
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool GetObjectToLocal(const FSmartHTTPHandle& Handle, const FString& URL, const FString& SavePaths);

		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		void GetObjectsToLocal(const FSmartHTTPHandle& Handle, const TArray<FString>& URL, const FString& SavePaths);
	
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectFromLocal(const FSmartHTTPHandle& Handle, const FString& URL, const FString& LocalPaths);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectsFromLocal(const FSmartHTTPHandle& Handle, const FString& URL, const FString& LocalPaths);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectFromBuffer(const FSmartHTTPHandle& , const FString& URL, const TArray<uint8>& Buffer);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectFromStream(const FSmartHTTPHandle& Handle, const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
	
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool DeleteObject(const FSmartHTTPHandle& Handle, const FString& URL);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		void DeleteObjects(const FSmartHTTPHandle& Handle, const TArray<FString>& URL);

		/**
		 * Submit form to server.
		 *
		 * @param Handle					It can be used to retrieve the corresponding request.
		 * @param InURL						Address to visit.
		 */
		bool PostRequest(const FSmartHTTPHandle& Handle, const FString& URL);

	private:
		/*You can find the corresponding request according to the handle  */
		TWeakPtr<FSmartHttpActionRequest> Find(const FSmartHTTPHandle& Handle);

		/*HTTP Map*/
		TMap<FSmartHTTPHandle, TSharedPtr<FSmartHttpActionRequest>> HTTPMap;
	};

public:
	virtual ~FSmartHttpManage(){}

	/**
	 * GC
	 * Pure virtual that must be overloaded by the inheriting class. It will
	 * be called from within LevelTick.cpp after ticking all actors or from
	 * the rendering thread (depending on bIsRenderingThreadObject)
	 *
	 * @param DeltaTime	Game time passed since the last call.
	 */
	virtual void Tick(float DeltaTime);

	static FSmartHttpManage* Get();
	static void Destroy();

	/** Get HTTP function collection  **/
	FORCEINLINE FHTTP& GetHTTP() { return HTTP; }
private:

	static FSmartHttpManage *Instance;
	FHTTP HTTP;
	FCriticalSection Mutex;//https://blog.ch-wind.com/ue4-async-note/
};

#define SMART_HTTP FSmartHttpManage::Get()->GetHTTP()
