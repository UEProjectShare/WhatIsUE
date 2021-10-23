// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WhatIsDelegateUE4GameModeBase.generated.h"

UCLASS()
class UDelegatepTestClass : public UObject
{
	GENERATED_BODY()
protected:
	int m_nValue = 0;
public:
	void DelegateProc1(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateProc1 : %d"), nCode);
	}

	void DelegateProc2(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateProc2 : %d"), nCode);
	}

	UFUNCTION()
	void DelegateUFunctionProc1(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateUFunctionProc1 : %d"), nCode);
	}

	UFUNCTION()
	void DelegateUFunctionProc2(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateUFunctionProc2 : %d"), nCode);
	}
};

class DelegateCppTestClass
{
	int m_nValue = 0;
public:

	void CppDelegateProc(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc : %d"), nCode);
	}

	void CppDelegateProc2(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc2 : %d"), nCode);
	}

	void CppDelegateProc3(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc3 : %d"), nCode);
	}

	void CppDelegateProc4(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc4 : %d"), nCode);
	}

	void CppDelegateProc5(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc5 : %d"), nCode);
	}
};

DECLARE_DELEGATE_OneParam(FTestDelegate, int); 

DECLARE_DELEGATE_RetVal_OneParam(bool, FTestDelegate_RetVal, int); 

DECLARE_MULTICAST_DELEGATE_OneParam(FTestDelegate_Multicast, int);

DECLARE_DYNAMIC_DELEGATE_OneParam(FTestDelegate_Dynamic, int, nCode);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FTestDelegate_DynamicRetVal, int, nCode);  

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTestDelegate_DynamicMulticast, int, nCode);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTestDelegate_DynamicMulticast2, int, nCode); 

UCLASS()
class WHATISDELEGATEUE4_API AWhatIsDelegateUE4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	static void StaticTestDelegateProc(int nCode);
	static void StaticDelegateProc(int nCode);

	UFUNCTION(BlueprintCallable)
	void OnBindAndExecute();
	
	UFUNCTION(BlueprintCallable)
	void OnUnbind();

	FTestDelegate TestDelegate1;
	FTestDelegate TestDelegate2;
	FTestDelegate TestDelegate3;
	FTestDelegate TestDelegate4;
	FTestDelegate TestDelegate5;
	FTestDelegate TestDelegate6;
	FTestDelegate TestDelegate7;
	FTestDelegate TestDelegate8;
	FTestDelegate TestDelegate9;
	FTestDelegate TestDelegate10;
	FTestDelegate TestDelegate11;
	FTestDelegate TestDelegate12;
	FTestDelegate TestDelegate13;

	static void StaticDelegateProc2(int nCode);

	UFUNCTION(BlueprintCallable)
	void OnDelegateMulticastTest();

	FTestDelegate_Multicast TestDelegateMulticast1;
	FTestDelegate_Multicast TestDelegateMulticast2;
	FTestDelegate_Multicast TestDelegateMulticast3;
	FTestDelegate_Multicast TestDelegateMulticast4;
	FTestDelegate_Multicast TestDelegateMulticast5;
	FTestDelegate_Multicast TestDelegateMulticast6;
	FTestDelegate_Multicast TestDelegateMulticast7;
	FTestDelegate_Multicast TestDelegateMulticast8;

	void OnTriggerEvent(int nCode);

	UFUNCTION(BlueprintCallable)
	void OnEventTest();

	DECLARE_EVENT_OneParam(AWhatIsDelegateUE4GameModeBase, FTestEvent, int);
	
	FTestEvent TestEvent;

	UFUNCTION(BlueprintCallable)
	void OnDelegateDynamicTest();
	
	UFUNCTION()//注意此处要加宏
	bool DynamicMulticastProc(int nCode);

	FTestDelegate_Dynamic TestDelegateDynamic;//vs报错请忽略
	FTestDelegate_DynamicRetVal TestDelegateDynamic2;

	UFUNCTION(BlueprintCallable)
	void OnDelegateDynamicMulticastTest();

	UFUNCTION()//注意此处要加宏
	void DynamicMulticastProc1(int nCode);

	UFUNCTION()//注意此处要加宏
	void DynamicMulticastProc2(int nCode);

	UFUNCTION()//注意此处要加宏
	void DynamicMulticastProc3(int nCode);

	FTestDelegate_DynamicMulticast TestDelegateDynamicMulticast;

	UFUNCTION(BlueprintCallable)
	void BrocastTestDelegateDynamicMulticast(int nCode);

	UPROPERTY(BlueprintAssignable)
	FTestDelegate_DynamicMulticast2 TestDelegateDynamicMulticast2;
};
