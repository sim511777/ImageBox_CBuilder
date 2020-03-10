//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ImageBox.h"
#include <math.h>
#include <DateUtils.hpp>
#include <Dialogs.hpp>
#include "FrmAbout.h"

//---------------------------------------------------------------------------
template<class T>
T Clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

//---------------------------------------------------------------------------
template<class T>
T Min(T a, T b)
{
    if (a < b) return a;
    return b;
}

//---------------------------------------------------------------------------
template<class T>
T Max(T a, T b)
{
    if (a > b) return a;
    return b;
}

//---------------------------------------------------------------------------
double GetTimeMs() {
    static LARGE_INTEGER freq;
    static BOOL r = QueryPerformanceFrequency(&freq);
    LARGE_INTEGER cnt;
    QueryPerformanceCounter(&cnt);
    return cnt.QuadPart * 1000.0 / freq.QuadPart;
}

//---------------------------------------------------------------------------
int TColorToBGRA(TColor color) {
    int r = color & 0xff;
    int g = (color >> 8) & 0xff;
    int b = (color >> 16) & 0xff;
    return (b | g << 8 | r << 16 | 0xff << 24);
}

//---------------------------------------------------------------------------
void CopyImageBufferZoom(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel) {
    // 인덱스 버퍼 생성
    int* siys = new int[dbh];
    int* sixs = new int[dbw];
    for (int y = 0; y < dbh; y++) {
        int siy = (int)floor((y - pany) / zoom);
        siys[y] = (sbuf == NULL || siy < 0 || siy >= sbh) ? -1 : siy;
    }
    for (int x = 0; x < dbw; x++) {
        int six = (int)floor((x - panx) / zoom);
        sixs[x] = (sbuf == NULL || six < 0 || six >= sbw) ? -1 : six;
    }

    // dst 범위만큼 루프를 돌면서 해당 픽셀값 쓰기
    for (int y = 0; y < dbh; y++) {
        int siy = siys[y];
        BYTE* sptr = (BYTE*)sbuf + (INT64)sbw * siy * bytepp;
        int* dp = (int*)dbuf->ScanLine[y];
        for (int x = 0; x < dbw; x++, dp++) {
            int six = sixs[x];
            if (siy == -1 || six == -1) {   // out of boundary of image
                *dp = bgColor;
            } else {
                BYTE* sp = &sptr[six * bytepp];
                if (bytepp == 1) {          // 8bit gray
                    int v = sp[0];
                    *dp = v | v << 8 | v << 16 | 0xff << 24;
                } else if (bytepp == 2) {   // 16bit gray (*.hra)
                    int v = sp[0];
                    *dp = v | v << 8 | v << 16 | 0xff << 24;
                } else if (bytepp == 3) {   // 24bit bgr
                    *dp = sp[0] | sp[1] << 8 | sp[2] << 16 | 0xff << 24;
                } else if (bytepp == 4) {   // 32bit bgra
                    *dp = sp[0] | sp[1] << 8 | sp[2] << 16 | 0xff << 24;
                }
            }
        }
    }

	delete[] siys;
	delete[] sixs;
}

