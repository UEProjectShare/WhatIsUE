#include "HTTP/SmartHttpActionRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SmartHttpMacro.h"
#include "HAL/FileManager.h"
#include "SmartHTTPLog.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"
//#include "GenericPlatform/GenericPlatformHttp.h"

FSmartHttpActionRequest::FSmartHttpActionRequest()
	: bRequestComplete(false)
	, bSaveDisk(true)
{
	
}

FSmartHttpActionRequest::~FSmartHttpActionRequest()
{

}

void FSmartHttpActionRequest::GetObjects(const TArray<FString>& URL, const FString& SavePaths)
{

}

void FSmartHttpActionRequest::GetObjects(const TArray<FString>& URL)
{

}

void FSmartHttpActionRequest::DeleteObjects(const TArray<FString>& URL)
{

}

void FSmartHttpActionRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	auto Call = [&]()
	{
		FSmartHttpRequest SmartHttpRequest;
		FSmartHttpResponse SmartHttpResponse;
		ResponsePtrToSmartResponse(Response, SmartHttpResponse);
		RequestPtrToSmartRequest(Request, SmartHttpRequest);

		SmartHttpRequestCompleteDelegate.ExecuteIfBound(SmartHttpRequest, SmartHttpResponse, bConnectedSuccessfully);
		SmartCompleteDelegate.ExecuteIfBound(SmartHttpRequest, SmartHttpResponse, bConnectedSuccessfully);

		UE_LOG(LogSmartHTTP, Warning, TEXT("Request completion failed."));
	};

	//404 405 100 -199 200 -299
	if (!Request.IsValid())
	{
		//switch (Request.GetStatus())
		//{
		//case EHttpRequestStatus::NotStarted:
		//case EHttpRequestStatus::Processing:
		//case EHttpRequestStatus::Failed:
		//case EHttpRequestStatus::Failed_ConnectionError:
		//case EHttpRequestStatus::Succeeded:
		//}

		UE_LOG(LogSmartHTTP, Warning, TEXT("Server request failed."));

		Call();
	}
	else if(!Response.IsValid())
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT(" Response failed."));

		Call();
	}
	else if (!bConnectedSuccessfully)
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("HTTP connected failed."));

		Call();
	}
	else if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("HTTP error code [%d]."), Response->GetResponseCode());

		Call();
	}
	else
	{
		if (Request->GetVerb() == "GET")
		{
			if (bSaveDisk)
			{
				FString Filename = FPaths::GetCleanFilename(Request->GetURL());
				FFileHelper::SaveArrayToFile(Response->GetContent(), *(TmpSavePaths / Filename));

				UE_LOG(LogSmartHTTP, Log, TEXT("Store the obtained http file locally."));
				UE_LOG(LogSmartHTTP, Log, TEXT("%s."), *Filename);
			}
			else
			{
				UE_LOG(LogSmartHTTP, Log, TEXT("This is a get request that is not stored locally."));
			}
		}

		FSmartHttpRequest SmartHttpRequest;
		FSmartHttpResponse SmartHttpResponse;
		ResponsePtrToSmartResponse(Response, SmartHttpResponse);
		RequestPtrToSmartRequest(Request, SmartHttpRequest);

		SmartHttpRequestCompleteDelegate.ExecuteIfBound(SmartHttpRequest, SmartHttpResponse, bConnectedSuccessfully);
		SmartCompleteDelegate.ExecuteIfBound(SmartHttpRequest, SmartHttpResponse, bConnectedSuccessfully);

		UE_LOG(LogSmartHTTP, Log, TEXT("Request to complete execution of binding agent."));
	}

	AllRequestCompleteDelegate.ExecuteIfBound();
	AllTasksCompletedDelegate.ExecuteIfBound();

	bRequestComplete = true;
}

void FSmartHttpActionRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSmartHttpRequest SmartHttpRequest;
	RequestPtrToSmartRequest(Request, SmartHttpRequest);

	SmartHttpRequestProgressDelegate.ExecuteIfBound(SmartHttpRequest, BytesSent, BytesReceived);
	SmartSingleRequestProgressDelegate.ExecuteIfBound(SmartHttpRequest, BytesSent, BytesReceived);

//	UE_LOG(LogSmartHTTP, Log, TEXT("Http request progress."));
}

void FSmartHttpActionRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSmartHttpRequest SmartHttpRequest;
	RequestPtrToSmartRequest(Request, SmartHttpRequest);

	SmartHttpRequestHeaderReceivedDelegate.ExecuteIfBound(SmartHttpRequest, HeaderName, NewHeaderValue);
	SmartSingleRequestHeaderReceivedDelegate.ExecuteIfBound(SmartHttpRequest, HeaderName, NewHeaderValue);

//	UE_LOG(LogSmartHTTP, Log, TEXT("Http request header received."));
}

void FSmartHttpActionRequest::Print(const FString& Msg, float Time /*= 10.f*/, FColor Color /*= FColor::Red*/)
{
#ifdef PLATFORM_PROJECT
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
	}
#endif
}

bool FSmartHttpActionRequest::GetObject(const FString& URL, const FString& SavePaths)
{
	TmpSavePaths = SavePaths;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN(FSmartHttpActionRequest)

	return Client.GetObject(Request);
}

bool FSmartHttpActionRequest::GetObject(const FString& URL)
{
	bSaveDisk = false;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN(FSmartHttpActionRequest)

	return Client.GetObject(Request);
}

bool FSmartHttpActionRequest::PutObject(const FString& URL,const TArray<uint8>& Data)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Data);

	REQUEST_BIND_FUN(FSmartHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSmartHttpActionRequest::PutObject(const FString& URL, const FString& LocalPaths)
{
	TArray<uint8> Data;
	if (FFileHelper::LoadFileToArray(Data, *LocalPaths))
	{
		FHTTPClient Client;
		FPutObjectRequest Request(URL, Data);

		REQUEST_BIND_FUN(FSmartHttpActionRequest)

		return Client.PutObject(Request);
	}

	return false;
}

bool FSmartHttpActionRequest::PutObject(const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Stream);

	REQUEST_BIND_FUN(FSmartHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSmartHttpActionRequest::DeleteObject(const FString& URL)
{
	FHTTPClient Client;

	FDeleteObjectsRequest Request(URL);

	return Client.DeleteObject(Request);
}

bool FSmartHttpActionRequest::PostObject(const FString& URL)
{
	//FString NewURL = FGenericPlatformHttp::UrlEncode(URL);

	bSaveDisk = false;

	FHTTPClient Client;

	FPostObjectsRequest Request(URL);

	REQUEST_BIND_FUN(FSmartHttpActionRequest)

	return Client.PostObject(Request);
}