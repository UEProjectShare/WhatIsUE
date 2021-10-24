#include "Core/SmartThreadType.h"
#include "Interface/ProxyInterface.h"

uint64 SpawnUniqueID()
{
	return FMath::Abs(FMath::RandRange(100, 1256343) + FDateTime::Now().GetTicks());
}

FSmartThreadHandle::FSmartThreadHandle()
	: GUIDTypeA(SpawnUniqueID())
	, GUIDTypeB(SpawnUniqueID())
	, GUIDTypeC(SpawnUniqueID())
	, GUIDTypeD(SpawnUniqueID())
{

}

IThreadProxy::IThreadProxy()
	: SmartThreadHandle(MakeShareable(new FSmartThreadHandle))
{

}