//---------------------------------------------------------------------------
void CopyImageBufferZoomIpl(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel) {
	// 인덱스 버퍼 생성
	int* siy0s = new int[dbh];
	int* siy1s = new int[dbh];
	int* six0s = new int[dbw];
	int* six1s = new int[dbw];
	int* sity0s = new int[dbh];
	int* sity1s = new int[dbh];
	int* sitx0s = new int[dbw];
	int* sitx1s = new int[dbw];
	for (int y = 0; y < dbh; y++) {
		double siy = (y + 0.5 - pany) / zoom - 0.5;
		if (sbuf == NULL || siy < -0.5 || siy >= sbh -0.5) {
			siy0s[y] = -1;
			continue;
		}
		int siy0 = (int)floor(siy);
		int siy1 = siy0 + 1;
		sity0s[y] = (int)((siy1 - siy) * 256);
		sity1s[y] = (int)((siy - siy0) * 256);
		siy0s[y] = Clamp(siy0, 0, sbh - 1);
		siy1s[y] = Clamp(siy1, 0, sbh - 1);
	}
	for (int x = 0; x < dbw; x++) {
		double six = (x + 0.5 - panx) / zoom - 0.5;
		if (sbuf == NULL || six < -0.5 || six >= sbw - 0.5) {
			six0s[x] = -1;
			continue;
		}
		int six0 = (int)floor(six);
		int six1 = six0 + 1;
		sitx0s[x] = (int)((six1 - six) * 256);
		sitx1s[x] = (int)((six - six0) * 256);
		six0s[x] = Clamp(six0, 0, sbw - 1);
		six1s[x] = Clamp(six1, 0, sbw - 1);
	}

	// dst 범위만큼 루프를 돌면서 해당 픽셀값 쓰기
	for (int y = 0; y < dbh; y++) {
		int siy0 = siy0s[y];
		int siy1 = siy1s[y];
		BYTE* sptr0 = (BYTE*)sbuf + (INT64)sbw * siy0 * bytepp;
		BYTE* sptr1 = (BYTE*)sbuf + (INT64)sbw * siy1 * bytepp;
        int* dp = (int*)dbuf->ScanLine[y];
		int ty0 = sity0s[y];
		int ty1 = sity1s[y];
		for (int x = 0; x < dbw; x++, dp++) {
			int six0 = six0s[x];
			int six1 = six1s[x];
			if (siy0 == -1 || six0 == -1) {   // out of boundary of image
				*dp = bgColor;
			} else {
				BYTE* sp00 = &sptr0[six0 * bytepp];
				BYTE* sp01 = &sptr0[six1 * bytepp];
				BYTE* sp10 = &sptr1[six0 * bytepp];
				BYTE* sp11 = &sptr1[six1 * bytepp];
				int tx0 = sitx0s[x];
				int tx1 = sitx1s[x];
				int t00 = ty0 * tx0;
				int t01 = ty0 * tx1;
				int t10 = ty1 * tx0;
				int t11 = ty1 * tx1;
				if (bytepp == 1) {          // 8bit gray
					int v = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					*dp = v | v << 8 | v << 16 | 0xff << 24;
				} else if (bytepp == 2) {   // 16bit gray (*.hra)
					int v = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					*dp = v | v << 8 | v << 16 | 0xff << 24;
				} else if (bytepp == 3) {   // 24bit bgr
					int b = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					int g = (sp00[1] * t00 + sp01[1] * t01 + sp10[1] * t10 + sp11[1] * t11) >> 16;
					int r = (sp00[2] * t00 + sp01[2] * t01 + sp10[2] * t10 + sp11[2] * t11) >> 16;
					*dp = b | g << 8 | r << 16 | 0xff << 24;
				} else if (bytepp == 4) {   // 32bit bgra
					int b = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					int g = (sp00[1] * t00 + sp01[1] * t01 + sp10[1] * t10 + sp11[1] * t11) >> 16;
					int r = (sp00[2] * t00 + sp01[2] * t01 + sp10[2] * t10 + sp11[2] * t11) >> 16;
					*dp = b | g << 8 | r << 16 | 0xff << 24;
				}
			}
		}
	}

	delete[] siy0s;
	delete[] siy1s;
	delete[] six0s;
	delete[] six1s;
	delete[] sity0s;
	delete[] sity1s;
	delete[] sitx0s;
	delete[] sitx1s;
}

//---------------------------------------------------------------------------
const AnsiString TImageBox::VersionHistory =
"ImageBox C++Builder 컨트롤\r\n"
"\r\n"
"v1.0.0.10 - 20200310\r\n"
"1. 개별 픽셀 표시 폰트와 일반 표시 폰트 두가지 따로 갈것\r\n"
"2. pseudo 기본 컬러 더 잘보이는 것으로 수정\r\n"
"\r\n"
"v1.0.0.9 - 20200305\r\n"
"1. 더블클릭 정보창 띄운 후 이미지 이동하는 버그 수정\r\n"
"\r\n"
"v1.0.0.8 - 20200304\r\n"
"1. 버전정보 창에 속성 변경기능 추가\r\n"
"2. 쿼드클릭 대신 ctrl + 더블클릭 누를때 버전정보 창 띄움\r\n"
"3. ShowAbout() 함수 추가\r\n"
"\r\n"
"v1.0.0.7 - 20200217\r\n"
"1. DrawInfo() 깜빡이 않게 더블버퍼 처리\r\n"
"\r\n"
"v1.0.0.6 - 20200214\r\n"
"1. DrawInfo() immediate로 바꾸면서 이전 글자와 겹치는 문제 수정\r\n"
"2. PixelValueDispZoomFactor 속성 추가\r\n"
"3. DrawCenterLine 벗어나는거 그리지 않도록 수정\r\n"
"4. immediate 드로잉 마우스 Move시에 안지워지도록 수정\r\n"
"5. ImgToDisp / DrawCenterLine overflow 에러 처리\r\n"
"6. OnPaint에서 원그리는 코드 주석 처리\r\n"
"7. DrawInfo 에서 panX, panY 표시하는 문제 수정\r\n"
"8. WinMain 함수 리턴값 int\r\n"
"9. 타입에러 수정 / 캐스팅 에러 수정 / CB6 프로젝트 추가\r\n"
"10. initial upload\r\n";

