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
		auto s = FPaths::ProjectDir() / TEXT("Resources");
		this->SetContentRoot(FPaths::ProjectDir() / TEXT("Resources"));
		const FVector2D Icon64x64(64.0f, 64.0f);
		const FVector2D Icon40x40(40.0f, 40.0f);
		const FVector2D Icon16x16(16.0f, 16.0f);
		const FVector2D Icon8x8(8.0f, 8.0f);
		
		const FLinearColor NormalColor(FColor(0xffeff3f3));
		const FLinearColor SelectedColor(FColor(0xffdbe4d5));
		const FLinearColor HoverColor(FColor(0xffdbe4e4));
		const FLinearColor DisabledColor(FColor(0xaaaaaa));
		const FLinearColor TextColor(FColor(0xff2c3e50));

		Set("ToolPanel.GroupBorder", new IMAGE_BRUSH("GroupBorder", Icon64x64));

		Set("ToolPanel.DarkGroupBorder", new BOX_BRUSH("DarkGroupBorder", FMargin( 4.0f / 16.0f )));
		
		Set("Log.TextBox", FEditableTextBoxStyle(FCoreStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
		.SetBackgroundImageNormal( BOX_BRUSH( "WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundImageHovered( BOX_BRUSH( "WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundImageFocused( BOX_BRUSH( "WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundImageReadOnly( BOX_BRUSH( "WhiteGroupBorder", FMargin(4.0f/16.0f) ) )
		.SetBackgroundColor( FLinearColor(0.015f, 0.015f, 0.015f) )
		);
		
		NormalText = FTextBlockStyle()
		.SetFont(DEFAULT_FONT("Regular", FCoreStyle::RegularTextSize))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetHighlightColor( FLinearColor( 0.02f, 0.3f, 0.0f ) )
		.SetHighlightShape( BOX_BRUSH( "TextBlockHighlightShape", FMargin(3.f/8.f) ) );
		
		const int32 LogFontSize = 9;
		
		const FTextBlockStyle NormalLogText = FTextBlockStyle(NormalText)
		.SetFont( DEFAULT_FONT( "Mono", LogFontSize ) )
		.SetColorAndOpacity( FLinearColor(FColor(0xffaaaaaa)) )
		.SetSelectedBackgroundColor( FLinearColor(0, 0, 1) );
		
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
		
		FButtonStyle Button = FButtonStyle()
		.SetNormal( BOX_BRUSH( "Button", FVector2D(32,32), 8.0f/32.0f ) )
		.SetHovered( BOX_BRUSH( "Button_Hovered", FVector2D(32,32), 8.0f/32.0f ) )
		.SetPressed( BOX_BRUSH( "Button_Pressed", FVector2D(32,32), 8.0f/32.0f ) )
		.SetNormalPadding( FMargin( 2,2,2,2 ) )
		.SetPressedPadding( FMargin( 2,3,2,1 ) );
			Set( "Button", Button );
			Set( "Button.Disabled", new BOX_BRUSH( "Button_Disabled", 8.0f/32.0f ) );
		
		Set( "PlacementBrowser.Asset", FButtonStyle( Button )
		.SetNormal( FSlateNoResource() )
		.SetHovered( IMAGE_BRUSH( "Selection", Icon8x8, FLinearColor( 1.0f, 1.0f, 1.0f, 0.1f ) ) )
		.SetPressed( IMAGE_BRUSH( "Selection", Icon8x8, FLinearColor( 0.728f, 0.364f, 0.003f )  ) )
		.SetNormalPadding( 0 )
		.SetPressedPadding( 0 )
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
