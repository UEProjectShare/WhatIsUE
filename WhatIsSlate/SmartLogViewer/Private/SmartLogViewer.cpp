// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "SmartLogViewer.h"
#include "Core.h"
#include "Developer/StandaloneRenderer/Public/StandaloneRenderer.h"
#include "Slate/Public/Framework/Application/SlateApplication.h"
#include "SlateCore/Public/Widgets/DeclarativeSyntaxSupport.h"
#include "SlateCore/Public/Widgets/SWindow.h"
#include "Log/SmartLogViewerLog.h"
#include "RequiredProgramMainCPPInclude.h"
#include "Slate/Public/Widgets/Docking/SDockTab.h"
#include "Slate/Public/Framework/Docking/TabManager.h"
#include "Slate/Public/Framework/Docking/LayoutService.h"
#include "Slate/Public/Widgets/Input/SHyperlink.h"
#include "Slate/Public/Widgets/Input/SMultiLineEditableTextBox.h"
#include "Slate/Public/Widgets/Layout/SBox.h"
#include "LogViewerWidgetMain.h"

IMPLEMENT_APPLICATION(SmartLogViewer, "SmartLogViewer");

#define LOCTEXT_NAMESPACE "SSmartLogViewerWidget"

TSharedPtr<class FTabManager> SmartLogViewerTabManager;
TSharedPtr<FTabManager::FLayout> TabManagerLayout;
TWeakPtr<SDockTab> PluginTab;
TWeakPtr<SDockTab> SpecialTab;
FName TabNameOpenNewTab(TEXT("TabNameOpenNewTab"));
FName TabNameDefaultEngineOutput(TEXT("TabNameDefaultEngineOutput"));
static const FName LogViewerTabName("LogViewer");

bool OnSpecialTabTryClose()
{
	return true;
}

void OnSpecialTabClosed(TSharedRef<SDockTab> Tab)
{
	if (SpecialTab.IsValid())
	{
		//Clear callback so we are not in the loop
		SpecialTab.Pin()->SetOnTabClosed(SDockTab::FOnTabClosedCallback());
	}
	if (PluginTab.IsValid())
	{
		TSharedPtr<SWindow> ParentWindowPtr = PluginTab.Pin()->GetParentWindow();
		ParentWindowPtr->RequestDestroyWindow();
	}
}

TSharedRef<SDockTab> SpawnEmptyLogViewerTab()
{
	TSharedRef<SLogViewerWidgetMain> Widget = SNew(SLogViewerWidgetMain);
	TSharedRef<SDockTab> Tab = SNew(SDockTab)
		.TabRole(ETabRole::DocumentTab)
		.Label(NSLOCTEXT("LogViewer", "EngineOutputTabTitle", "LogDock"))
		[
			Widget
		];
	Widget->SetParentTab(Tab);
	return Tab;
}

void OnOpenNewTabClicked(TSharedRef<SDockTab> DockTab, ETabActivationCause InActivationCause)
{
	if (InActivationCause == UserClickedOnTab)
	{
		SmartLogViewerTabManager->InsertNewDocumentTab(TabNameOpenNewTab, FTabManager::ESearchPreference::PreferLiveTab, SpawnEmptyLogViewerTab());
	}
}

TSharedRef<SDockTab> SpawnSpecialTab()
{
	const FString Link = TEXT("https://www.unrealengine.com/marketplace/en-US/product/log-viewer-pro");
	TSharedRef<SDockTab> NewTab = SNew(SDockTab)
		.TabRole(ETabRole::DocumentTab)
		.OnCanCloseTab(SDockTab::FCanCloseTab::CreateStatic(&OnSpecialTabTryClose))
		.OnTabClosed(SDockTab::FOnTabClosedCallback::CreateStatic(&OnSpecialTabClosed))
		.Label(NSLOCTEXT("LogVeiwer", "NewTabTabTitle", "Create New Tab"))
		.ToolTipText(NSLOCTEXT("LogVeiwer", "NewLogTabTitleToolTip", "Creates new empty log viewer tab"))
		[
			SNew(SBox)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(0.0f, 0.0f, 8.0f, 0.0f))
			[
				SNew(SHyperlink)
				.OnNavigate_Lambda([=]() { FPlatformProcess::LaunchURL(*Link, nullptr, nullptr); })
				.Text(NSLOCTEXT("LogViewer", "LinkToProText", "If you love that plugin, please try LogViewerPro from Epic Marketplace"))
				.ToolTipText(FText::FromString(Link))
			]
		];
	SpecialTab = NewTab;
	NewTab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateStatic(&OnOpenNewTabClicked));
	return NewTab;
}

TSharedRef<SDockTab> OnSpawnTabNewDocument(const FSpawnTabArgs& Args)
{
	return SpawnSpecialTab();
};