//---------------------------------------------------------------------------
__fastcall TImageBox::TImageBox(TComponent* Owner) : TCustomControl(Owner)
{
    imgBW = 0;
    imgBH = 0;
    imgBuf = NULL;
    imgBytepp = 1;
    mouseDown = FALSE;
    mouseDblClicked = FALSE;
    FUseDrawPixelValue = TRUE;
    FUseDrawInfo = TRUE;
    FUseDrawCenterLine = TRUE;
    FUseDrawDrawTime = FALSE;
    FUseInterPorlation = FALSE;
    FUseParallel = FALSE;
    FUseMouseMove = TRUE;
    FUseMouseWheelZoom = TRUE;
    FZoomLevel = 0;
    FPixelValueDispZoomFactor = 20;
    PanX = 0;
    PanY = 0;
    FPixelValueDispFont = new TFont();
    FPixelValueDispFont->Name = "MS Sans Serif";
    FPixelValueDispFont->Size = 8;

    dispBmp = new Graphics::TBitmap;
    dispBmp->PixelFormat = pf32bit;

    DoubleBuffered = true;
}

//---------------------------------------------------------------------------
__fastcall TImageBox::~TImageBox()
{
    delete dispBmp;
    delete FPixelValueDispFont;
}

//---------------------------------------------------------------------------
void TImageBox::GetZoomFactorComponents(int* exp_num, int* c)
{
    *exp_num = (ZoomLevel >= 0) ? ZoomLevel / 2 : (ZoomLevel - 1) / 2;
    if (ZoomLevel % 2 != 0)
        (*exp_num)--;
    *c = (ZoomLevel % 2 != 0) ? 3 : 1;
}

//---------------------------------------------------------------------------
double TImageBox::GetZoomFactor()
{
    int exp_num, c;
    GetZoomFactorComponents(&exp_num, &c);
    return c * pow(2.0, exp_num);
}

//---------------------------------------------------------------------------
AnsiString TImageBox::GetZoomText()
{
    int exp_num, c;
    GetZoomFactorComponents(&exp_num, &c);
    return (exp_num >= 0) ? IntToStr(c * (int)pow(2.0, exp_num)) : IntToStr(c) + "/" + IntToStr((int)pow(2.0, -exp_num));
}

//---------------------------------------------------------------------------
void TImageBox::SetImgBuf(BYTE* buf, int bw, int bh, int bytepp, BOOL bInvalidate)
{
    imgBuf = buf;
    imgBW = bw;
    imgBH = bh;
    imgBytepp = bytepp;
    if (bInvalidate)
        Invalidate();
}

//---------------------------------------------------------------------------
void TImageBox::ZoomToRect(int x, int y, int width, int height) {
    double scale1 = (double)ClientWidth / width;
    double scale2 = (double)ClientHeight / height;
    double wantedZoomFactor = Min(scale1, scale2);
    ZoomLevel = Clamp((int)floor(log(wantedZoomFactor) / log(sqrt(2.0))), -40, 40);
    double ZoomFactor = GetZoomFactor();
    PanX = (ClientWidth - width * ZoomFactor) / 2 - x * ZoomFactor;
    PanY = (ClientHeight - height * ZoomFactor) / 2 - y * ZoomFactor;
}

