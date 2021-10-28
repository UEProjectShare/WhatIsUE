#include "SmartHTTPManage.h"
#include "HTTP/SmartHttpActionMultpleRequest.h"
#include "Core/SmartHttpMacro.h"
#include "Misc/FileHelper.h"
#include "SmartHTTPLog.h"
#include "HttpModule.h"
#include "HttpManager.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FSmartHttpManage* FSmartHttpManage::Instance = nullptr;

TSharedPtr<FSmartHttpActionRequest> GetHttpActionRequest(EHTTPRequestType RequestType)
{
	TSharedPtr<FSmartHttpActionRequest> HttpObject = nullptr;
	
	switch (RequestType)
	{
		case EHTTPRequestType::SINGLE:
		{
			HttpObject = MakeShareable(new FSmartHttpActionRequest());
			UE_LOG(LogSmartHTTP, Log, TEXT("Action to create a single HTTP request"));
			break;
		}
		case EHTTPRequestType::MULTPLE:
		{
			HttpObject = MakeShareable(new FSmartHttpActionMultpleRequest());
			UE_LOG(LogSmartHTTP, Log, TEXT("Action to create a multple HTTP request"));
			break;
		}
	}

	return HttpObject;
}

void FSmartHttpManage::Tick(float DeltaTime)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	FHttpModule::Get().GetHttpManager().Tick(DeltaTime);

	TArray<FString> RemoveRequest;
	for (auto &Tmp : HTTP.HTTPMap)
	{
		if (Tmp.Value->IsRequestComplete())
		{
			RemoveRequest.Add(Tmp.Key);
		}
	}

	for (auto &Tmp : RemoveRequest)
	{
		GetHTTP().HTTPMap.Remove(Tmp);

		UE_LOG(LogSmartHTTP, Log, TEXT("Remove request %s from tick"), *Tmp);
	}
}

bool FSmartHttpManage::IsTickableInEditor() const
{
	return true;
}

TStatId FSmartHttpManage::GetStatId() const
{
	return TStatId();
}

FSmartHttpManage * FSmartHttpManage::Get()
{
	if (Instance == nullptr)
	{
		Instance = new FSmartHttpManage();

		UE_LOG(LogSmartHTTP, Log, TEXT("Get HTTP management"));
	}

	return Instance;
}

void FSmartHttpManage::Destroy()
{
	if (Instance != nullptr)
	{
		FScopeLock ScopeLock(&Instance->Mutex);
		delete Instance;		

		UE_LOG(LogSmartHTTP, Log, TEXT("delete HTTP management"));
	}

	Instance = nullptr;
}

FSmartHTTPHandle FSmartHttpManage::FHTTP::RegisteredHttpRequest(
	EHTTPRequestType RequestType ,
	FSmartHttpSingleRequestCompleteDelegate SmartHttpRequestCompleteDelegate /*= FSmartHttpRequestCompleteDelegate()*/,
	FSmartHttpSingleRequestProgressDelegate SmartHttpRequestProgressDelegate /*= FSmartHttpRequestProgressDelegate()*/, 
	FSmartHttpSingleRequestHeaderReceivedDelegate SmartHttpRequestHeaderReceivedDelegate /*= FSmartHttpRequestHeaderReceivedDelegate()*/,
	FAllRequestCompleteDelegate AllRequestCompleteDelegate /*= FAllRequestCompleteDelegate()*/)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	UE_LOG(LogSmartHTTP, Log, TEXT("Start registering single BP agent."));

	TSharedPtr<FSmartHttpActionRequest> HttpObject = GetHttpActionRequest(RequestType);
	
	HttpObject->SmartHttpRequestCompleteDelegate = SmartHttpRequestCompleteDelegate;
	HttpObject->SmartHttpRequestHeaderReceivedDelegate = SmartHttpRequestHeaderReceivedDelegate;
	HttpObject->SmartHttpRequestProgressDelegate = SmartHttpRequestProgressDelegate;
	HttpObject->AllRequestCompleteDelegate = AllRequestCompleteDelegate;

	FSmartHTTPHandle Key = FGuid::NewGuid().ToString();
	HTTPMap.Add(Key, HttpObject);

	return Key;
}

