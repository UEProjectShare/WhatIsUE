#include "HTTP/SmartHttpActionMultpleRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SmartHttpMacro.h"
#include "HAL/FileManager.h"
#include "SmartHTTPLog.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"

FSmartHttpActionMultpleRequest::FSmartHttpActionMultpleRequest()
	: FSmartHttpActionRequest()
	, RequestNumber(0)
{

}

void FSmartHttpActionMultpleRequest::GetObjects(const TArray<FString>& URL, const FString& SavePaths)
{
	SetPaths(SavePaths);

	for (const auto& Tmp : URL)
	{
		FHTTPClient Client;

		FGetObjectRequest Request(Tmp);

		REQUEST_BIND_FUN(FSmartHttpActionMultpleRequest)

		if (Client.GetObject(Request))
		{
			RequestNumber++;

			UE_LOG(LogSmartHTTP, Log, TEXT("Multple get objects request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSmartHTTP, Error, TEXT("Multple get objects request execution failed."));
		}
	}
}

void FSmartHttpActionMultpleRequest::GetObjects(const TArray<FString>& URL)
{
	bSaveDisk = false;

	for (const auto& Tmp : URL)
	{
		FHTTPClient Client;

		FGetObjectRequest Request(Tmp);

		REQUEST_BIND_FUN(FSmartHttpActionMultpleRequest)

		if (Client.GetObject(Request))
		{
			RequestNumber++;
			UE_LOG(LogSmartHTTP, Log, TEXT("Multple get objects request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSmartHTTP, Error, TEXT("Multple get objects request execution failed."));
		}
	}
}

void FSmartHttpActionMultpleRequest::DeleteObjects(const TArray<FString>& URL)
{
	for (const auto& Tmp : URL)
	{
		FHTTPClient Client;

		FDeleteObjectsRequest Request(Tmp);

		REQUEST_BIND_FUN(FSmartHttpActionMultpleRequest)

		if (Client.DeleteObject(Request))
		{
			RequestNumber++;

			UE_LOG(LogSmartHTTP, Log, TEXT("Multple delete request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSmartHTTP, Error, TEXT("Fail to delete object."));
		}
	}
}

bool FSmartHttpActionMultpleRequest::PutObject(const FString& URL, const FString& LocalPaths)
{
	if (LocalPaths.Len())
	{
		UE_LOG(LogSmartHTTP, Log, TEXT("Set path %s."), *LocalPaths);
	}
	
	SetPaths(LocalPaths);

	//Start filtering path
	TArray<FString> AllPaths;
	IFileManager::Get().FindFilesRecursive(AllPaths, *LocalPaths, TEXT("*"), true, true);

	for (const auto& Tmp: AllPaths)
	{
		TArray<uint8> ByteData;
		FFileHelper::LoadFileToArray(ByteData, *Tmp);

		FString ObjectName = FPaths::GetCleanFilename(Tmp);

		FHTTPClient Client;

		FPutObjectRequest Request(URL / ObjectName, ByteData);

		REQUEST_BIND_FUN(FSmartHttpActionMultpleRequest)

		if (Client.PutObject(Request))
		{
			RequestNumber++;

			UE_LOG(LogSmartHTTP, Log, TEXT("Multple put object request number %d by multple request."), RequestNumber);
		}
		else
		{
			UE_LOG(LogSmartHTTP, Error, TEXT("Fail to put object."));
		}
	}

	return RequestNumber > 0;
}

void FSmartHttpActionMultpleRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
	else if (!Response.IsValid())
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
				FFileHelper::SaveArrayToFile(Response->GetContent(), *(GetPaths() / Filename));

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

	if (RequestNumber > 0)
	{
		RequestNumber--;
		if (RequestNumber <= 0)
		{
			AllRequestCompleteDelegate.ExecuteIfBound();
			AllTasksCompletedDelegate.ExecuteIfBound();

			bRequestComplete = true;

			UE_LOG(LogSmartHTTP, Log, TEXT("The task has been completed."));
		}
	}
	else
	{
		UE_LOG(LogSmartHTTP, Error, TEXT("Request quantity error."));
		//check(0);
	}
}

void FSmartHttpActionMultpleRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSmartHttpActionRequest::HttpRequestProgress(Request, BytesSent, BytesReceived);
}

void FSmartHttpActionMultpleRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSmartHttpActionRequest::HttpRequestHeaderReceived(Request, HeaderName, NewHeaderValue);
}

