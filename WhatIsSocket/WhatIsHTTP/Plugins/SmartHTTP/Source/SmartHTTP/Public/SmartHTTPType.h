#pragma once

#include "CoreMinimal.h"
#include "SmartHTTPType.generated.h"

UCLASS(BlueprintType)
class SMARTHTTP_API USmartHttpContent : public UObject
{
	GENERATED_BODY()

public:
	TArray<uint8>* Content;

	UFUNCTION(BlueprintPure, Category = "SmartHTTP|HttpContent")
	TArray<uint8>& GetContent();

	UFUNCTION(BlueprintCallable, Category = "SmartHTTP|HttpContent")
	bool Save(const FString& LocalPath);
};

enum class EHTTPRequestType
{
	SINGLE,
	MULTPLE,
};

UENUM(BlueprintType)
enum class FSmartHttpStatus : uint8
{
	NotStarted,//ProcessRequest()
	Processing,
	Failed,
	Failed_ConnectionError,
	Succeeded,
};

USTRUCT(BlueprintType)
struct SMARTHTTP_API FSmartHttpBase
{
	GENERATED_USTRUCT_BODY()

	FSmartHttpBase()
		: ContentLength(INDEX_NONE)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "SmartHttpBase|SmartHttpBase")
	FString URL;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpBase")
	TArray<FString> AllHeaders;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpBase")
	FString ContentType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpBase")
	int32 ContentLength;
};

USTRUCT(BlueprintType)
struct SMARTHTTP_API FSmartHttpRequest : public FSmartHttpBase
{
	GENERATED_USTRUCT_BODY()

	FSmartHttpRequest()
		: Super()
		, ElapsedTime(INDEX_NONE)
		, Status(FSmartHttpStatus::NotStarted)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpRequest")
	FString Verb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpRequest")
	float ElapsedTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpRequest")
	FSmartHttpStatus Status;
};

USTRUCT(BlueprintType)
struct SMARTHTTP_API FSmartHttpResponse : public FSmartHttpBase
{
	GENERATED_USTRUCT_BODY()

	FSmartHttpResponse()
		: Super()
		, ResponseCode(INDEX_NONE)
		, Content(NewObject<USmartHttpContent>())
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpResponse")
	int32 ResponseCode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpResponse")
	FString ResponseMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHttpBase|SmartHttpResponse")
	USmartHttpContent* Content;
};

//BP
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSmartHttpSingleRequestCompleteDelegate, const FSmartHttpRequest, Request, const FSmartHttpResponse, Response, bool ,bConnectedSuccessfully);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSmartHttpSingleRequestProgressDelegate, const FSmartHttpRequest, Request, int64, BytesSent, int64, BytesReceived);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSmartHttpSingleRequestHeaderReceivedDelegate, const FSmartHttpRequest, Request, const FString, HeaderName, const FString, NewHeaderValue);
DECLARE_DYNAMIC_DELEGATE(FAllRequestCompleteDelegate);

//C++
DECLARE_DELEGATE_ThreeParams(FSmartSingleCompleteDelegate, const FSmartHttpRequest&, const FSmartHttpResponse&, bool);
DECLARE_DELEGATE_ThreeParams(FSmartSingleRequestProgressDelegate, const FSmartHttpRequest&, int64, int64);
DECLARE_DELEGATE_ThreeParams(FSmartSingleRequestHeaderReceivedDelegate, const FSmartHttpRequest&, const FString&, const FString&);

USTRUCT(BlueprintType)
struct SMARTHTTP_API FSmartHTTPBPResponseDelegate
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHTTP|HTTPResponseDelegate")
	FSmartHttpSingleRequestCompleteDelegate				SmartHttpRequestCompleteDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHTTP|HTTPResponseDelegate")
	FSmartHttpSingleRequestProgressDelegate				SmartHttpRequestProgressDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHTTP|HTTPResponseDelegate")
	FSmartHttpSingleRequestHeaderReceivedDelegate		SmartHttpRequestHeaderReceivedDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartHTTP|HTTPResponseDelegate")
	FAllRequestCompleteDelegate							AllRequestCompleteDelegate;
};

struct SMARTHTTP_API FSmartHTTPResponseDelegate
{
	FSmartSingleCompleteDelegate						SmartCompleteDelegate;
	FSmartSingleRequestProgressDelegate					SmartSingleRequestProgressDelegate;
	FSmartSingleRequestHeaderReceivedDelegate			SmartSingleRequestHeaderReceivedDelegate;
	FSimpleDelegate										AllTasksCompletedDelegate;
};