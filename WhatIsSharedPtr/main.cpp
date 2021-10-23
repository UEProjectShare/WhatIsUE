#include "SharedPtr.h"


class FTestPtr
{
public:
	FTestPtr()
	{
		a = 1000;
	}

	~FTestPtr()
	{

	}

	void Init()
	{
		cout << "Init" << endl;
	}

private:
	int a;
};

int main()
{
	FSharedPtr<FTestPtr> TestPtr1(new FTestPtr());
	FSharedPtr<FTestPtr> TestPtr2(TestPtr1);
	FSharedPtr<FTestPtr> TestPtr3;
	TestPtr3 = TestPtr1;//调用重载=

	TestPtr3.Release();
	TestPtr2->Init();
	TestPtr2.Release();

	FWeakPtr<FTestPtr> TestPtr4 = TestPtr1;//调用拷贝构造函数
	FWeakPtr<FTestPtr> TestPtr5(TestPtr1);//调用拷贝构造函数
	TestPtr5.Pin()->Init();
	TestPtr5.Release();

	return 0;
}