FSmartHTTPHandle FSmartHttpManage::FHTTP::RegisteredHttpRequest(
	EHTTPRequestType RequestType /*= EHTTPRequestType::SINGLE*/, 
	FSmartSingleCompleteDelegate SmartHttpRequestCompleteDelegate /*= nullptr*/, 
	FSmartSingleRequestProgressDelegate SmartHttpRequestProgressDelegate /*= nullptr*/,
	FSmartSingleRequestHeaderReceivedDelegate SmartHttpRequestHeaderReceivedDelegate /*= nullptr*/,
	FSimpleDelegate AllRequestCompleteDelegate /*= nullptr*/)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	UE_LOG(LogSmartHTTP, Log, TEXT("Start registering single C++ agent."));

	TSharedPtr<FSmartHttpActionRequest> HttpObject = GetHttpActionRequest(RequestType);

	HttpObject->SmartCompleteDelegate = SmartHttpRequestCompleteDelegate;
	HttpObject->SmartSingleRequestHeaderReceivedDelegate = SmartHttpRequestHeaderReceivedDelegate;
	HttpObject->SmartSingleRequestProgressDelegate = SmartHttpRequestProgressDelegate;
	HttpObject->AllTasksCompletedDelegate = AllRequestCompleteDelegate;

	FSmartHTTPHandle Key = FGuid::NewGuid().ToString();
	HTTPMap.Add(Key, HttpObject);

	return Key;
}

TWeakPtr<FSmartHttpActionRequest> FSmartHttpManage::FHTTP::Find(const FSmartHTTPHandle& Handle)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	TWeakPtr<FSmartHttpActionRequest> Object = nullptr;
	for (auto& Tmp : Instance->GetHTTP().HTTPMap)
	{
		if (Tmp.Key == Handle)
		{
			Object = Tmp.Value;

			UE_LOG(LogSmartHTTP, Log, TEXT("Http Find at %s"),*Tmp.Key);
			break;
		}
	}

	return Object;
}

bool FSmartHttpManage::FHTTP::GetObjectToMemory(const FSmartHTTPHandle& Handle, const FString& URL)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->GetObject(URL);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}

	return false;
}

bool FSmartHttpManage::FHTTP::GetObjectToMemory(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return GetObjectToMemory(Handle, URL);
}

bool FSmartHttpManage::FHTTP::GetObjectToMemory(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return GetObjectToMemory(Handle, URL);
}

void FSmartHttpManage::FHTTP::GetObjectsToMemory(const FSmartHTTPHandle& Handle, const TArray<FString>& URL)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		Object.Pin()->GetObjects(URL);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}
}

void FSmartHttpManage::FHTTP::GetObjectsToMemory(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	GetObjectsToMemory(Handle, URL);
}

void FSmartHttpManage::FHTTP::GetObjectsToMemory(const FSmartHTTPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	GetObjectsToMemory(Handle, URL);
}

bool FSmartHttpManage::FHTTP::GetObjectToLocal(const FSmartHTTPHandle& Handle, const FString& URL, const FString& SavePaths)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->GetObject(URL, SavePaths);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}

	return false;
}

bool FSmartHttpManage::FHTTP::GetObjectToLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& SavePaths)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return GetObjectToLocal(Handle, URL, SavePaths);
}

bool FSmartHttpManage::FHTTP::GetObjectToLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& SavePaths)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return GetObjectToLocal(Handle, URL, SavePaths);
}

void FSmartHttpManage::FHTTP::GetObjectsToLocal(const FSmartHTTPHandle& Handle, const TArray<FString>& URL, const FString& SavePaths)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		Object.Pin()->GetObjects(URL, SavePaths);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}
}

void FSmartHttpManage::FHTTP::GetObjectsToLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL, const FString& SavePaths)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	GetObjectsToLocal(Handle, URL, SavePaths);
}

void FSmartHttpManage::FHTTP::GetObjectsToLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL, const FString& SavePaths)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	GetObjectsToLocal(Handle, URL, SavePaths);
}

bool FSmartHttpManage::FHTTP::PutObjectFromBuffer(const FSmartHTTPHandle& Handle, const FString& URL, const TArray<uint8>& Data)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->PutObject(URL, Data);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}

	return false;
}

