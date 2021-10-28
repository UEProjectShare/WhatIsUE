#pragma once

#include "CoreMinimal.h"
#include "Request/HTTPClientRequest.h"

using namespace SmartHTTP::HTTP;

namespace SmartHTTP
{
	namespace HTTP
	{
		struct FHTTPClient
		{
			FHTTPClient();

			bool GetObject(FGetObjectRequest &Request) const;
			bool DeleteObject(FDeleteObjectsRequest &Request)const;
			bool PutObject(FPutObjectRequest &Request) const;
			bool PostObject(FPostObjectsRequest &Request) const;
			void AbortRequest(IHTTPClientRequest &Request);
		};
	}
}