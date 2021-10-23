// Copyright Epic Games, Inc. All Rights Reserved.


#include "WhatIsDelegateUE4GameModeBase.h"

void AWhatIsDelegateUE4GameModeBase::StaticTestDelegateProc(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("StaticTestDelegateProc : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::StaticDelegateProc(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("StaticDelegateProc : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::OnBindAndExecute()
{
	// Bind Static
	TestDelegate1.BindStatic(StaticDelegateProc);

	TestDelegate2.BindStatic(AWhatIsDelegateUE4GameModeBase::StaticTestDelegateProc);

	// Bind Raw
	DelegateCppTestClass Obj1;
	TestDelegate3.BindRaw(&Obj1, &DelegateCppTestClass::CppDelegateProc);

	// Bind Lambda
	auto LambdaDelegateProc = [](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("LambdaDelegateProc : %d"), nCode);
	};

	TestDelegate4.BindLambda(LambdaDelegateProc);

	TestDelegate5.BindLambda(
		[](int nCode)->void
		{
			UE_LOG(LogTemp, Log, TEXT("LambdaDelegateProc2 : %d"), nCode);
		}
	);

	// Bind Weak Lambda
	auto WeakLambdaDelegateProc = [](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("WeakLambdaDelegateProc : %d"), nCode);
	};
	
	UDelegatepTestClass* UObj1 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegate6.BindWeakLambda(UObj1, WeakLambdaDelegateProc);

	UDelegatepTestClass* UObj2 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegate7.BindWeakLambda(
		UObj2, [](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("WeakLambdaDelegateProc2 : %d"), nCode);
	}
	);

	// Bind SP(Shared Pointer)
	TSharedRef<DelegateCppTestClass> ObjSP1 = MakeShareable(new DelegateCppTestClass());
	TestDelegate8.BindSP(ObjSP1, &DelegateCppTestClass::CppDelegateProc2);

	TSharedRef<DelegateCppTestClass> ObjSP2 = MakeShared<DelegateCppTestClass>();
	TestDelegate9.BindSP(ObjSP2, &DelegateCppTestClass::CppDelegateProc3);

	// Bind Thread Safe SP(Shared Pointer)
	TSharedRef<DelegateCppTestClass, ESPMode::ThreadSafe> ObjSafeSP1 = MakeShareable(new DelegateCppTestClass());
	TestDelegate10.BindThreadSafeSP(ObjSafeSP1, &DelegateCppTestClass::CppDelegateProc4);

	TSharedRef<DelegateCppTestClass, ESPMode::ThreadSafe> ObjSafeSP2 = MakeShared<DelegateCppTestClass, ESPMode::ThreadSafe>();
	TestDelegate11.BindThreadSafeSP(ObjSafeSP2, &DelegateCppTestClass::CppDelegateProc5);

	// Bind UObject
	UDelegatepTestClass* UObj3 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegate12.BindUObject(UObj3, &UDelegatepTestClass::DelegateProc1);

	// Bind UFunction
	UDelegatepTestClass* UObj4 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegate13.BindUFunction(UObj4, STATIC_FUNCTION_FNAME(TEXT("UDelegatepTestClass::DelegateUFunctionProc1")));

	{
		TestDelegate1.ExecuteIfBound(1);
		TestDelegate2.ExecuteIfBound(2);
		TestDelegate3.ExecuteIfBound(3);
		TestDelegate4.ExecuteIfBound(4);
		TestDelegate5.ExecuteIfBound(5);
		TestDelegate6.ExecuteIfBound(6);
		TestDelegate7.ExecuteIfBound(7);
		TestDelegate8.ExecuteIfBound(8);
		TestDelegate9.ExecuteIfBound(9);
		TestDelegate10.ExecuteIfBound(10);
		TestDelegate11.ExecuteIfBound(11);
		TestDelegate12.ExecuteIfBound(12);
		if (TestDelegate13.IsBound())
		{
			TestDelegate13.Execute(13);
		}
	}

	OnUnbind();
}

