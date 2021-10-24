#include "Abandonable/SmartAbandonable.h"

FSmartAbandonable::FSmartAbandonable(const FSimpleDelegate &InThreadDelegate)
	: ThreadDelegate(InThreadDelegate)
{

}

void FSmartAbandonable::DoWork()
{
	ThreadDelegate.ExecuteIfBound();
}