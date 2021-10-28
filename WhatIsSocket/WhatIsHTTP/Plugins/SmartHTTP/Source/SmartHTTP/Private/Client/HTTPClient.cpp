#include "Client/HTTPClient.h"

SmartHTTP::HTTP::FHTTPClient::FHTTPClient()
{

}

bool SmartHTTP::HTTP::FHTTPClient::GetObject(FGetObjectRequest &Request) const
{
	return Request.ProcessRequest();
}

bool SmartHTTP::HTTP::FHTTPClient::DeleteObject(FDeleteObjectsRequest &Request) const
{
	return Request.ProcessRequest();
}

bool SmartHTTP::HTTP::FHTTPClient::PutObject(FPutObjectRequest &Request) const
{
	return Request.ProcessRequest();
}

void SmartHTTP::HTTP::FHTTPClient::AbortRequest(IHTTPClientRequest &Request)
{
	Request.CancelRequest();
}

bool SmartHTTP::HTTP::FHTTPClient::PostObject(FPostObjectsRequest &Request) const
{
	return Request.ProcessRequest();
}