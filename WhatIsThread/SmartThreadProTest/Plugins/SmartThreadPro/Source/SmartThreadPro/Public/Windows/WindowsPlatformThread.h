#pragma once

#include "CoreTypes.h"

//Windows native thread, this thread scheme can be used in special cases 
struct SMARTTHREADPRO_API FWindowsPlatformThread
{
	static void Show();

	static void Hide();

	static bool IsShown();

	static FSimpleDelegate CompletedDelegate;

	static FSimpleDelegate RunDelegate;
};
