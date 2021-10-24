#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

//Synchronous asynchronous thread, which is easy to use 
struct SMARTTHREADPRO_API FSmartAbandonable : FNonAbandonableTask
{
	FSmartAbandonable(const FSimpleDelegate &InThreadDelegate);

	//Where threads really execute logic 
	void DoWork();

	//ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSmartAbandonable, STATGROUP_ThreadPoolAsyncTasks);
	}
protected:

	//Bound events 
	FSimpleDelegate ThreadDelegate;
};