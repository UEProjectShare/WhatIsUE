#include "ZIPSDK.h"
#include "SmartZIPLibrary/xx/ZIP_SDK_LIBRARY/ZIP_SDK_Library.h"
#include "ZIPType.h"
#include "Async/AsyncWork.h"
#include "Async/TaskGraphInterfaces.h"

FCriticalSection ZipMutex;
class FZipAsynTask :public FNonAbandonableTask
{
public:
	FZipAsynTask(const TFunction<void()> InFuncton)
		:Functon(InFuncton)
	{
	}

	void DoWork()
	{
		Functon();
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FZipAsynTask, STATGROUP_ThreadPoolAsyncTasks);
	}

protected:
	const TFunction<void()> Functon;
};

bool SmartZIP::ZipByLoacl(const TCHAR *SrcPath, const TCHAR *DestFilename, const TCHAR *Password, bool bOverwrite)
{
	auto Result = zip_file_by_loacl(TCHAR_TO_ANSI(SrcPath), TCHAR_TO_ANSI(DestFilename), TCHAR_TO_ANSI(Password), bOverwrite);
	
	SmartZIP::CollectErrorInformation(Result);
	return Result == SMART_ZIP_OK;
}

bool SmartZIP::UnzipByLoacl(const TCHAR *SrcPath, const TCHAR *DestFilename, const TCHAR *Password, bool bOverwrite /*= true*/)
{
	auto Result = unzip_file_by_loacl(TCHAR_TO_ANSI(SrcPath), TCHAR_TO_ANSI(DestFilename), TCHAR_TO_ANSI(Password), bOverwrite);

	SmartZIP::CollectErrorInformation(Result);
	return Result == SMART_ZIP_OK;
}

TFunction<void(EZipType, int32, int32, const FString&)> ZipFunction;
void ZipCallbackFunction(e_zip_typ InType, int Num, int InProgress, const wchar_t* InFileName)
{
	FScopeLock ScopeLock(&ZipMutex);
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([InType, Num, InProgress, InFileName]()
	{
		ZipFunction((EZipType)InType, Num, InProgress, InFileName != NULL ? FString(WCHAR_TO_TCHAR(InFileName)): FString(TEXT("Complete.")));
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void SmartZIP::BindFunction(TFunction<void(EZipType, int32, int32, const FString&)> InZipFunction)
{
	ZipFunction = InZipFunction;
	zip_file_Callback_function(ZipCallbackFunction);
}

void SmartZIP::ClearZipFileCallbackFunction()
{
	clear_Callback_function();
}

void SmartZIP::AsyncZipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite)
{
	auto Lamada = [=]()
	{
		//FString InSrcPath = SrcPath;
		//FString InDestFilename = SrcPath;
		//FString InPassword = Password;
		ZipByLoacl(*SrcPath,*DestFilename,*Password, bOverwrite);
	};

	(new FAutoDeleteAsyncTask<FZipAsynTask>(Lamada))->StartBackgroundTask();
}

void SmartZIP::AsyncUnZipByLoacl(const FString& SrcPath, const FString& DestFilename, const FString& Password, bool bOverwrite /*= true*/)
{
	auto Lamada = [=]()
	{	
		UnzipByLoacl(*SrcPath,*DestFilename,*Password, bOverwrite);
	};

	(new FAutoDeleteAsyncTask<FZipAsynTask>(Lamada))->StartBackgroundTask();
}

