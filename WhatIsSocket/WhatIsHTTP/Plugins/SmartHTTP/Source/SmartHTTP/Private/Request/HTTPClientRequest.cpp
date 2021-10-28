#include "Request/HTTPClientRequest.h"
#include "Core/SmartHttpMacro.h"
#include "SmartHTTPLog.h"

SmartHTTP::HTTP::FPutObjectRequest::FPutObjectRequest(const FString& URL, const FString& ContentString)
{
	DEFINITION_HTTP_TYPE(PUT, "application/x-www-form-urlencoded;charset=utf-8")
	HttpReuest->SetContentAsString(ContentString);

	UE_LOG(LogSmartHTTP, Log, TEXT("Put Action as string"));
}

SmartHTTP::HTTP::FPutObjectRequest::FPutObjectRequest(const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	DEFINITION_HTTP_TYPE(PUT, "application/x-www-form-urlencoded;charset=utf-8")
	HttpReuest->SetContentFromStream(Stream);

	UE_LOG(LogSmartHTTP, Log, TEXT("Put Action by stream."));
}

SmartHTTP::HTTP::FPutObjectRequest::FPutObjectRequest(const FString& URL,const TArray<uint8>& ContentPayload)
{
	DEFINITION_HTTP_TYPE(PUT, "application/x-www-form-urlencoded;charset=utf-8")
	HttpReuest->SetContent(ContentPayload);

	UE_LOG(LogSmartHTTP, Log, TEXT("Put Action by content."));
}

//Get: get resources from the server
//Post: create a new resource on the server.
//Put: update resources on the server and provide the changed complete resources on the client
//Patch: update resources on the server and provide changed properties on the client
//Delete: delete a resource from the server.
//Head: get the metadata of the resource.
//Options: get information about which attributes of the resource can be changed by the client.

SmartHTTP::HTTP::FGetObjectRequest::FGetObjectRequest(const FString& URL)
{
	DEFINITION_HTTP_TYPE(GET, "application/x-www-form-urlencoded;charset=utf-8")

	UE_LOG(LogSmartHTTP, Log, TEXT("GET Action."));
}

SmartHTTP::HTTP::FDeleteObjectsRequest::FDeleteObjectsRequest(const FString& URL)
{
	DEFINITION_HTTP_TYPE(DELETE, "application/x-www-form-urlencoded;charset=utf-8")

	UE_LOG(LogSmartHTTP, Log, TEXT("DELETE Action."));
}

SmartHTTP::HTTP::FPostObjectsRequest::FPostObjectsRequest(const FString& URL)
{
	DEFINITION_HTTP_TYPE(POST, "application/x-www-form-urlencoded;charset=utf-8")

	UE_LOG(LogSmartHTTP, Log, TEXT("POST Action."));
}