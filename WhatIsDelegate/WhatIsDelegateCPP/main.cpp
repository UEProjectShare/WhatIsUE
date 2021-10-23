#include "iostream"
#include "Delegate.h"
using namespace std;

DEFINITION_SIMPLE_SINGLE_DELEGATE(SIMPLETEST, void, int)
DEFINITION_SIMPLE_MULTICAST_DELEGATE(MULTICASTTEST, int, int)

int Test(int a)
{
	cout << "Test()" << endl;
	return a;
}

struct FTest
{
	int Test(int a)
	{
		cout << "FTest.Test()" << endl;
		return a;
	}

	void TestObj(int a)
	{
		cout << "FTest.TestObj()��" << a << endl;
	}

	static int TestStatic(int a)
	{
		cout << "FTest:TestStatic()" << endl;
		return a;
	}
};

int main()
{
	FTest TestStruct;

	//����1
	SIMPLE_SINGLE_DELEGATE(Simple1, void, int);
	//�󶨺���
	Simple1.Bind([](int a)
	{
		cout << "SIMPLE_SINGLE_DELEGATE" << endl;
	});
	Simple1.Execute(1);

	Simple1.ReleaseDelegate();

	//�󶨶���
	Simple1.Bind(&TestStruct, &FTest::TestObj);
	Simple1.Execute(1);

	Simple1.ReleaseDelegate();

	//����2
	SIMPLETEST Simple2;
	//�󶨺���
	Simple2.Bind([](int a)
	{
		cout << "SIMPLE_SINGLE_DELEGATE" << endl;
	});
	Simple2.Execute(1);

	Simple2.ReleaseDelegate();

	//�󶨶���
	Simple2.Bind(&TestStruct, &FTest::TestObj);
	Simple2.Execute(2);

	Simple2.ReleaseDelegate();

	//�ಥ1
	MULTICAST_SINGLE_DELEGATE(Multicast1, int, int);
	Multicast1.AddFunction([](int a) -> int
	{
		cout << "SIMPLE_MULTICAST_DELEGATE" << endl;
		return 0;
	});

	Multicast1.AddFunction(Test);

	Multicast1.AddFunction(FTest::TestStatic);

	Multicast1.AddFunction(&TestStruct, &FTest::Test);

	Multicast1.Broadcast(100);
	Multicast1.ReleaseDelegates();

	//�ಥ2
	MULTICASTTEST Multicast2;
	Multicast2.AddFunction([](int a) -> int
	{
		cout << "SIMPLE_MULTICAST_DELEGATE" << endl;
		return 0;
	});
	
	Multicast2.AddFunction(Test);

	Multicast2.AddFunction(FTest::TestStatic);

	Multicast2.AddFunction(&TestStruct, &FTest::Test);

	Multicast2.Broadcast(100);
	Multicast2.ReleaseDelegates();

	return 0;
}