//---------------------------------------------------------------------------
void TImageBox::ZoomReset() {
    ZoomLevel = 0;
    PanX = 0;
    PanY = 0;
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::Resize()
{
    ResizeDispBuf();
    TCustomControl::Resize();
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::Paint()
{
    double t0 = GetTimeMs();

    int bgra = TColorToBGRA(FColor);
    if (UseInterPorlation)
        CopyImageBufferZoomIpl(ImgBuf, ImgBW, ImgBH, dispBmp, dispBmp->Width, dispBmp->Height, (INT64)PanX, (INT64)PanY, GetZoomFactor(), ImgBytepp, bgra, UseParallel);
    else
        CopyImageBufferZoom(ImgBuf, ImgBW, ImgBH, dispBmp, dispBmp->Width, dispBmp->Height, (INT64)PanX, (INT64)PanY, GetZoomFactor(), ImgBytepp, bgra, UseParallel);
    double t1 = GetTimeMs();

    Canvas->Font->Name = Font->Name;
    Canvas->Font->Size = Font->Size;
    Canvas->Draw(0, 0, dispBmp);
    double t2 = GetTimeMs();

    if (UseDrawPixelValue)
        DrawPixelValue();
    double t3 = GetTimeMs();

    if (UseDrawCenterLine)
        DrawCenterLine();
    double t4 = GetTimeMs();

    TCustomControl::Paint();
    if (FOnPaint)
        FOnPaint(this);
    double t5 = GetTimeMs();

    if (UseDrawInfo)
        DrawInfo();
    double t6 = GetTimeMs();

    if (UseDrawDrawTime) {
        AnsiString imgInfo = (ImgBuf == NULL) ? AnsiString("X") : AnsiString().sprintf("%d*%d*%dbpp", ImgBW, ImgBH, ImgBytepp*8);
        AnsiString info = AnsiString().sprintf(
"== Image == \n"
"%s\n"
"\n"
"== Draw option ==\n"
"UseDrawPixelValue : %s\n"
"UseDrawInfo : %s\n"
"UseDrawCenterLine : %s\n"
"UseDrawDrawTime : %s\n"
"UseInterPorlation : %s\n"
"UseParallel : %s\n"
"\n"
"== Mouse Option ==\n"
"MouseMove : %s\n"
"MouseWheelZoom : %s\n"
"\n"
"== Draw Time ==\n"
"CopyImage : %.1fms\n"
"DrawImage : %.1fms\n"
"PixelValue : %.1fms\n"
"CenterLine : %.1fms\n"
"OnPaint : %.1fms\n"
"CursorInfo : %.1fms\n"
"Total : %.1fms"
        , imgInfo.c_str()
        , UseDrawPixelValue ? "O" : "X"
        , UseDrawInfo ? "O" : "X"
        , UseDrawCenterLine ? "O" : "X"
        , UseDrawDrawTime ? "O" : "X"
        , UseInterPorlation ? "O" : "X"
        , UseParallel ? "O" : "X"
        , UseMouseMove ? "O" : "X"
        , UseMouseWheelZoom ? "O" : "X"
        , t1-t0
        , t2-t1
        , t3-t2
        , t4-t3
        , t5-t4
        , t6-t5
        , t6-t0
        );
        DrawDrawTime(info);
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseWheelHandler(Messages::TMessage &Message) {
    TCustomControl::MouseWheelHandler(Message);
    if (!UseMouseWheelZoom)
        return;

    TWMMouseWheel mw = *(TWMMouseWheel*)&Message;
    TPoint MousePos = this->ScreenToClient(TPoint(mw.XPos, mw.YPos));
    int WheelDelta = mw.WheelDelta;
    TShiftState ShiftState = *(TShiftState*)&mw.Keys;

    if (ShiftState.Contains(ssShift)) {
        WheelScroll(WheelDelta, MousePos, true);
    } else if (ShiftState.Contains(ssCtrl)) {
        WheelScroll(WheelDelta, MousePos, false);
    } else {
        bool fixPanning = (ShiftState.Contains(ssAlt));
        WheelZoom(WheelDelta, MousePos, fixPanning);
    }
    Invalidate();
}

//---------------------------------------------------------------------------
void TImageBox::WheelScroll(int WheelDelta, TPoint MousePos, BOOL vertical)
{
    int scroll = (WheelDelta > 0) ? 128 : -128;
    if (vertical)
        PanY += scroll;
    else
        PanX += scroll;
}

//---------------------------------------------------------------------------
void TImageBox::WheelZoom(int WheelDelta, TPoint MousePos, BOOL fixPanning)
{
    double zoomFactorOld = GetZoomFactor();
    ZoomLevel = Clamp((WheelDelta > 0) ? (ZoomLevel + 1) : (ZoomLevel - 1), -40, 40);
    if (fixPanning)
        return;

    double zoomFactorNew = GetZoomFactor();

    double zoomFactorDelta = zoomFactorNew / zoomFactorOld;
    double ptX = (PanX - MousePos.x) * zoomFactorDelta + MousePos.x;
    double ptY = (PanY - MousePos.y) * zoomFactorDelta + MousePos.y;
    PanX = ptX;
    PanY = ptY;
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TCustomControl::MouseDown(Button, Shift, X, Y);

    if (Button == mbLeft) {
        if (mouseDblClicked) {
            mouseDblClicked = false;
        } else {
            mouseDown = true;
        }
    }
    ptMouseLast = TPoint(X, Y);
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseMove(TShiftState Shift, int X, int Y)
{
    TCustomControl::MouseMove(Shift, X, Y);

    TPoint ptMouse = TPoint(X, Y);
    if (UseMouseMove && mouseDown) {
        PanX += ptMouse.x - ptMouseLast.x;
        PanY += ptMouse.y - ptMouseLast.y;
        Invalidate();
    }
    ptMouseLast = ptMouse;

    if (UseDrawInfo)
        DrawInfo();
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TCustomControl::MouseUp(Button, Shift, X, Y);

    if (Button == mbLeft)
        mouseDown = false;
}

//---------------------------------------------------------------------------
void __fastcall TImageBox::DblClick(void)
{
    if (GetAsyncKeyState(VK_CONTROL) < 0 && GetKeyState(VK_LBUTTON) < 0) {
        ShowAbout();
        mouseDblClicked = true;
    } else {
        TCustomControl::DblClick();
    }
}

//---------------------------------------------------------------------------
void TImageBox::ResizeDispBuf()
{
    dispBmp->Width = Max(ClientWidth, 64);
    dispBmp->Height = Max(ClientHeight, 64);
}

//---------------------------------------------------------------------------
void TImageBox::DrawCenterLine()
{
    if (ImgBuf == NULL)
        return;

    TPointD ptLeft(0, imgBH/2);
    TPointD ptRight(imgBW, imgBH/2);
    TPointD ptdLeft = ImgToDisp(ptLeft);
    TPointD ptdRight = ImgToDisp(ptRight);

    if (ptdLeft.y >= 0 && ptdLeft.y < ClientHeight && ptdRight.x >= 0 && ptdLeft.x < ClientWidth)
    {
        ptdLeft.x = Clamp(ptdLeft.x, (double)0, (double)ClientWidth);
        ptdRight.x = Clamp(ptdRight.x, (double)0, (double)ClientWidth);
        ptLeft = DispToImg(ptdLeft);
        ptRight = DispToImg(ptdRight);
        DrawLine(ptLeft, ptRight, clYellow, psDot);
    }

    TPointD ptTop(imgBW/2, 0);
    TPointD ptBottom(imgBW/2, imgBH);
    TPointD ptdTop = ImgToDisp(ptTop);
    TPointD ptdBottom = ImgToDisp(ptBottom);

    if (ptdTop.x >= 0 && ptdTop.x < ClientWidth && ptdBottom.y >= 0 && ptTop.y < ClientHeight)
    {
        ptdTop.y = Clamp(ptdTop.y, (double)0, (double)ClientHeight);
        ptdBottom.y = Clamp(ptdBottom.y, (double)0, (double)ClientHeight);
        ptTop = DispToImg(ptdTop);
        ptBottom = DispToImg(ptdBottom);
        DrawLine(ptTop, ptBottom, clYellow, psDot);
    }
}

//---------------------------------------------------------------------------
TColor pseudo[8]= {
    clWhite,    // 0~31
    clAqua,     // 32~63         // blue
    clBlue,     // 63~95
    clYellow,   // 96~127
    clMaroon,   // 128~159         // green
    clFuchsia,  // 160~191
    clRed    ,  // 192~223         // red
    clBlack,    // 224~255
};

//---------------------------------------------------------------------------
void TImageBox::DrawPixelValue()
{
    double ZoomFactor = GetZoomFactor();
    double pixeValFactor = (ImgBytepp == 4) ? 3 : ImgBytepp;
    if (ZoomFactor < FPixelValueDispZoomFactor * pixeValFactor)
        return;

    TPointD ptDisp1 = TPointD(0, 0);
    TPointD ptDisp2 = TPointD(ClientWidth, ClientHeight);
    TPointD ptImg1 = DispToImg(ptDisp1);
    TPointD ptImg2 = DispToImg(ptDisp2);
    int imgX1 = Clamp((int)floor(ptImg1.x), 0, ImgBW-1);
    int imgY1 = Clamp((int)floor(ptImg1.y), 0, ImgBH-1);
    int imgX2 = Clamp((int)floor(ptImg2.x), 0, ImgBW-1);
    int imgY2 = Clamp((int)floor(ptImg2.y), 0, ImgBH-1);

    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = bsClear;
    TColor ofc = Canvas->Font->Color;
    AnsiString ofn = Canvas->Font->Name;
    int ofs = Canvas->Font->Size;
    Canvas->Font->Name = PixelValueDispFont->Name;
    Canvas->Font->Size = PixelValueDispFont->Size;
    for (int imgY = imgY1; imgY <= imgY2; imgY++) {
        for (int imgX = imgX1; imgX <= imgX2; imgX++) {
            TPointD ptImg = TPointD(imgX, imgY);
            TPointD ptDisp = ImgToDisp(ptImg);
            AnsiString pixelValText = GetImagePixelValueText(imgX, imgY);
            int pixelVal = GetImagePixelValueAverage(imgX, imgY);
            Canvas->Font->Color = pseudo[pixelVal / 32];
            Canvas->TextOut(ptDisp.x, ptDisp.y, pixelValText);
        }
    }

    Canvas->Brush->Style = obs;
    Canvas->Font->Color = ofc;
    Canvas->Font->Name = ofn;
    Canvas->Font->Size = ofs;
}

//---------------------------------------------------------------------------
void TImageBox::DrawInfo() {
    TPointD ptCur(ptMouseLast.x, ptMouseLast.y);
    TPointD ptImg = DispToImg(ptCur);
    int imgX = (int)floor(ptImg.x);
    int imgY = (int)floor(ptImg.y);
    AnsiString pixelVal = GetImagePixelValueText(imgX, imgY);
    AnsiString info = AnsiString().sprintf("zoom=%s (%d,%d)=%s", GetZoomText().c_str(), imgX, imgY, pixelVal.c_str());

    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->Canvas->Font->Name = Font->Name;
    bmp->Canvas->Font->Size = Font->Size;
    SIZE size = Canvas->TextExtent(info);
    bmp->Width = 250;
    bmp->Height = size.cy;
    bmp->Canvas->Pen->Color = clBlack;
    bmp->Canvas->Brush->Color = clBlack;
    bmp->Canvas->Rectangle(0, 0, 250, size.cy);
    bmp->Canvas->Font->Color = clWhite;
    bmp->Canvas->TextOut(0, 0, info);
    Canvas->Draw(2, 2, bmp);
    delete bmp;
}

//---------------------------------------------------------------------------
void TImageBox::DrawDrawTime(AnsiString info) {
    DrawStringScreen(info, ClientWidth - 120, 0, clBlack, true, clWhite);
}

//---------------------------------------------------------------------------
TPointD TImageBox::DispToImg(TPointD pt) {
    double ZoomFactor = GetZoomFactor();
    double x = (pt.x - PanX) / ZoomFactor;
    double y = (pt.y - PanY) / ZoomFactor;
    return TPointD(x, y);
}

//---------------------------------------------------------------------------
TPointD TImageBox::ImgToDisp(TPointD pt) {
    double ZoomFactor = GetZoomFactor();
    double x = pt.x * ZoomFactor + PanX;
    double y = pt.y * ZoomFactor + PanY;
    return TPointD(x, y);
}

//---------------------------------------------------------------------------
AnsiString TImageBox::GetImagePixelValueText(int x, int y) {
    if (ImgBuf == NULL || x < 0 || x >= ImgBW || y < 0 || y >= ImgBH)
        return "";
    BYTE* ptr = ImgBuf + ((INT64)ImgBW * y + x) * ImgBytepp;
    if (ImgBytepp == 1)
        return IntToStr(ptr[0]);
    if (ImgBytepp == 2)
        return IntToStr(ptr[1] | ptr[0] << 8);
    return AnsiString().sprintf("%d,%d,%d", ptr[2], ptr[1], ptr[0]);
}

//---------------------------------------------------------------------------
int TImageBox::GetImagePixelValueAverage(int x, int y) {
    if (ImgBuf == NULL || x < 0 || x >= ImgBW || y < 0 || y >= ImgBH)
        return 0;
    BYTE* ptr = ImgBuf + ((INT64)ImgBW * y + x) * ImgBytepp;
    if (ImgBytepp == 1)
        return ptr[0];
    if (ImgBytepp == 2)
        return ptr[0];
    return ((int)ptr[2] + (int)ptr[1] + (int)ptr[0]) / 3;
}

//---------------------------------------------------------------------------
void TImageBox::DrawPixel(TPointD pt, TColor color)
{
    TPointD ptd = ImgToDisp(pt);
    Canvas->Pixels[(int)ptd.x][(int)ptd.y] = color;
}

//---------------------------------------------------------------------------
void TImageBox::DrawPixel(double x, double y, TColor color)
{
    DrawPixel(TPointD(x, y), color);
}

//---------------------------------------------------------------------------
void TImageBox::DrawLine(TPointD pt1, TPointD pt2, TColor color, TPenStyle ps)
{
    TColor oc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;
    TPenStyle ops = Canvas->Pen->Style;
    Canvas->Pen->Style = ps;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = bsClear;


    TPointD ptd1 = ImgToDisp(pt1);
    TPointD ptd2 = ImgToDisp(pt2);
    Canvas->MoveTo(ptd1.x, ptd1.y);
    Canvas->LineTo(ptd2.x, ptd2.y);

    Canvas->Pen->Color = oc;
    Canvas->Pen->Style = ops;
    Canvas->Brush->Style = obs;
}

//---------------------------------------------------------------------------
void TImageBox::DrawLine(double x1, double y1, double x2, double y2, TColor color, TPenStyle ps)
{
    DrawLine(TPointD(x1, y1), TPointD(x2, y2), color, ps);
}

//---------------------------------------------------------------------------
void TImageBox::DrawRectangle(TPointD pt1, TPointD pt2, TColor color, bool fill, TColor fillColor)
{
    TColor opc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;
    TColor obc = Canvas->Brush->Color;
    Canvas->Brush->Color = fillColor;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = fill ? bsSolid : bsClear;

    TPointD ptd1 = ImgToDisp(pt1);
    TPointD ptd2 = ImgToDisp(pt2);
    Canvas->Rectangle(ptd1.x, ptd1.y, ptd2.x, ptd2.y);

    Canvas->Pen->Color = opc;
    Canvas->Brush->Color = obc;
    Canvas->Brush->Style = obs;
}

//---------------------------------------------------------------------------
void TImageBox::DrawRectangle(double x1, double y1, double x2, double y2, TColor color, bool fill, TColor fillColor)
{
    DrawRectangle(TPointD(x1, y1), TPointD(x2, y2), color, fill, fillColor);
}

//---------------------------------------------------------------------------
void TImageBox::DrawEllipse(TPointD pt1, TPointD pt2, TColor color, bool fill, TColor fillColor)
{
    TColor opc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;
    TColor obc = Canvas->Brush->Color;
    Canvas->Brush->Color = fillColor;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = fill ? bsSolid : bsClear;

    TPointD ptd1 = ImgToDisp(pt1);
    TPointD ptd2 = ImgToDisp(pt2);
    Canvas->Ellipse(ptd1.x, ptd1.y, ptd2.x, ptd2.y);

    Canvas->Pen->Color = opc;
    Canvas->Brush->Color = obc;
    Canvas->Brush->Style = obs;
}

//---------------------------------------------------------------------------
void TImageBox::DrawEllipse(double x1, double y1, double x2, double y2, TColor color, bool fill, TColor fillColor)
{
    DrawEllipse(TPointD(x1, y1), TPointD(x2, y2), color, fill, fillColor);
}

//---------------------------------------------------------------------------
void TImageBox::DrawSquare(TPointD pt, double size, TColor color, bool fixSize, bool fill, TColor fillColor)
{
    TColor opc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;
    TColor obc = Canvas->Brush->Color;
    Canvas->Brush->Color = fillColor;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = fill ? bsSolid : bsClear;

    TPointD ptd = ImgToDisp(pt);
    double sizeh = fixSize ? size * 0.5 : size * GetZoomFactor() * 0.5;
    double x1 = ptd.x - sizeh;
    double y1 = ptd.y - sizeh;
    double x2 = ptd.x + sizeh;
    double y2 = ptd.y + sizeh;
    Canvas->Rectangle(x1, y1, x2, y2);

    Canvas->Pen->Color = opc;
    Canvas->Brush->Color = obc;
    Canvas->Brush->Style = obs;
}

//---------------------------------------------------------------------------
void TImageBox::DrawSquare(double x, double y, double size, TColor color, bool fixSize, bool fill, TColor fillColor)
{
    DrawSquare(TPointD(x, y), size, color, fixSize, fill, fillColor);
}

//---------------------------------------------------------------------------
void TImageBox::DrawCircle(TPointD pt, double size, TColor color, bool fixSize, bool fill, TColor fillColor)
{
    TColor opc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;
    TColor obc = Canvas->Brush->Color;
    Canvas->Brush->Color = fillColor;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = fill ? bsSolid : bsClear;

    TPointD ptd = ImgToDisp(pt);
    double sizeh = fixSize ? size * 0.5 : size * GetZoomFactor() * 0.5;
    double x1 = ptd.x - sizeh;
    double y1 = ptd.y - sizeh;
    double x2 = ptd.x + sizeh;
    double y2 = ptd.y + sizeh;
    Canvas->Ellipse(x1, y1, x2, y2);

    Canvas->Pen->Color = opc;
    Canvas->Brush->Color = obc;
    Canvas->Brush->Style = obs;
}

//---------------------------------------------------------------------------
void TImageBox::DrawCircle(double x, double y, double size, TColor color, bool fixSize, bool fill, TColor fillColor)
{
    DrawCircle(TPointD(x, y), size, color, fixSize, fill, fillColor);
}

//---------------------------------------------------------------------------
void TImageBox::DrawPlus(TPointD pt, double size, TColor color, bool fixSize)
{
    TColor oc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;

    TPointD ptd = ImgToDisp(pt);
    double sizeh = fixSize ? size * 0.5 : size * GetZoomFactor() * 0.5;
    double x1 = ptd.x;
    double y1 = ptd.y - sizeh;
    double x2 = ptd.x;
    double y2 = ptd.y + sizeh;
    double x3 = ptd.x - sizeh;
    double y3 = ptd.y;
    double x4 = ptd.x + sizeh;
    double y4 = ptd.y;
    Canvas->MoveTo(x1, y1);
    Canvas->LineTo(x2, y2);
    Canvas->MoveTo(x3, y3);
    Canvas->LineTo(x4, y4);

    Canvas->Pen->Color = oc;
}

//---------------------------------------------------------------------------
void TImageBox::DrawPlus(double x, double y, double size, TColor color, bool fixSize)
{
    DrawPlus(TPointD(x, y), size, color, fixSize);
}

//---------------------------------------------------------------------------
void TImageBox::DrawCross(TPointD pt, double size, TColor color, bool fixSize)
{
    TColor oc = Canvas->Pen->Color;
    Canvas->Pen->Color = color;

    TPointD ptd = ImgToDisp(pt);
    double sizeh = fixSize ? size * 0.5 : size * GetZoomFactor() * 0.5;
    double x1 = ptd.x - sizeh;
    double y1 = ptd.y - sizeh;
    double x2 = ptd.x + sizeh;
    double y2 = ptd.y + sizeh;
    double x3 = ptd.x + sizeh;
    double y3 = ptd.y - sizeh;
    double x4 = ptd.x - sizeh;
    double y4 = ptd.y + sizeh;
    Canvas->MoveTo(x1, y1);
    Canvas->LineTo(x2, y2);
    Canvas->MoveTo(x3, y3);
    Canvas->LineTo(x4, y4);

    Canvas->Pen->Color = oc;
}

//---------------------------------------------------------------------------
void TImageBox::DrawCross(double x, double y, double size, TColor color, bool fixSize)
{
    DrawCross(TPointD(x, y), size, color, fixSize);
}

//---------------------------------------------------------------------------
void TImageBox::DrawString(AnsiString text, TPointD pt, TColor color, bool fill, TColor fillColor)
{
    TColor opc = Canvas->Pen->Color;
    Canvas->Pen->Color = fillColor;
    TPenStyle ops = Canvas->Pen->Style;
    Canvas->Pen->Style = fill ? psSolid : psClear;
    TColor obc = Canvas->Brush->Color;
    Canvas->Brush->Color = fillColor;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = fill ? bsSolid : bsClear;
    TColor ofc = Canvas->Font->Color;
    Canvas->Font->Color = color;

    TPointD ptd = ImgToDisp(pt);
    RECT rc;
    DrawTextA(Canvas->Handle, text.c_str(), text.Length(), &rc, DT_CALCRECT);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    RECT rect = {(int)ptd.x, (int)ptd.y, (int)ptd.x + width, (int)ptd.y + height};
    Canvas->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
    DrawTextA(Canvas->Handle, text.c_str(), text.Length(), &rect, NULL);

    Canvas->Pen->Color = opc;
    Canvas->Pen->Style = ops;
    Canvas->Brush->Color = obc;
    Canvas->Brush->Style = obs;
    Canvas->Font->Color = ofc;
}

//---------------------------------------------------------------------------
void TImageBox::DrawString(AnsiString text, double x, double y, TColor color, bool fill, TColor fillColor)
{
    DrawString(text, TPointD(x, y), color, fill, fillColor);
}

//---------------------------------------------------------------------------
void TImageBox::DrawStringScreen(AnsiString text, TPoint pt, TColor color, bool fill, TColor fillColor)
{
    TColor opc = Canvas->Pen->Color;
    Canvas->Pen->Color = fillColor;
    TPenStyle ops = Canvas->Pen->Style;
    Canvas->Pen->Style = fill ? psSolid : psClear;
    TColor obc = Canvas->Brush->Color;
    Canvas->Brush->Color = fillColor;
    TBrushStyle obs = Canvas->Brush->Style;
    Canvas->Brush->Style = fill ? bsSolid : bsClear;
    TColor ofc = Canvas->Font->Color;
    Canvas->Font->Color = color;

    RECT rc;
    DrawTextA(Canvas->Handle, text.c_str(), text.Length(), &rc, DT_CALCRECT);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    RECT rect = {pt.x, pt.y, pt.x + width, pt.y + height};
    Canvas->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
    DrawTextA(Canvas->Handle, text.c_str(), text.Length(), &rect, NULL);

    Canvas->Pen->Color = opc;
    Canvas->Pen->Style = ops;
    Canvas->Brush->Color = obc;
    Canvas->Brush->Style = obs;
    Canvas->Font->Color = ofc;
}

//---------------------------------------------------------------------------
void TImageBox::DrawStringScreen(AnsiString text, int x, int y, TColor color, bool fill, TColor fillColor)
{
    DrawStringScreen(text, TPoint(x, y), color, fill, fillColor);
}

//---------------------------------------------------------------------------
void TImageBox::ShowAbout() {
    TFormAbout *frm = new TFormAbout(this);
    TModalResult mr = frm->ShowModal();
    if (mr == mrOk) {
        Invalidate();    
    }
}

//---------------------------------------------------------------------------
#pragma package(smart_init)



