#pragma once

#define DEFINITION_HTTP_TYPE(VerbString,Content) \
HttpReuest->SetURL(URL);\
HttpReuest->SetVerb(TEXT(#VerbString));\
HttpReuest->SetHeader(TEXT("Content-Type"), TEXT(#Content)); 

#define REQUEST_BIND_FUN(RequestClass) \
Request \
<< FHttpRequestHeaderReceivedDelegate::CreateRaw(this, &RequestClass::HttpRequestHeaderReceived)\
<< FHttpRequestProgressDelegate::CreateRaw(this, &RequestClass::HttpRequestProgress)\
<< FHttpRequestCompleteDelegate::CreateRaw(this, &RequestClass::HttpRequestComplete);

#define SMART_HTTP_REGISTERED_REQUEST_BP(TYPE) \
auto Handle = RegisteredHttpRequest(TYPE, \
	BPResponseDelegate.SmartHttpRequestCompleteDelegate, \
	BPResponseDelegate.SmartHttpRequestProgressDelegate, \
	BPResponseDelegate.SmartHttpRequestHeaderReceivedDelegate, \
	BPResponseDelegate.AllRequestCompleteDelegate)

#define SMART_HTTP_REGISTERED_REQUEST(TYPE) \
auto Handle = RegisteredHttpRequest(TYPE, \
	BPResponseDelegate.SmartCompleteDelegate, \
	BPResponseDelegate.SmartSingleRequestProgressDelegate, \
	BPResponseDelegate.SmartSingleRequestHeaderReceivedDelegate, \
	BPResponseDelegate.AllTasksCompletedDelegate)

void RequestPtrToSmartRequest(FHttpRequestPtr Request, FSmartHttpRequest &SmartHttpRequest)
{
	if (Request.IsValid())
	{
		SmartHttpRequest.Verb = Request->GetVerb();
		SmartHttpRequest.URL = Request->GetURL();
		SmartHttpRequest.Status = (FSmartHttpStatus)Request->GetStatus();
		SmartHttpRequest.ElapsedTime = Request->GetElapsedTime();
		SmartHttpRequest.ContentType = Request->GetContentType();
		SmartHttpRequest.ContentLength = Request->GetContentLength();
		SmartHttpRequest.AllHeaders = Request->GetAllHeaders();
	}
}

void ResponsePtrToSmartResponse(FHttpResponsePtr Response, FSmartHttpResponse &SmartHttpResponse)
{
	if (Response.IsValid())
	{
		SmartHttpResponse.ResponseCode = Response->GetResponseCode();
		SmartHttpResponse.URL = Response->GetURL();
		SmartHttpResponse.ResponseMessage = Response->GetContentAsString();
		SmartHttpResponse.ContentType = Response->GetContentType();
		SmartHttpResponse.ContentLength = Response->GetContentLength();
		SmartHttpResponse.AllHeaders = Response->GetAllHeaders();
		SmartHttpResponse.Content->Content = const_cast<TArray<uint8>*>(&Response->GetContent());
	}
}