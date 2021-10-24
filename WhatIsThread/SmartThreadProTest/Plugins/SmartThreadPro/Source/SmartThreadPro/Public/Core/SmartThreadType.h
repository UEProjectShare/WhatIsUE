#pragma once
#include "CoreMinimal.h"

//Mainly used as guid 
struct SMARTTHREADPRO_API FSmartThreadHandle : public TSharedFromThis<FSmartThreadHandle>
{
	FSmartThreadHandle();

	bool operator==(const FSmartThreadHandle& SmartThreadHandle)
	{
		return
			this->GUIDTypeA == SmartThreadHandle.GUIDTypeA &&
			this->GUIDTypeB == SmartThreadHandle.GUIDTypeB &&
			this->GUIDTypeC == SmartThreadHandle.GUIDTypeC &&
			this->GUIDTypeD == SmartThreadHandle.GUIDTypeD;
	}

protected:
	uint64 GUIDTypeA;
	uint64 GUIDTypeB;
	uint64 GUIDTypeC;
	uint64 GUIDTypeD;
};

//Thread state 
enum class EThreadState :uint8
{
	THREAD_LEISURELY,	
	THREAD_WORKING,	
	THREAD_ERROR,		
};

typedef TWeakPtr<FSmartThreadHandle> FThreadHandle;
typedef TFunction<void()> FThreadLambda;

