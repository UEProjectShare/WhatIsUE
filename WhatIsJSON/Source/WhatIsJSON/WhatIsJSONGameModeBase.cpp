// Copyright Epic Games, Inc. All Rights Reserved.


#include "WhatIsJSONGameModeBase.h"

struct FObjectTest
{
	int32 Index;
	float Indexf;

};

struct FJSONTest
{
	FJSONTest()
		: Age(0)
		, bMarry(false)
		, Fff(0.f)
		, AlienIdentity(TEXT("Alien"))
		, TestPtr(nullptr)
	{}

	FString Name;
	int32 Age;
	bool bMarry;
	float Fff;
	FString AlienIdentity;

	FObjectTest Test;
	FObjectTest *TestPtr;
	TArray<FJSONTest> Array;
	
	//转Json
	FString ToString()
	{
		FString JsonString;
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);

		ToString(JsonWriter, *this);
		JsonWriter->Close();
		return JsonString;
	}
	
	void ToString(TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter, FJSONTest &InXXX)
	{
		JsonWriter->WriteObjectStart();//{
		{
			JsonWriter->WriteValue(TEXT("Name"), InXXX.Name);
			JsonWriter->WriteValue(TEXT("Age"), InXXX.Age);
			JsonWriter->WriteValue(TEXT("bMarry"), InXXX.bMarry);
			JsonWriter->WriteValue(TEXT("Fff"), InXXX.Fff);
			JsonWriter->WriteValue(TEXT("AlienIdentity"), InXXX.AlienIdentity);
			JsonWriter->WriteObjectStart(TEXT("Test"));//{
			{
				JsonWriter->WriteValue(TEXT("Index"), InXXX.Test.Index);
				JsonWriter->WriteValue(TEXT("Indexf"), InXXX.Test.Indexf);
			}
			JsonWriter->WriteObjectEnd();//}
			
			if (InXXX.TestPtr)
			{
				JsonWriter->WriteObjectStart(TEXT("TestPtr"));//{
				{
					JsonWriter->WriteValue(TEXT("Index"), InXXX.TestPtr->Index);
					JsonWriter->WriteValue(TEXT("Indexf"), InXXX.TestPtr->Indexf);
				}
				JsonWriter->WriteObjectEnd();//}
			}
			
			JsonWriter->WriteArrayStart(TEXT("Array"));//[
			for (auto& Tmp : InXXX.Array)
			{
				ToString(JsonWriter, Tmp);
			}
			JsonWriter->WriteArrayEnd();//]
		}
		JsonWriter->WriteObjectEnd();//}
	}

	//读取json
	void FormJsonString(const FString& InString)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InString);
		TSharedPtr<FJsonObject> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			FormJsonString(ReadRoot, *this);
		}
	}

	void FormJsonString(const TSharedPtr<FJsonObject> ReadRoot, FJSONTest& InXXX)
	{
		if (ReadRoot.IsValid())
		{
			InXXX.Name = ReadRoot->GetStringField(TEXT("Name"));
			InXXX.Age = ReadRoot->GetIntegerField(TEXT("Age"));
			InXXX.bMarry = ReadRoot->GetBoolField(TEXT("bMarry"));
			InXXX.Fff = ReadRoot->GetNumberField(TEXT("Fff"));
			InXXX.AlienIdentity = ReadRoot->GetStringField(TEXT("AlienIdentity"));

			TSharedPtr<FJsonObject> TestRoot = ReadRoot->GetObjectField(TEXT("Test"));
			{
				InXXX.Test.Index = TestRoot->GetIntegerField(TEXT("Index"));
				InXXX.Test.Indexf = TestRoot->GetNumberField(TEXT("Indexf"));
			}

			TSharedPtr<FJsonObject> TestRootPtr = ReadRoot->GetObjectField(TEXT("TestPtr"));
			{
				InXXX.TestPtr = new FObjectTest();
				InXXX.TestPtr->Index = TestRootPtr->GetIntegerField(TEXT("Index"));
				InXXX.TestPtr->Indexf = TestRootPtr->GetNumberField(TEXT("Indexf"));
			}

			const TArray<TSharedPtr<FJsonValue>>& Arrays = ReadRoot->GetArrayField(TEXT("Array"));
			for (auto& Tmp : Arrays)
			{
				FJSONTest &InObjectXXX = InXXX.Array.AddDefaulted_GetRef();
				FormJsonString(Tmp->AsObject(), InObjectXXX);
			}
		}
	}
};

void AWhatIsJSONGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FJSONTest Object;
	Object.Age = 22;
	Object.AlienIdentity = TEXT("NULL");
	Object.bMarry = false;
	Object.Fff = 10.f;
	Object.Name = TEXT("Smartuil");
	Object.Test.Index = 100;
	Object.Test.Indexf = 1024.f;
	Object.TestPtr = new FObjectTest();
	Object.TestPtr->Index = 123;
	Object.TestPtr->Indexf = 12.f;

	FJSONTest Test = Object;
	Object.Array.Add(Test);
	Object.Array.Add(Test);
	Object.Array.Add(Test);
	FString JsonString = Object.ToString();
	
	UE_LOG(LogTemp, Display, TEXT("JsonString = %s"), *JsonString);
	
	FJSONTest Object2;
	Object2.FormJsonString(JsonString);

	UE_LOG(LogTemp, Display, TEXT("Name = %s"), *Object2.Name);
}