bool FSmartHttpManage::FHTTP::PutObjectFromBuffer(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const TArray<uint8>& Buffer)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return PutObjectFromBuffer(Handle, URL, Buffer);
}

bool FSmartHttpManage::FHTTP::PutObjectFromBuffer(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const TArray<uint8>& Buffer)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return PutObjectFromBuffer(Handle, URL, Buffer);
}

bool FSmartHttpManage::FHTTP::PutObjectFromStream(const FSmartHTTPHandle& Handle, const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->PutObject(URL, Stream);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}

	return false;
}

bool FSmartHttpManage::FHTTP::PutObjectFromStream(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return PutObjectFromStream(Handle, URL, Stream);
}

bool FSmartHttpManage::FHTTP::PutObjectFromStream(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return PutObjectFromStream(Handle, URL, Stream);
}

bool FSmartHttpManage::FHTTP::PutObjectFromLocal(const FSmartHTTPHandle& Handle, const FString& URL, const FString& LocalPaths)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->PutObject(URL, LocalPaths);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}

	return false;
}

bool FSmartHttpManage::FHTTP::PutObjectFromLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return PutObjectFromLocal(Handle, URL, LocalPaths);
}

bool FSmartHttpManage::FHTTP::PutObjectFromLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return GetObjectToLocal(Handle, URL, LocalPaths);
}

bool FSmartHttpManage::FHTTP::PutObjectsFromLocal(const FSmartHTTPHandle& Handle, const FString& URL, const FString& LocalPaths)
{
	return PutObjectFromLocal(Handle, URL, LocalPaths);
}

bool FSmartHttpManage::FHTTP::PutObjectsFromLocal(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	return PutObjectsFromLocal(Handle, URL, LocalPaths);
}

bool FSmartHttpManage::FHTTP::PutObjectsFromLocal(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL, const FString& LocalPaths)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	return PutObjectsFromLocal(Handle, URL, LocalPaths);
}

bool FSmartHttpManage::FHTTP::DeleteObject(const FSmartHTTPHandle& Handle, const FString& URL)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->DeleteObject(URL);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}

	return false;
}

bool FSmartHttpManage::FHTTP::DeleteObject(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const FString& URL)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return DeleteObject(Handle, URL);
}

bool FSmartHttpManage::FHTTP::DeleteObject(const FSmartHTTPResponseDelegate& BPResponseDelegate, const FString& URL)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return DeleteObject(Handle, URL);
}

void FSmartHttpManage::FHTTP::DeleteObjects(const FSmartHTTPHandle& Handle, const TArray<FString>& URL)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		Object.Pin()->DeleteObjects(URL);
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
	}
}

void FSmartHttpManage::FHTTP::DeleteObjects(const FSmartHTTPBPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	DeleteObjects(Handle, URL);
}

void FSmartHttpManage::FHTTP::DeleteObjects(const FSmartHTTPResponseDelegate& BPResponseDelegate, const TArray<FString>& URL)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	DeleteObjects(Handle, URL);
}

bool FSmartHttpManage::FHTTP::PostRequest(const TCHAR* InURL, const TCHAR* InParam, const FSmartHTTPBPResponseDelegate& BPResponseDelegate)
{
	SMART_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);
	
	return PostRequest(Handle,InURL + FString(TEXT("?")) + InParam);
}

bool FSmartHttpManage::FHTTP::PostRequest(const FSmartHTTPHandle& Handle, const FString& URL)
{
	TWeakPtr<FSmartHttpActionRequest> Object = Find(Handle);

	if (Object.IsValid())
	{
		Object.Pin()->PostObject(URL);

		return true;
	}
	else
	{
		UE_LOG(LogSmartHTTP, Warning, TEXT("The handle was not found [%s]"), *Handle);
		return false;
	}
}

bool FSmartHttpManage::FHTTP::PostRequest(const TCHAR* InURL, const TCHAR* InParam, const FSmartHTTPResponseDelegate& BPResponseDelegate)
{
	SMART_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return PostRequest(Handle, InURL + FString(TEXT("?")) + InParam);
}

TArray<uint8>& USmartHttpContent::GetContent()
{
	return *Content;
}

bool USmartHttpContent::Save(const FString& LocalPath)
{
	return FFileHelper::SaveArrayToFile(*Content, *LocalPath);
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif