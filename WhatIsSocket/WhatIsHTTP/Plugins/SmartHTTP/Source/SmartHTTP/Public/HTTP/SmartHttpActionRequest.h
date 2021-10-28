#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "SmartHTTPType.h"
/**
 * 
 */
struct SMARTHTTP_API FSmartHttpActionRequest : public TSharedFromThis<FSmartHttpActionRequest>
{
public:
	//BP
	FSmartHttpSingleRequestCompleteDelegate				SmartHttpRequestCompleteDelegate;
	FSmartHttpSingleRequestProgressDelegate				SmartHttpRequestProgressDelegate;
	FSmartHttpSingleRequestHeaderReceivedDelegate		SmartHttpRequestHeaderReceivedDelegate;
	FAllRequestCompleteDelegate							AllRequestCompleteDelegate;

	//C++
	FSmartSingleCompleteDelegate						SmartCompleteDelegate;
	FSmartSingleRequestProgressDelegate					SmartSingleRequestProgressDelegate;
	FSmartSingleRequestHeaderReceivedDelegate			SmartSingleRequestHeaderReceivedDelegate;
	FSimpleDelegate										AllTasksCompletedDelegate;

public:
	FSmartHttpActionRequest();
	virtual ~FSmartHttpActionRequest();

	//Compatibility blueprint
	virtual void GetObjects(const TArray<FString>& URL, const FString& SavePaths);
	virtual void GetObjects(const TArray<FString>& URL);
	virtual void DeleteObjects(const TArray<FString>& URL);

	virtual bool GetObject(const FString& URL);
	virtual bool GetObject(const FString& URL, const FString& SavePaths);
	virtual bool PutObject(const FString& URL, const TArray<uint8>& Data);
	virtual bool PutObject(const FString& URL, const FString& LocalPaths);
	virtual bool PutObject(const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
	virtual bool DeleteObject(const FString& URL);
	virtual bool PostObject(const FString& URL);

	FORCEINLINE FString GetPaths() { return TmpSavePaths; }
	FORCEINLINE void SetPaths(const FString& NewPaths) { TmpSavePaths = NewPaths; }
	FORCEINLINE bool IsRequestComplete() { return bRequestComplete; }

protected:
	virtual void HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	virtual void HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
	virtual void HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

	void Print(const FString& Msg, float Time = 10.f, FColor Color = FColor::Red);
protected:
	FString						TmpSavePaths;
	bool						bRequestComplete;
	bool						bSaveDisk;
};
