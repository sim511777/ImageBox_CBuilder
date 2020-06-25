//---------------------------------------------------------------------------

#pragma hdrstop

#include "ImageBoxOption.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void TImageBoxOption::FromImageBox(TImageBox* pbx) {
    UseDrawPixelValue = pbx->UseDrawPixelValue;
    UseDrawInfo = pbx->UseDrawInfo;
    UseDrawCenterLine = pbx->UseDrawCenterLine;
    UseDrawDrawTime = pbx->UseDrawDrawTime;
    UseInterPorlation = pbx->UseInterPorlation;
    UseParallel = pbx->UseParallel;
    PixelValueDispZoomFactor = pbx->PixelValueDispZoomFactor;
    UseMouseMove = pbx->UseMouseMove;
    UseMouseWheelZoom = pbx->UseMouseWheelZoom;

    DrawFontName = pbx->DrawFont->Name;
    DrawFontSize = pbx->DrawFont->Size;

	PixelValueDispFontName = pbx->PixelValueDispFont->Name;
    PixelValueDispFontSize = pbx->PixelValueDispFont->Size;

	BgColor = pbx->BgColor;
    UseMousePanClamp = pbx->UseMousePanClamp;
    ZoomLevelMin = pbx->ZoomLevelMin;
    ZoomLevelMax = pbx->ZoomLevelMax;
}
//---------------------------------------------------------------------------

void TImageBoxOption::ToImageBox(TImageBox* pbx) {
    pbx->UseDrawPixelValue = UseDrawPixelValue;
    pbx->UseDrawInfo = UseDrawInfo;
    pbx->UseDrawCenterLine = UseDrawCenterLine;
    pbx->UseDrawDrawTime = UseDrawDrawTime;
    pbx->UseInterPorlation = UseInterPorlation;
    pbx->UseParallel = UseParallel;
    pbx->PixelValueDispZoomFactor = PixelValueDispZoomFactor;
    pbx->UseMouseMove = UseMouseMove;
    pbx->UseMouseWheelZoom = UseMouseWheelZoom;

    pbx->DrawFont->Name = DrawFontName;
    pbx->DrawFont->Size = DrawFontSize;

	pbx->PixelValueDispFont->Name = PixelValueDispFontName;
    pbx->PixelValueDispFont->Size = PixelValueDispFontSize;

	pbx->BgColor = BgColor;
    pbx->UseMousePanClamp = UseMousePanClamp;
    pbx->ZoomLevelMin = ZoomLevelMin;
    pbx->ZoomLevelMax = ZoomLevelMax;
}
//---------------------------------------------------------------------------