// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

class FFeatureStyle : public FSlateStyleSet
{
public:
	FTextBlockStyle NormalText;
	
	FFeatureStyle() : FSlateStyleSet("FeatureStyle")
	{
		this->SetContentRoot(FPaths::EngineDir() / TEXT("Source/Programs/SmartLogViewer/Resources"));
		const FVector2D Icon64x64(64.0f, 64.0f);
		const FVector2D Icon40x40(40.0f, 40.0f);
		const FVector2D Icon16x16(16.0f, 16.0f);
		
		const FLinearColor NormalColor(FColor(0xffeff3f3));
		const FLinearColor SelectedColor(FColor(0xffdbe4d5));
		const FLinearColor HoverColor(FColor(0xffdbe4e4));
		const FLinearColor DisabledColor(FColor(0xaaaaaa));
		const FLinearColor TextColor(FColor(0xff2c3e50));

		Set("ToolPanel.GroupBorder", new IMAGE_BRUSH("GroupBorder", Icon64x64));

		Set("ToolPanel.DarkGroupBorder", new IMAGE_BRUSH("DarkGroupBorder", Icon64x64));
		
		Set("Log.TextBox", FEditableTextBoxStyle(FCoreStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
		.SetBackgroundImageNormal( BOX_BRUSH( "Common/WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundImageHovered( BOX_BRUSH( "Common/WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundImageFocused( BOX_BRUSH( "Common/WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundImageReadOnly( BOX_BRUSH( "Common/WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundColor( FLinearColor(FColor(0x040404FF)) )
		);
		
		NormalText = FTextBlockStyle()
		.SetFont(DEFAULT_FONT("Regular", FCoreStyle::RegularTextSize))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetHighlightColor( FLinearColor( 0.02f, 0.3f, 0.0f ) )
		.SetHighlightShape( BOX_BRUSH( "Common/TextBlockHighlightShape", FMargin(3.f/8.f) ) );
		
		const int32 LogFontSize = 9;
		
		const FTextBlockStyle NormalLogText = FTextBlockStyle(NormalText)
		.SetFont( DEFAULT_FONT( "Mono", LogFontSize ) )
		.SetColorAndOpacity( FLinearColor(FColor(0xffaaaaaa)) )
		.SetSelectedBackgroundColor( FLinearColor(FColor(0x020202FF)) );
		
		Set("Log.Normal", NormalLogText );

		Set("Log.Command", FTextBlockStyle(NormalLogText)
		.SetColorAndOpacity( FLinearColor(FColor(0xff33dd33)) )
		);

		Set("Log.Warning", FTextBlockStyle(NormalLogText)
			.SetColorAndOpacity( FLinearColor(FColor(0xEBB000FF)) )
			);

		Set("Log.Error", FTextBlockStyle(NormalLogText)
			.SetColorAndOpacity( FLinearColor(FColor(0xFF0D0FFF)) )
			);
		
		FSlateStyleRegistry::RegisterSlateStyle(*this);
	}
	
	~FFeatureStyle()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*this);
	}
	
	static const ISlateStyle& Get()
	{
		if (!Style.IsValid())
		{
			Style = MakeShareable(new FFeatureStyle());
		}
		return *Style;
	}
private:
	static TSharedPtr< FSlateStyleSet > Style;
};
TSharedPtr< FSlateStyleSet >  FFeatureStyle::Style = nullptr;
#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
