#include "Core/SmartSemaphore.h"
#include "HAL/Event.h"
#include "SmartThreadProPlatform.h"

FSmartSemaphore::FSmartSemaphore()
	: Event(FPlatformProcess::GetSynchEventFromPool())
	, bWait(false)
{

}

FSmartSemaphore::~FSmartSemaphore()
{
	FPlatformProcess::ReturnSynchEventToPool(Event);
}

void FSmartSemaphore::Wait()
{
	bWait = true;
	Event->Wait();
}

void FSmartSemaphore::Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats)
{
	bWait = true;
	Event->Wait(WaitTime, bIgnoreThreadIdleStats);
}

void FSmartSemaphore::Trigger()
{
	Event->Trigger();
	bWait = false;
}