void AWhatIsDelegateUE4GameModeBase::OnUnbind()
{
	TestDelegate1.Unbind();
	TestDelegate2.Unbind();
	TestDelegate3.Unbind();
	TestDelegate4.Unbind();
	TestDelegate5.Unbind();
	TestDelegate6.Unbind();
	TestDelegate7.Unbind();
	TestDelegate8.Unbind();
	TestDelegate9.Unbind();
	TestDelegate10.Unbind();
	TestDelegate11.Unbind();
	TestDelegate12.Unbind();
	TestDelegate13.Unbind();
}

void AWhatIsDelegateUE4GameModeBase::StaticDelegateProc2(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("StaticDelegateProc2 : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::OnDelegateMulticastTest()
{
	// Add Static
	bool b1 = TestDelegateMulticast1.IsBound(); // false

	FDelegateHandle HandleMC1 = TestDelegateMulticast1.AddStatic(StaticDelegateProc); // 绑定实例个数为：1
	TestDelegateMulticast1.AddStatic(AWhatIsDelegateUE4GameModeBase::StaticTestDelegateProc); // 绑定实例个数为：2

	bool b2 = TestDelegateMulticast1.IsBound(); // true
	bool b3 = TestDelegateMulticast1.IsBoundToObject(this); // false

	TestDelegateMulticast1.Remove(HandleMC1); // 绑定实例个数为：1

	FTestDelegate_Multicast::FDelegate MC1 = FTestDelegate_Multicast::FDelegate::CreateStatic(StaticDelegateProc2);
	TestDelegateMulticast1.Add(MC1); // 绑定实例个数为：2
	TestDelegateMulticast1.Broadcast(1); // 执行绑定实例列表（共2个）  注：执行顺序可能与函数的添加顺序不相同


	// Add Raw
	DelegateCppTestClass ObjMC1;
	TestDelegateMulticast2.AddRaw(&ObjMC1, &DelegateCppTestClass::CppDelegateProc); // 绑定实例个数为：1
	TestDelegateMulticast2.AddRaw(&ObjMC1, &DelegateCppTestClass::CppDelegateProc2); // 绑定实例个数为：2
	TestDelegateMulticast2.AddStatic(/*AWhatIsDelegateUE4GameModeBase::*/StaticTestDelegateProc); // 绑定实例个数为：3

	bool b4 = TestDelegateMulticast2.IsBoundToObject(&ObjMC1); // true

	TestDelegateMulticast2.RemoveAll(&ObjMC1); // CppDelegateProc、CppDelegateProc2被删除，绑定实例个数为：1

	bool b5 = TestDelegateMulticast2.IsBoundToObject(&ObjMC1); // false

	TestDelegateMulticast2.Broadcast(2); // 执行绑定实例列表（共1个）  注：执行顺序可能与函数的添加顺序不相同

	TestDelegateMulticast2.Clear(); // 绑定实例个数为：0

	// Add Lambda
	auto LambdaDelegateMCProc = [](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("LambdaDelegateMCProc : %d"), nCode);
	};
	TestDelegateMulticast3.AddLambda(LambdaDelegateMCProc);// 绑定实例个数为：1

	TestDelegateMulticast3.AddLambda(
		[](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("LambdaDelegateMCProc2 : %d"), nCode);
	}
	);// 绑定实例个数为：2
	
	TestDelegateMulticast3.Broadcast(3);// 执行绑定实例列表（共2个）  注：执行顺序可能与函数的添加顺序不相同

	// Add Weak Lambda
	auto WeakLambdaDelegateMCProc = [](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("WeakLambdaDelegateMCProc : %d"), nCode);
	};
	
	UDelegatepTestClass* UObjMC1 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegateMulticast4.AddWeakLambda(UObjMC1, WeakLambdaDelegateMCProc);// 绑定实例个数为：1

	UDelegatepTestClass* UObjMC2 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegateMulticast4.AddWeakLambda(
		UObjMC2, [](int nCode)->void
	{
		UE_LOG(LogTemp, Log, TEXT("WeakLambdaDelegateMCProc2 : %d"), nCode);
	}
	);// 绑定实例个数为：2
	
	TestDelegateMulticast4.Broadcast(4);// 执行绑定实例列表（共2个）  注：执行顺序可能与函数的添加顺序不相同

	// Add SP(Shared Pointer)
	TSharedRef<DelegateCppTestClass> ObjMCSP1 = MakeShareable(new DelegateCppTestClass());
	TestDelegateMulticast5.AddSP(ObjMCSP1, &DelegateCppTestClass::CppDelegateProc2);// 绑定实例个数为：1

	TSharedRef<DelegateCppTestClass> ObjMCSP2 = MakeShared<DelegateCppTestClass>();
	TestDelegateMulticast5.AddSP(ObjMCSP2, &DelegateCppTestClass::CppDelegateProc3);// 绑定实例个数为：2
	TestDelegateMulticast5.Broadcast(5);

	// Add Thread Safe SP(Shared Pointer)
	TSharedRef<DelegateCppTestClass, ESPMode::ThreadSafe> ObjSafeMCSP1 = MakeShareable(new DelegateCppTestClass());
	TestDelegateMulticast6.AddThreadSafeSP(ObjSafeMCSP1, &DelegateCppTestClass::CppDelegateProc4);// 绑定实例个数为：1

	TSharedRef<DelegateCppTestClass, ESPMode::ThreadSafe> ObjSafeMCSP2 = MakeShared<DelegateCppTestClass, ESPMode::ThreadSafe>();
	TestDelegateMulticast6.AddThreadSafeSP(ObjSafeMCSP2, &DelegateCppTestClass::CppDelegateProc5);// 绑定实例个数为：2

	TestDelegateMulticast6.Broadcast(6);// 执行绑定实例列表（共2个）  注：执行顺序可能与函数的添加顺序不相同

	// Add UObject
	UDelegatepTestClass* UObjMC3 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegateMulticast7.AddUObject(UObjMC3, &UDelegatepTestClass::DelegateProc1);// 绑定实例个数为：1
	TestDelegateMulticast7.AddUObject(UObjMC3, &UDelegatepTestClass::DelegateProc2);// 绑定实例个数为：2

	TestDelegateMulticast7.Broadcast(7);// 执行绑定实例列表（共2个）  注：执行顺序可能与函数的添加顺序不相同

	// Add UFunction
	UDelegatepTestClass* UObjMC4 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegateMulticast8.AddUFunction(UObjMC4, STATIC_FUNCTION_FNAME(TEXT("UDelegatepTestClass::DelegateUFunctionProc1")));// 绑定实例个数为：1
	TestDelegateMulticast8.Broadcast(8);// 执行绑定实例列表（共1个）  注：执行顺序可能与函数的添加顺序不相同
}