void RegisterDefaultLogViewerTab(FTabManager& InTabManager)
{
	const auto SpawnDefaultLogViewerTab = [](const FSpawnTabArgs& Args)
	{
		TSharedRef<SLogViewerWidgetMain> Widget = SNew(SLogViewerWidgetMain);
		TSharedRef<SDockTab> Tab = SNew(SDockTab)
			.TabRole(ETabRole::DocumentTab)
			.Label(NSLOCTEXT("LogViewer", "EngineOutputTabTitle", "LogDock"))
			[
				Widget
			];
		Widget->SetParentTab(Tab);
		return Tab;
	};

	InTabManager.RegisterTabSpawner(TabNameDefaultEngineOutput, FOnSpawnTab::CreateStatic(SpawnDefaultLogViewerTab))
		.SetDisplayName(NSLOCTEXT("LogViewer", "EngineOutputTabTitle", "Engine Output"))
		.SetTooltipText(NSLOCTEXT("LogViewer", "EngineOutputTabTitle", "Engine Output"));
}

TSharedRef<SDockTab> SpawnSizeofTab(const FSpawnTabArgs& Args)
{
	UE_LOG(LogSmartLogViewer, Error, TEXT("Engine Preinit failed."));
	return SNew(SDockTab)
	.TabRole(ETabRole::NomadTab)
	.Label(NSLOCTEXT("LogViewer", "MainTabTitle", "Log Viewer"));
}

TSharedRef<SDockTab> CreateLogViewerPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	UE_LOG(LogSmartLogViewer, Error, TEXT("Engine Preinit failed."));
	// 注册TabSpawner
	TSharedPtr<SDockTab> NomadTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Label(NSLOCTEXT("LogViewer", "MainTabTitle", "Log Viewer"));

	PluginTab = NomadTab;
	
	if (!SmartLogViewerTabManager.IsValid())
	{
		SmartLogViewerTabManager = FGlobalTabmanager::Get()->NewTabManager(NomadTab.ToSharedRef());
	}
	
	TWeakPtr<FTabManager> TabManagerWeak = SmartLogViewerTabManager;
	
	NomadTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateStatic(
		[](TSharedRef<SDockTab> Self, TWeakPtr<FTabManager> TabManager)
		{
			TSharedPtr<FTabManager> OwningTabManager = TabManager.Pin();
			if (OwningTabManager.IsValid())
			{
				FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, OwningTabManager->PersistLayout());
				OwningTabManager->CloseAllAreas();
			}
		}
		, TabManagerWeak
		));
	
	if (!TabManagerLayout.IsValid())
	{
		SmartLogViewerTabManager->RegisterTabSpawner(TabNameOpenNewTab, FOnSpawnTab::CreateStatic(OnSpawnTabNewDocument));
		RegisterDefaultLogViewerTab(*SmartLogViewerTabManager);

		TabManagerLayout = FTabManager::NewLayout("LogViewerLayout")
			->AddArea
			(
				FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(.4f)
					->SetHideTabWell(false)
					->AddTab(TabNameOpenNewTab, ETabState::OpenedTab)
					->AddTab(TabNameDefaultEngineOutput, ETabState::OpenedTab)
					->SetForegroundTab(TabNameDefaultEngineOutput)
				)
			);
	}

	TSharedRef<SWidget> TabContents = SmartLogViewerTabManager->RestoreFrom(TabManagerLayout.ToSharedRef(), TSharedPtr<SWindow>()).ToSharedRef();

	NomadTab->SetContent(
		TabContents
	);
	return NomadTab.ToSharedRef();
}

void InitApp()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(LogViewerTabName, FOnSpawnTab::CreateStatic(&CreateLogViewerPluginTab))
	.SetDisplayName(LOCTEXT("FLogViewerTabTitle", "LogViewer"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
	
	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("SmartLog")
		->AddArea
		(
			FTabManager::NewArea(1920,1080)
			->SetWindow(FVector2D(420, 10), false)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(LogViewerTabName, ETabState::OpenedTab)
			)
		);

	//通过布局文件创建MainWindow
	FGlobalTabmanager::Get()->RestoreFrom(Layout, TSharedPtr<SWindow>());
}

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR, _In_ int nCmdShow)
{
	// 启动引擎的主循环，解析命令行
	int32 result = GEngineLoop.PreInit(GetCommandLineW());
	if (result != 0)
	{
		UE_LOG(LogSmartLogViewer, Error, TEXT("Engine Preinit failed."));
		return -1;
	}

	// 如果Cmd中没有指定opengl，则使用FSlateD3DRenderer渲染器
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());
	FFeatureStyle::Get();
	InitApp();
	
	while(!IsEngineExitRequested())
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FStats::AdvanceFrame(false);
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FSlateApplication::Get().PumpMessages();
		FSlateApplication::Get().Tick();
		FPlatformProcess::Sleep(0);
	}
	
	FCoreDelegates::OnExit.Broadcast();
	FSlateApplication::Shutdown();
	FModuleManager::Get().UnloadModulesAtShutdown();
	return 0;
}
