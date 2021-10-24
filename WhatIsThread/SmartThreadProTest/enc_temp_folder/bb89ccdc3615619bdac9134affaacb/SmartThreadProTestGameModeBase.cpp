// Copyright Epic Games, Inc. All Rights Reserved.


#include "SmartThreadProTestGameModeBase.h"
#include "ThreadManage.h"

PRAGMA_DISABLE_OPTIMIZATION

TArray<FThreadHandle> ThreadHandle;

//锁
FCriticalSection Mutex;

//打印
void ThreadPrint(const FString Mes)
{
	{
		FScopeLock ScopeLock(&Mutex);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *Mes);
			UE_LOG(LogTemp, Display, TEXT("%s"), *Mes);
		}
	}
}

//结构体
struct FMyStruct
{
	void Hello(FString Mes)
	{
		ThreadPrint(FString::Printf(TEXT("FMyStruct::Hello : %s"), *Mes));
	}
};

//智能指针
struct FMyStructSP : public TSharedFromThis<FMyStructSP>
{
	void HelloSP(FString Mes)
	{
		UE_LOG(LogTemp, Display, TEXT("HelloSP"));
		ThreadPrint(FString::Printf(TEXT("FMyStructSP::Hello : %s"), *Mes));
	}
};

void ASmartThreadProTestGameModeBase::Test1(int32 i)
{
	ThreadPrint(FString::Printf(TEXT("Test1 : %i"), i));
}

void ASmartThreadProTestGameModeBase::Test2(int32 i, FString Mes)
{
	ThreadPrint(FString::Printf(TEXT("Test2 : %i ,Mes = %s"), i, *Mes));
}

void ASmartThreadProTestGameModeBase::Do()
{
	if (Handle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
	
	//同步执行
	for (auto &Tmp : ThreadHandle)
	{
		//GThread::GetProxy().Join(Tmp);
	}

	//异步执行
	for (auto &Tmp : ThreadHandle)
	{
		GThread::GetProxy().Detach(Tmp);
	}
}

void ASmartThreadProTestGameModeBase::Run()
{
}

void ASmartThreadProTestGameModeBase::OK()
{
	ThreadPrint(TEXT("Windows Run"));
}

FMyStruct MyStruct;
TSharedPtr<FMyStructSP> MyStructSP = MakeShareable(new FMyStructSP);

void ASmartThreadProTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//1.代理线程(FThreadProxyManage)
	//代理线程有着自己的线程池方案，当你通过代理线程创建一个线程，那么这个线程会加入到线程池内，不会被销毁掉而是挂起，已等待下一个任务。

	//CreateXXX：创建线程并且直接执行任务,是最快捷的异步方式
	GThread::GetProxy().CreateUObject(this, &ASmartThreadProTestGameModeBase::Test1, 1);
	GThread::GetProxy().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("ProxyHello~"));
	GThread::GetProxy().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ProxyHelloSP~"));
	GThread::GetProxy().CreateUFunction(this, TEXT("Test2"), 123, FString("ProxyTest2"));
	GThread::GetProxy().CreateLambda([](FString Mes)
	{
		ThreadPrint(Mes);
	}, "ProxyLambda");

	//BindXXX:创建线程并且绑定任务，但不执行，通过Join和Detach来决定是异步执行还是同步执行
	ThreadHandle.SetNum(5);

	ThreadHandle[0] = GThread::GetProxy().BindUObject(this, &ASmartThreadProTestGameModeBase::Test1, 1);
	ThreadHandle[1] = GThread::GetProxy().BindRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadHandleHello~"));
	ThreadHandle[2] = GThread::GetProxy().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadHandleHelloSP~"));
	ThreadHandle[3] = GThread::GetProxy().BindUFunction(this, TEXT("Test2"), 123, FString("ThreadHandleTest2"));
	ThreadHandle[4] = GThread::GetProxy().BindLambda([](FString Mes)
	{
		ThreadPrint(Mes);
	}, "ThreadHandleLambda");


	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASmartThreadProTestGameModeBase::Do, 3.f);

	//2.任务线程(FThreadTaskManagement)
	//使用任务线程，可以往线程里面不断的去放任务

	//BindXXX 添加到任务队列中 如果有空置的线程可以直接执行该任务
	GThread::GetTask().BindUObject(this, &ASmartThreadProTestGameModeBase::Test1, 2);
	GThread::GetTask().BindRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadTaskHello~"));
	GThread::GetTask().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadTaskHelloSP~"));
	GThread::GetTask().BindUFunction(this, TEXT("Test2"), 123, FString("ThreadTaskTest2"));
	GThread::GetTask().BindLambda([](FString Mes)
	{
		ThreadPrint(Mes);
	}, "ThreadTaskLambda");


}

void ASmartThreadProTestGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GThread::Get()->Destroy();
}

void ASmartThreadProTestGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GThread::Get()->Tick(DeltaSeconds);
}

PRAGMA_ENABLE_OPTIMIZATION
