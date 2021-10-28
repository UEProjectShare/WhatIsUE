#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

namespace SmartHTTP
{
	namespace HTTP
	{
		struct IHTTPClientRequest
		{
			friend struct FHTTPClient;

			IHTTPClientRequest();

			IHTTPClientRequest &operator<<(const FHttpRequestCompleteDelegate& SmartDelegateInstance)
			{
				HttpReuest->OnProcessRequestComplete() = SmartDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestProgressDelegate& SmartDelegateInstance)
			{			
				HttpReuest->OnRequestProgress() = SmartDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestHeaderReceivedDelegate& SmartDelegateInstance)
			{
				HttpReuest->OnHeaderReceived() = SmartDelegateInstance;
				return *this;
			}

		protected:
			bool ProcessRequest();
			void CancelRequest();
			
			TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> HttpReuest; //4.26
			//TSharedPtr<class IHttpRequest> HttpReuest;
		};
	}
}