#include "SmartHTTPFunctionLibrary.h"
#include "SmartHttpManage.h"
#include "HTTP/SmartHttpActionRequest.h"
#include "HTTP/SmartHTTPHandle.h"

bool USmartHTTPFunctionLibrary::PostRequest(const FString& InURL, const FString& InParam, const FSmartHTTPBPResponseDelegate& BPResponseDelegate )
{
	return SMART_HTTP.PostRequest(*InURL, *InParam, BPResponseDelegate);
}

void USmartHTTPFunctionLibrary::Tick(float DeltaTime)
{
	FSmartHttpManage::Get()->Tick(DeltaTime);
}

bool USmartHTTPFunctionLibrary::GetObjectToMemory(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	return SMART_HTTP.GetObjectToMemory(BPResponseDelegate, URL);
}

bool USmartHTTPFunctionLibrary::GetObjectToLocal(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths)
{
	return SMART_HTTP.GetObjectToLocal(BPResponseDelegate, URL, SavePaths);
}

bool USmartHTTPFunctionLibrary::PutObjectFromLocal(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	return SMART_HTTP.PutObjectFromLocal(BPResponseDelegate, URL, LocalPaths);
}

bool USmartHTTPFunctionLibrary::PutObjectFromBuffer(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer)
{
	return SMART_HTTP.PutObjectFromBuffer(BPResponseDelegate, URL, Buffer);
}

bool USmartHTTPFunctionLibrary::DeleteObject(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	return SMART_HTTP.DeleteObject(BPResponseDelegate, URL);
}

bool USmartHTTPFunctionLibrary::PutObjectsFromLocal(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	return SMART_HTTP.PutObjectsFromLocal(BPResponseDelegate, URL,LocalPaths);
}

void USmartHTTPFunctionLibrary::GetObjectsToLocal(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths)
{
	SMART_HTTP.GetObjectsToLocal(BPResponseDelegate, URL, SavePaths);
}

void USmartHTTPFunctionLibrary::GetObjectsToMemory(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SMART_HTTP.GetObjectsToMemory(BPResponseDelegate, URL);
}

void USmartHTTPFunctionLibrary::DeleteObjects(const FSmartHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SMART_HTTP.DeleteObjects(BPResponseDelegate, URL);
}
