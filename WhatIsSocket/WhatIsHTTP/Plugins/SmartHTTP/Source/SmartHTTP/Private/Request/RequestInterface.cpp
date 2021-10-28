#include "Request/RequestInterface.h"
#include "HttpModule.h"
#include "SmartHTTPLog.h"

SmartHTTP::HTTP::IHTTPClientRequest::IHTTPClientRequest()
	: HttpReuest(FHttpModule::Get().CreateRequest())
{

}

bool SmartHTTP::HTTP::IHTTPClientRequest::ProcessRequest()
{
	UE_LOG(LogSmartHTTP, Log, TEXT("Process Request."));

	return HttpReuest->ProcessRequest();
}

void SmartHTTP::HTTP::IHTTPClientRequest::CancelRequest()
{
	UE_LOG(LogSmartHTTP, Log, TEXT("Cancel Request."));

	HttpReuest->CancelRequest();
}

