# 食用方法
## RunnableThread
```C++
class FMyRunnable : public FRunnable
{
public:
	FMyRunnable(const EThreadPriority InPriority,
		unsigned int InStack,
		const char* InThreadName)
		: bStop(false)
	{
		Thread = Platform::Create(this, InPriority, InStack, InThreadName);
	}

	virtual bool Init()
	{
		return true;
	};

	virtual int Run()
	{
		for (;;)
		{
			int i = 0;
			while (!bStop)//逻辑层
			{
				//具体逻辑
				Sleep(200);

				std::cout << "FMyRunnable" << std::endl;

				if (i >= 10)//打印够次数，挂起线程
				{
					if (Thread)
					{
						Thread->Suspend();
					}
				}

				i++;
			}

			Sleep(1000);
			break;
		}

		return 0;
	}

	virtual int Stop()
	{
		return 0;
	}

	virtual bool Exit()
	{
		delete Thread;
		return true;
	}

	virtual FRunnableThread* GetThread()
	{
		return Thread;
	}
protected:
	bool bStop;
	FRunnableThread* Thread;
};

int main()
{
	char ThreadName[512] = "SmartThreadLite";
	FMyRunnable(EThreadPriority::TPri_Normal, 0, ThreadName);

	{
		FEvent();

		//主线程执行任务，主线程直接退出，FMyRunnable就会被析构了
		while (1)
		{
			std::cout << "MainThread" << std::endl;
			Sleep(10000);
		}

	}

	return 0;
}
```

## TaskRunnable
```C++
int main()
{
    //TaskRunnable
	auto Task1 = [](FTaskRunnable* R)
	{
		std::cout << "Hello Task1" << std::endl;
	};

	auto Task2 = [](FTaskRunnable* R)
	{
		std::cout << "Hello Task2" << std::endl;
	};

	ThreadManage.Bind(Task1);
	ThreadManage.Bind(Task2);

	{
		FEvent();

		//主线程执行任务
		while (1)
		{
			std::cout << "MainThread" << std::endl;
			Sleep(10000);
		}

	}

	return 0;
}
```