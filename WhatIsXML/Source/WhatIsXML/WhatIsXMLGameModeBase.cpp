// Copyright Epic Games, Inc. All Rights Reserved.


#include "WhatIsXMLGameModeBase.h"
#include "XmlParser.h"

struct FObjectTest
{
	int32 Index;
	float Indexf;

};

struct FXMLTest
{
	FXMLTest()
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
	TArray<FXMLTest> Array;
	
	void ToXML(const FXMLTest &InXXX, FXmlNode* InRootNode)
	{
		InRootNode->AppendChildNode(TEXT("Name"), InXXX.Name);
		InRootNode->AppendChildNode(TEXT("Age"), FString::FromInt(InXXX.Age));
		InRootNode->AppendChildNode(TEXT("bMarry"), FString::FromInt(InXXX.bMarry));
		InRootNode->AppendChildNode(TEXT("Fff"), FString::SanitizeFloat((double)InXXX.Fff));
		InRootNode->AppendChildNode(TEXT("AlienIdentity"), InXXX.AlienIdentity);
		InRootNode->AppendChildNode(TEXT("Test"), "");
		{
			if (FXmlNode* TestNode = InRootNode->FindChildNode(TEXT("Test")))
			{
				TestNode->AppendChildNode(TEXT("Index"), FString::FromInt(InXXX.Test.Index));
				TestNode->AppendChildNode(TEXT("Indexf"), FString::SanitizeFloat((double)InXXX.Test.Indexf));
			}
		}
		
		InRootNode->AppendChildNode(TEXT("TestPtr"), "");
		{
			if (FXmlNode* TestPtrNode = InRootNode->FindChildNode(TEXT("TestPtr")))
			{
				TestPtrNode->AppendChildNode(TEXT("Index"), FString::FromInt(InXXX.TestPtr->Index));
				TestPtrNode->AppendChildNode(TEXT("Indexf"), FString::SanitizeFloat((double)InXXX.TestPtr->Indexf));
			}
		}

		for (int32 i =0;i < InXXX.Array.Num();i++)
		{
			const FString Key = TEXT("Child_") + FString::FromInt(i);
			InRootNode->AppendChildNode(Key, "");
			if (FXmlNode* InNode = InRootNode->FindChildNode(Key))
			{
				ToXML(InXXX.Array[i], InNode);
			}
		}
	}

	void ToSave()
	{
		FString Buffer = TEXT("<XXXX>\n</XXXX>");
		FXmlFile* XmlFile = new FXmlFile(Buffer, EConstructMethod::ConstructFromBuffer);
		if (FXmlNode* RootNode = XmlFile->GetRootNode())
		{
			ToXML(*this, RootNode);
			
			XmlFile->Save(FPaths::ProjectDir() / TEXT("XMLTest.xml"));
		}

		delete XmlFile;
	}

	void FormXMLStringPaths(FXMLTest& InFXXX, FXmlNode* InRootNode)
	{
		auto GetValue_String = [&](const FString& InTag, FString& OutValue)
		{
			if (FXmlNode* NameNode = InRootNode->FindChildNode(InTag))
			{
				OutValue = NameNode->GetContent();
			}
		};
		auto GetValue_Int = [&](const FString& InTag, int32& OutValue)
		{
			if (FXmlNode* NameNode = InRootNode->FindChildNode(InTag))
			{
				OutValue = FCString::Atoi(*NameNode->GetContent());
			}
		};

		auto GetValue_Float = [&](const FString& InTag, float& OutValue)
		{
			if (FXmlNode* NameNode = InRootNode->FindChildNode(InTag))
			{
				OutValue = FCString::Atof(*NameNode->GetContent());
			}
		};

		auto GetValue_Bool = [&](const FString& InTag, bool& OutValue)
		{
			if (FXmlNode* NameNode = InRootNode->FindChildNode(InTag))
			{
				OutValue = NameNode->GetContent().ToBool();
			}
		};

		auto GetValue_ObjectTest = [&](const FString& InTag, FObjectTest& OutValue)
		{
			if (FXmlNode* NameNode = InRootNode->FindChildNode(InTag))
			{
				if (FXmlNode* NameNodeIndex = NameNode->FindChildNode(TEXT("Index")))
				{
					OutValue.Index = FCString::Atoi(*NameNodeIndex->GetContent());
				}

				if (FXmlNode* NameNodeIndexf = NameNode->FindChildNode(TEXT("Indexf")))
				{
					OutValue.Indexf = FCString::Atof(*NameNodeIndexf->GetContent());
				}
			}
		};

		if (FXmlNode* TestPtrNode = InRootNode->FindChildNode(TEXT("TestPtr")))
		{
			InFXXX.TestPtr = new FObjectTest();
		}

		GetValue_String(TEXT("Name"), InFXXX.Name);
		GetValue_Int(TEXT("Age"), InFXXX.Age);
		GetValue_Bool(TEXT("bMarry"), InFXXX.bMarry);
		GetValue_Float(TEXT("Fff"), InFXXX.Fff);
		GetValue_String(TEXT("AlienIdentity"), InFXXX.AlienIdentity);
		GetValue_ObjectTest(TEXT("Test"), InFXXX.Test);
		GetValue_ObjectTest(TEXT("TestPtr"), *InFXXX.TestPtr);

		for (int32 i = 0 ;;i++)
		{
			if (FXmlNode* InChildNode = InRootNode->FindChildNode(TEXT("Child_") + FString::FromInt(i)))
			{
				FXMLTest &InMyXXX = InFXXX.Array.AddDefaulted_GetRef();
				FormXMLStringPaths(InMyXXX, InChildNode);
			}
			else
			{
				break;
			}
		}
		
	}

	void FormXMLStringPaths(const FString& InPath)
	{
		FXmlFile* XmlFile = new FXmlFile(InPath, EConstructMethod::ConstructFromFile);
		if (FXmlNode* RootNode = XmlFile->GetRootNode())
		{
			FormXMLStringPaths(*this, RootNode);
		}

		delete XmlFile;
	}

};

void AWhatIsXMLGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FXMLTest Object;
	Object.Age = 22;
	Object.AlienIdentity = TEXT("nullptr");
	Object.bMarry = false;
	Object.Fff = 10.f;
	Object.Name = TEXT("Smartuil");
	Object.Test.Index = 100;
	Object.Test.Indexf = 1023.f;
	Object.TestPtr = new FObjectTest();
	Object.TestPtr->Index = 123;
	Object.TestPtr->Indexf = 12.f;

	FXMLTest Test = Object;
	Object.Array.Add(Test);
	Object.Array.Add(Test);
	Object.Array.Add(Test);

	Object.ToSave();


	FXMLTest Object2;
	Object2.FormXMLStringPaths(FPaths::ProjectDir() / TEXT("XMLTest.xml"));
	if (Object2.Name.Len())
	{
		UE_LOG(LogTemp, Display, TEXT("Object2.Name = %s"), *Object2.Name);
	}
}