void AWhatIsDelegateUE4GameModeBase::OnTriggerEvent(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("OnTriggerEvent : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::OnEventTest()
{
	TestEvent.AddUObject(this, &AWhatIsDelegateUE4GameModeBase::OnTriggerEvent);
	TestEvent.Broadcast(1);
}

void AWhatIsDelegateUE4GameModeBase::OnDelegateDynamicTest()
{
	bool bd1 = TestDelegateDynamic.IsBound(); // false

	UDelegatepTestClass* UObjDy1 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	TestDelegateDynamic.BindUFunction(UObjDy1, STATIC_FUNCTION_FNAME(TEXT("UDelegatepTestClass::DelegateUFunctionProc1")));// 绑定实例个数为：1

	bool bd2 = TestDelegateDynamic.IsBound(); // true

	TestDelegateDynamic.ExecuteIfBound(1);

	TestDelegateDynamic.Unbind();

	// 使用BindDynamic宏
	TestDelegateDynamic2.BindDynamic(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc);
	if (TestDelegateDynamic2.IsBound()) // true
	{
		TestDelegateDynamic2.Execute(2);
	}

	TestDelegateDynamic2.Clear(); // 功能与Unbind一样，内部是直接调用Unbind方法
}

bool AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("DynamicMulticastProc : %d"), nCode);
	return true;
}

