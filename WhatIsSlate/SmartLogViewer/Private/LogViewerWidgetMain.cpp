#include "LogViewerWidgetMain.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "LogViewerWidgetCategoriesView.h"
#include "LogViewerWidgetTopBar.h"
#include "OutputLogTextLayoutMarshaller.h"
#include "SmartLogViewerStyle.h"
#include "Editor/EditorStyle/Public/EditorStyle.h"
#include "Widgets/Input/SSearchBox.h"

#define LOCTEXT_NAMESPACE "SLogViewerWidgetMain"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SLogViewerWidgetMain::Construct( const FArguments& InArgs )
{
	CategoryMenu = SNew(SLogViewerWidgetCategoriesView)
		.MainWidget(this);

	MessagesTextMarshaller = FOutputLogTextLayoutMarshaller::Create(InArgs._Messages, &CategoryMenu->Filter);

	MessagesTextBox = SNew(SMultiLineEditableTextBox)
		.Style(FFeatureStyle::Get(), "Log.TextBox")
		.TextStyle(FFeatureStyle::Get(), "Log.Normal")
		.ForegroundColor(FLinearColor::Blue)
		.Marshaller(MessagesTextMarshaller)
		.OnVScrollBarUserScrolled(this, &SLogViewerWidgetMain::OnUserScrolled)
		.IsReadOnly(true)
		.AlwaysShowScrollbars(true);

	TopBar = SNew(SLogViewerWidgetTopBar)
		.MainWidget(this)
		.CategoryMenu(CategoryMenu.Get());
	//.OnVScrollBarUserScrolled(this, &SLogViewerWidgetMain::OnUserScrolled);
	//.ContextMenuExtender(this, &SLogViewerWidgetMain::ExtendTextBoxMenu);

	ChildSlot
	[
		SNew(SBorder)
		.Padding(3)
		//.BorderImage(FFeatureStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)

			// Output Log Filter
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0)
			.FillHeight(1.0)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.FillWidth(0.8)
				[
					SNew(SVerticalBox)
					// Output Log Filter
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0)
					[
						TopBar.ToSharedRef()
					]

					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0)
					.FillHeight(1.0)
					[
						MessagesTextBox.ToSharedRef()
					]
				]
			
				+SHorizontalBox::Slot()
				.FillWidth(0.2)
				[
					CategoryMenu.ToSharedRef()
				]
			]
		]
	];
}

void SLogViewerWidgetMain::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Display, TEXT("OnDragEnter"));
	//这里就可以按照想要的方式来获取了。
	TSharedPtr<FExternalDragOperation> Op = DragDropEvent.GetOperationAs<FExternalDragOperation>();
	const TArray<FString>& Files = Op->GetFiles();  //得到拖入的文件列表
	if (Files.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Files:%s"), *Files[0]);
		TopBar->AnalysisFile(*Files[0]);
	}
}

FReply SLogViewerWidgetMain::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedRef<FDragDropOperation> DragDropOperation = MakeShareable(new FDragDropOperation);
	return FReply::Handled().BeginDragDrop(DragDropOperation);
}

FReply SLogViewerWidgetMain::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Display, TEXT("OnDrop"));
	return FReply::Handled();
}

SLogViewerWidgetMain::~SLogViewerWidgetMain()
{

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLogViewerWidgetMain::HandleNewLogMessageReceived(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category)
{
	CategoryMenu->AddCategory(Category);
	MessagesTextMarshaller->AppendMessage(V, Verbosity, Category);
	
	if (bIsFollowLog)
	{
		MessagesTextBox->ScrollTo(FTextLocation(MessagesTextMarshaller->GetNumFilteredMessages() - 1));
	}
}

void SLogViewerWidgetMain::BeginFollowLog()
{
	bIsFollowLog = true;
	MessagesTextBox->ScrollTo(FTextLocation(MessagesTextMarshaller->GetNumFilteredMessages() - 1));
}

void SLogViewerWidgetMain::EndFollowLog()
{
	bIsFollowLog = false;
}

bool SLogViewerWidgetMain::IsFollowLog() const
{
	return bIsFollowLog;
}

void SLogViewerWidgetMain::OnUserScrolled(float ScrollOffset)
{
	const bool bIsUserScrolled = ScrollOffset < 1.0 && !FMath::IsNearlyEqual(ScrollOffset, 1.0f);
	if (bIsUserScrolled)
	{
		EndFollowLog();
	}
}

void SLogViewerWidgetMain::Refresh()
{
	MessagesTextMarshaller->MarkMessagesCacheAsDirty();
	CategoryMenu->MarkDirty();

	// Re-count messages if filter changed before we refresh
	MessagesTextMarshaller->CountMessages();

	MessagesTextBox->GoTo(FTextLocation(0));
	MessagesTextMarshaller->MakeDirty();
	MessagesTextBox->Refresh();

	//RequestForceScroll();
}

void SLogViewerWidgetMain::Cleanup()
{
	MessagesTextMarshaller->ClearMessages();
	CategoryMenu->ClearCategories();
	Refresh();
}

void SLogViewerWidgetMain::SetFocusToFilterEditBox()
{
	FSlateApplication::Get().SetUserFocus(FSlateApplication::Get().GetUserIndexForKeyboard(), TopBar->FilterTextBox->AsShared());
}

void SLogViewerWidgetMain::SetFocusToFilterCategoryBox()
{
	FSlateApplication::Get().SetUserFocus(FSlateApplication::Get().GetUserIndexForKeyboard(), CategoryMenu->SearchBoxPtr->AsShared());
}

FReply SLogViewerWidgetMain::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.IsControlDown())
	{
		if (InKeyEvent.GetKey() == EKeys::O)
		{
			TopBar->OpenLog();
			return FReply::Handled();
		}

		if (InKeyEvent.GetKey() == EKeys::F)
		{
			SetFocusToFilterEditBox();
			return FReply::Handled();
		}
	}

	if (InKeyEvent.IsAltDown())
	{
		if (InKeyEvent.GetKey() == EKeys::C)
		{
			SetFocusToFilterCategoryBox();
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE