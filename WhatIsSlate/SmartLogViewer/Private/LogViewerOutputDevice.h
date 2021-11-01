#pragma once

#include "CoreMinimal.h"
#include "LogViewerStructs.h"

class FLogViewerOutputDevice
{

public:
	TArray<TSharedPtr<FLogMessage>> Messages;
	
	static bool CreateLogMessages(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category, TArray< TSharedPtr<FLogMessage> >& OutMessages);

};