void AWhatIsDelegateUE4GameModeBase::OnDelegateDynamicMulticastTest()
{
	UDelegatepTestClass* UObjDMC1 = NewObject<UDelegatepTestClass>(this, UDelegatepTestClass::StaticClass());
	// 使用AddDynamic宏
	//TestDelegateDynamicMulticast.AddDynamic(UObjDMC1, &UDelegatepTestClass::DelegateProc1); // 运行时错误：Unable to bind delegate (function might not be marked as a UFUNCTION or object may be pending kill)
	TestDelegateDynamicMulticast.AddDynamic(UObjDMC1, &UDelegatepTestClass::DelegateUFunctionProc1); // 绑定实例个数为：1
	//TestDelegateDynamicMulticast.AddDynamic(UObjDMC1, &UDelegatepTestClass::DelegateUFunctionProc1); // 运行时错误 加入DelegateUFunctionProc1的绑定实例已存在
	TestDelegateDynamicMulticast.AddDynamic(UObjDMC1, &UDelegatepTestClass::DelegateUFunctionProc2); // 绑定实例个数为：2

	// 使用AddUniqueDynamic宏
	TestDelegateDynamicMulticast.AddUniqueDynamic(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc1); // 绑定实例个数为：3
	TestDelegateDynamicMulticast.AddUniqueDynamic(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc1); // 加入DynamicMulticastProc的绑定实例已存在
	TestDelegateDynamicMulticast.AddUniqueDynamic(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc2); // 绑定实例个数为：4

	FScriptDelegate delegateVar1; // FScriptDelegate即为TScriptDelegate<> //typedef TScriptDelegate<> FScriptDelegate;
	delegateVar1.BindUFunction(this, STATIC_FUNCTION_FNAME(TEXT("AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc3")));
	TestDelegateDynamicMulticast.Add(delegateVar1); // 绑定实例个数为：5
	FScriptDelegate delegateVar2;
	delegateVar2.BindUFunction(this, STATIC_FUNCTION_FNAME(TEXT("AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc3")));
	//TestDelegateDynamicMulticast.Add(delegateVar2); // 运行时错误 加入DynamicMulticastProc3的绑定实例已存在
	TestDelegateDynamicMulticast.AddUnique(delegateVar2); // 加入DynamicMulticastProc3的绑定实例已存在

	// 使用RemoveDynamic宏
	TestDelegateDynamicMulticast.RemoveDynamic(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc2); // 绑定实例个数为：4

	// 使用IsAlreadyBound宏
	bool bDMC1 = TestDelegateDynamicMulticast.IsAlreadyBound(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc3);// true

	TestDelegateDynamicMulticast.Remove(delegateVar2); // 绑定实例个数为：3
	TestDelegateDynamicMulticast.Remove(UObjDMC1, STATIC_FUNCTION_FNAME(TEXT("UDelegatepTestClass::DelegateUFunctionProc1")));// 绑定实例个数为：2

	bool bDMC2 = TestDelegateDynamicMulticast.IsAlreadyBound(this, &AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc3); // false

	TestDelegateDynamicMulticast.RemoveAll(this); // 绑定实例个数为：1

	TestDelegateDynamicMulticast.Broadcast(300); // 执行绑定实例列表（共1个）  注：执行顺序可能与函数的添加顺序不相同

	TestDelegateDynamicMulticast.Clear(); // 清除所有绑定实例
}

void AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc1(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("DynamicMulticastProc1 : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc2(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("DynamicMulticastProc2 : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::DynamicMulticastProc3(int nCode)
{
	UE_LOG(LogTemp, Log, TEXT("DynamicMulticastProc3 : %d"), nCode);
}

void AWhatIsDelegateUE4GameModeBase::BrocastTestDelegateDynamicMulticast(int nCode)
{
	TestDelegateDynamicMulticast2.Broadcast(nCode);
	TestDelegateDynamicMulticast2.Clear();
}