# 食用方法
## 配置文件SmartThreadPro.Build.cs
```C++
using UnrealBuildTool;

public class SmartThreadPro : ModuleRules
{
	public SmartThreadPro(ReadOnlyTargetRules Target) : base(Target)
	{
        ......

		//如果是在项目里面想用到该内容，就把它打开
        //如果是独立程序用到该插件，请注释该行，独立程序中不能使用Engine模块
		PublicDefinitions.Add("PLATFORM_PROJECT");
		if (PublicDefinitions.Contains("PLATFORM_PROJECT"))
        {
			PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine"
			});
		}

        ......
	}
}

```
## 测试用例
```C++
TArray<FThreadHandle> ThreadHandle;
TArray<FCoroutinesHandle> CoroutinesHandle;
TArray<FGraphEventRef > ArrayEventRef;
TSharedPtr<FStreamableHandle> StreamableHandle;

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

void ASmartThreadProTestGameModeBase::Do2()
{
	if (Handle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}

	for (auto& Tmp : CoroutinesHandle)
	{
		if (Tmp.IsValid())
		{
			//唤醒该协程下的事件
			Tmp.Pin()->Awaken();
		}
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

	{
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
	}

	{
		//2.任务线程(FThreadTaskManagement)
		//使用任务线程，可以往线程里面不断的去放任务

		//CreateXXX 直接在线程池里面找 如果有闲置的线程 直接运行当前任务
		GThread::GetTask().CreateUObject(this, &ASmartThreadProTestGameModeBase::Test1, 2);
		GThread::GetTask().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadTaskCreateHello~"));
		GThread::GetTask().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadTaskCreateHelloSP~"));
		GThread::GetTask().CreateUFunction(this, TEXT("Test2"), 123, FString("ThreadTaskCreateTest2"));
		GThread::GetTask().CreateLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadTaskCreateLambda");

		//BindXXX 添加到任务队列中 如果有空置的线程可以直接执行该任务
		GThread::GetTask().BindUObject(this, &ASmartThreadProTestGameModeBase::Test1, 2);
		GThread::GetTask().BindRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadTaskBindHello~"));
		GThread::GetTask().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadTaskBindHelloSP~"));
		GThread::GetTask().BindUFunction(this, TEXT("Test2"), 123, FString("ThreadTaskBindTest2"));
		GThread::GetTask().BindLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadTaskBindLambda");
	}

	{
		//3.同步异步线程(FThreadAbandonableManage)
		//该线程是UE4线程池内的线程，轻巧便捷，要么执行同步，要么执行异步

		//CreateXXX 异步绑定 直接启动，任务完成后自动销毁
		GThread::GetAbandonable().CreateUObject(this, &ASmartThreadProTestGameModeBase::Test1, 3);
		GThread::GetAbandonable().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadAbandonableCreateHello~"));
		GThread::GetAbandonable().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadAbandonableCreateHelloSP~"));
		GThread::GetAbandonable().CreateUFunction(this, TEXT("Test2"), 123, FString("ThreadAbandonableCreateTest2"));
		GThread::GetAbandonable().CreateLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadAbandonableCreateLambda");

		//BindXXX 同步绑定 会阻塞启动线程 完成任务后激活启动线程
		GThread::GetAbandonable().BindUObject(this, &ASmartThreadProTestGameModeBase::Test1, 3);
		GThread::GetAbandonable().BindRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadAbandonableBindHello~"));
		GThread::GetAbandonable().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadAbandonableBindHelloSP~"));
		GThread::GetAbandonable().BindUFunction(this, TEXT("Test2"), 123, FString("ThreadAbandonableBindTest2"));
		GThread::GetAbandonable().BindLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadAbandonableBindLambda");

		//除此之外还添加了便捷的宏启动线程，该线程也是可以执行同步异步

		//同步宏
		SYNCTASK_UOBJECT(this, &ASmartThreadProTestGameModeBase::Test1, 3);
		SYNCTASK_Raw(&MyStruct, &FMyStruct::Hello, FString("SYNCTASKHello~"));
		SYNCTASK_SP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("SYNCTASKHelloSP~"));
		SYNCTASK_UFunction(this, TEXT("Test2"), 123, FString("SYNCTASKTest2"));
		SYNCTASK_Lambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "SYNCTASKLambda");

		//异步宏
		ASYNCTASK_UOBJECT(this, &ASmartThreadProTestGameModeBase::Test1, 3);
		ASYNCTASK_Raw(&MyStruct, &FMyStruct::Hello, FString("ASYNCTASKHello~"));
		ASYNCTASK_SP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ASYNCTASKHelloSP~"));
		ASYNCTASK_UFunction(this, TEXT("Test2"), 123, FString("ASYNCTASKTest2"));
		ASYNCTASK_Lambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ASYNCTASKLambda");
	}

	{
		//4.协程(FCoroutinesManage)

		//CreateXXX 绑定完毕返回一个Handle ,由程序员来决定什么时候执行
		CoroutinesHandle.SetNum(5);
		CoroutinesHandle[0] = GThread::GetCoroutines().CreateUObject(this, &ASmartThreadProTestGameModeBase::Test1, 4);
		CoroutinesHandle[1] = GThread::GetCoroutines().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("CoroutinesCreateHello~"));
		CoroutinesHandle[2] = GThread::GetCoroutines().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("CoroutinesCreateHelloSP~"));
		CoroutinesHandle[3] = GThread::GetCoroutines().CreateUFunction(this, TEXT("Test2"), 123, FString("CoroutinesCreateTest2"));
		CoroutinesHandle[4] = GThread::GetCoroutines().CreateLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "CoroutinesCreateLambda");

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ASmartThreadProTestGameModeBase::Do2, 3.f);

		//Bind XXX 绑定后可以设置时间，多久执行
		//设置1s后执行函数T1
		GThread::GetCoroutines().BindUObject(1.f, this, &ASmartThreadProTestGameModeBase::Test1, 4);
		GThread::GetCoroutines().BindRaw(2.f, &MyStruct, &FMyStruct::Hello, FString("CoroutinesBindHello~"));
		GThread::GetCoroutines().BindSP(2.4f, MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("CoroutinesBindHelloSP~"));
		GThread::GetCoroutines().BindUFunction(4.f, this, TEXT("Test2"), 123, FString("CoroutinesBindTest2"));
		GThread::GetCoroutines().BindLambda(7.f, [](FString Mes)
		{
			ThreadPrint(Mes);
		}, "CoroutinesBindLambda");
	}

	{
		//5.Windows原生线程(FWindowsPlatformThread)
		FWindowsPlatformThread::RunDelegate.BindUObject(this, &ASmartThreadProTestGameModeBase::Run);
		FWindowsPlatformThread::CompletedDelegate.BindUObject(this, &ASmartThreadProTestGameModeBase::OK);
		FWindowsPlatformThread::Show();//执行线程
	}

	{
		//6.异步资源读取(FResourceLoadingManage)
		//异步资源读取我们分为同步读取和异步读取

		//异步
		auto LambdaFunction = [](TSharedPtr<FStreamableHandle>* InHandle)
		{
			TArray<UObject*> ExampleObject;
			(*InHandle)->GetLoadedAssets(ExampleObject);

			for (UObject* Tmp : ExampleObject)
			{
				ThreadPrint(Tmp->GetName());
			}
		};

		//异步使用方法
		GThread::GetResourceLoading() >> ObjectPath;
		StreamableHandle = GThread::GetResourceLoading().CreateLambda(LambdaFunction, &StreamableHandle);

		//同步
		StreamableHandle = GThread::GetResourceLoading() << ObjectPath;
		LambdaFunction(&StreamableHandle);
	}

	{
		//7.图表线程(ThreadGraphManage)
		//图表线程是UE4使用频率最高的线程，它可以实现线程的前置任务

		//CreateXXX 绑定任意线程
		GThread::GetGraph().CreateUObject(this, &ASmartThreadProTestGameModeBase::Test1, 7);
		GThread::GetGraph().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadGraphCreateHello~"));
		GThread::GetGraph().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadGraphCreateHelloSP~"));
		GThread::GetGraph().CreateUFunction(this, TEXT("Test2"), 123, FString("ThreadGraphCreateTest2"));
		GThread::GetGraph().CreateLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadGraphCreateLambda");

		//BindXXX :只呼叫主线程
		ArrayEventRef.SetNum(5);
		ArrayEventRef[0] = GThread::GetGraph().BindUObject(this, &ASmartThreadProTestGameModeBase::Test1, 7);
		ArrayEventRef[1] = GThread::GetGraph().BindRaw(&MyStruct, &FMyStruct::Hello, FString("ThreadGraphBindHello~"));
		ArrayEventRef[2] = GThread::GetGraph().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadGraphBindHelloSP~"));
		ArrayEventRef[3] = GThread::GetGraph().BindUFunction(this, TEXT("Test2"), 123, FString("ThreadGraphBindTest2"));
		ArrayEventRef[4] = GThread::GetGraph().BindLambda([](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadGraphBindLambda");

		//GThread::GetGraph().Wait(ArrayEventRef[0]);
		FGraphEventArray ArrayEvent;
		for (auto& Tmp : ArrayEventRef)
		{
			ArrayEvent.Add(Tmp);
		}
		//可以设置等待这些线程完成任务后再执行自己
		GThread::GetGraph().Wait(ArrayEvent);
		ThreadPrint("Wait-oK");

		//其他图表线程
		//除此之外还有宏类型的图表线程，它的使用更加强大，可以对单个事件进行等待
		auto A = CALL_THREAD_UOBJECT(nullptr, ENamedThreads::AnyThread, this, &ASmartThreadProTestGameModeBase::Test1, 7);
		//等待A事件完成执行B事件
		auto B = CALL_THREAD_Raw(A, ENamedThreads::AnyThread, &MyStruct, &FMyStruct::Hello, FString("ThreadGraphOtherHello~"));
		auto C = CALL_THREAD_SP(nullptr, ENamedThreads::AnyThread, MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("ThreadGraphOtherHelloSP~"));
		auto D = CALL_THREAD_UFunction(nullptr, ENamedThreads::AnyThread, this, TEXT("Test2"), 123, FString("ThreadGraphOtherTest2"));
		auto E = CALL_THREAD_Lambda(nullptr, ENamedThreads::AnyThread, [](FString Mes)
		{
			ThreadPrint(Mes);
		}, "ThreadGraphOtherLambda");
	
		//WAITING_OTHER_THREADS_COMPLETED(EventRef)
		//WAITING_OTHER_THREADS_COMPLETED_MULTI(EventRef)
	}
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
```