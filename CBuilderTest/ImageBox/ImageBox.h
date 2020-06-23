//---------------------------------------------------------------------------
#ifndef ImageBoxH
#define ImageBoxH

//---------------------------------------------------------------------------
#include <Controls.hpp>

//---------------------------------------------------------------------------
struct TPointD {
    double x, y;
    TPointD(double _x, double _y) : x(_x), y(_y) {}
};

//---------------------------------------------------------------------------
class TImageBox : public TCustomControl
{
public:
    static const String VersionHistory;
private:
    bool FUseDrawPixelValue;
    bool FUseDrawInfo;
    bool FUseDrawCenterLine;
    bool FUseDrawDrawTime;
    bool FUseInterPorlation;
    bool FUseParallel;
    bool FUseMouseMove;
    bool FUseMouseWheelZoom;
    int FZoomLevel;
    int FZoomLevelMin;
    int FZoomLevelMax;
    int FPixelValueDispZoomFactor;
    TNotifyEvent FOnPaint;

    //디스플레이용 버퍼
    Graphics::TBitmap* dispBmp;
    TColor FColor;
    TCanvas* GetCanvas() { return TCustomControl::Canvas; }
    TFont* GetFont() { return TControl::Font; }
    TFont* FPixelValueDispFont;

    //이미지용 버퍼
    int imgBW;
    int imgBH;
    BYTE* imgBuf;
    int imgBytepp;

    // float 타입 버퍼
    bool FBufIsFloat;
    bool FFloatPreprocessed;
    BYTE* grayBuf;

    // 마우스 패닝
    double FPanX;
    double FPanY;
    bool FUseMousePanClamp;

    //마우스 다운 여부
    BOOL mouseDown;
    //마우스 마지막 위치
    TPoint ptMouseLast;
    //마우스 더블클릭했는지
    BOOL mouseDblClicked;   // 정보창 띄운후 이미지 이동 하는 버그 때문에

    void GetZoomFactorComponents(int* exp_num, int* c); // 줌 배율 계산
    String GetZoomText(); //줌 배율 표시 리턴
    void WheelScroll(int WheelDelta, TPoint MousePos, BOOL vertical); //휠 스크롤
    void WheelZoom(int WheelDelta, TPoint MousePos, BOOL fixPanning); //휠 줌
    void ResizeDispBuf(); //표시 버퍼 생성
    void DrawCenterLine(TCanvas* cnv); //중심선 표시
    void DrawPixelValue(TCanvas* cnv); //이미지 픽셀값 표시
    void DrawInfo(TCanvas* cnv); //좌상단 정보 표시
    void DrawDrawTime(TCanvas* cnv, String info); //렌더링 시간 표시
    String GetImagePixelValueText(int x, int y); //이미지 픽셀값 문자열 리턴
    int GetImagePixelValueAverage(int x, int y); //이미지 픽셀값 평균 리턴 (0~255)

public: //User declarations
    //생성자
    __fastcall TImageBox(TComponent* Owner);
    virtual __fastcall ~TImageBox();

    //이미지 버퍼 세팅
    void SetImgBuf(BYTE* buf, int bw, int bh, int bytepp, BOOL bInvalidate);
    //float, double 버퍼 세팅
    void SetFloatBuf(BYTE* buf, int bw, int bh, int bytepp, bool preprocess, BOOL bInvalidate);
    // 줌 배율 리턴
    double GetZoomFactor();
    //사각형을 피팅 되도록 줌 변경
    void ZoomToRect(int x, int y, int width, int height);
    //줌 리셋
    void ZoomReset();
    //화면 좌표를  이미지 좌표로 변환
    TPointD DispToImg(TPointD pt);
    // 이미지 좌표를 화면 좌표로 변환
    TPointD ImgToDisp(TPointD pt);
    // 정보 창 표시
    void ShowAbout();

    //이미지용 버퍼
    __property int ImgBW = {read = imgBW};
    __property int ImgBH = {read = imgBH};
    __property BYTE* ImgBuf = {read = imgBuf};
    __property int ImgBytepp = {read = imgBytepp};
    __property bool BufIsFloat = {read = FBufIsFloat};
    __property bool FloatPreprocessed = {read = FFloatPreprocessed};

    //캔버스
    __property TCanvas* Canvas = {read = GetCanvas};
    //폰트
    __property TFont* DrawFont = {read = GetFont };
    __property TFont* PixelValueDispFont = {read = FPixelValueDispFont};

    // draw methods
    void DrawPixel(TCanvas* cnv, TPointD pt, TColor color);
    void DrawPixel(TCanvas* cnv, double x, double y, TColor color);
    void DrawLine(TCanvas* cnv, TPointD pt1, TPointD pt2, TColor color, TPenStyle ps = psSolid);
    void DrawLine(TCanvas* cnv, double x1, double y1, double x2, double y2, TColor color, TPenStyle ps = psSolid);
    void DrawRectangle(TCanvas* cnv, TPointD pt1, TPointD pt2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawRectangle(TCanvas* cnv, double x1, double y1, double x2, double y2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawEllipse(TCanvas* cnv, TPointD pt1, TPointD pt2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawEllipse(TCanvas* cnv, double x1, double y1, double x2, double y2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawSquare(TCanvas* cnv, TPointD pt, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawSquare(TCanvas* cnv, double x, double y, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawCircle(TCanvas* cnv, TPointD pt, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawCircle(TCanvas* cnv, double x, double y, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawPlus(TCanvas* cnv, TPointD pt, double size, TColor color, bool fixSize);
    void DrawPlus(TCanvas* cnv, double x, double y, double size, TColor color, bool fixSize);
    void DrawCross(TCanvas* cnv, TPointD pt, double size, TColor color, bool fixSize);
    void DrawCross(TCanvas* cnv, double x, double y, double size, TColor color, bool fixSize);
    void DrawString(TCanvas* cnv, String text, TPointD pt, bool useImageCoord, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawString(TCanvas* cnv, String text, double x, double y, bool useImageCoord, TColor color, bool fill = false, TColor fillColor = clWhite);

__published:
    //화면 표시 옵션
    __property bool UseDrawPixelValue = {read = FUseDrawPixelValue, write = FUseDrawPixelValue};
    __property bool UseDrawInfo = {read = FUseDrawInfo, write = FUseDrawInfo};
    __property bool UseDrawCenterLine = {read = FUseDrawCenterLine, write = FUseDrawCenterLine};
    __property bool UseDrawDrawTime = {read = FUseDrawDrawTime, write = FUseDrawDrawTime};
    __property bool UseInterPorlation = {read = FUseInterPorlation, write = FUseInterPorlation};
    __property bool UseParallel = {read = FUseParallel, write = FUseParallel};
    //마우스 동작 옵션
    __property bool UseMouseMove = {read = FUseMouseMove, write = FUseMouseMove};
    __property bool UseMouseWheelZoom = {read = FUseMouseWheelZoom, write = FUseMouseWheelZoom};
    //줌 레벨
    __property int ZoomLevel = {read = FZoomLevel, write = SetZoomLevel};
    __property int ZoomLevelMin = {read = FZoomLevelMin, write = FZoomLevelMin};
    __property int ZoomLevelMax = {read = FZoomLevelMax, write = FZoomLevelMax};
    __property double PanX = {read = FPanX, write = SetPanX};
    __property double PanY = {read = FPanY, write = SetPanY};
    __property bool UseMousePanClamp = {read = FUseMousePanClamp, write = FUseMousePanClamp};
    //픽셀값 표시 줌값
    __property int PixelValueDispZoomFactor = {read = FPixelValueDispZoomFactor, write = FPixelValueDispZoomFactor};
    //배경색
    __property TColor BgColor = {read = FColor, write = FColor};
    //페인트 이벤트
	__property TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};
	__property TMouseEvent OnMouseDown = {read=FOnMouseDown, write=FOnMouseDown};
	__property TMouseMoveEvent OnMouseMove = {read=FOnMouseMove, write=FOnMouseMove};
	__property TMouseEvent OnMouseUp = {read=FOnMouseUp, write=FOnMouseUp};
    void __fastcall SetZoomLevel(const int Value);
    void __fastcall SetPanX(const double Value);
    void __fastcall SetPanY(const double Value);

protected:
    //리사이즈 할때
    DYNAMIC void __fastcall Resize();
    //페인트 할때
    virtual void __fastcall Paint();
    //마우스 휠
	DYNAMIC void __fastcall MouseWheelHandler(TMessage &Message);
    //마우스 다운
    DYNAMIC void __fastcall MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
    //마우스 무브
    DYNAMIC void __fastcall MouseMove(TShiftState Shift, int X, int Y);
    //마우스 업
    DYNAMIC void __fastcall MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
    //마우스 더블클릭
    DYNAMIC void __fastcall DblClick(void);
};

void CopyImageBufferZoom(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomFloat(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomIpl(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomIplFloat(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);

//---------------------------------------------------------------------------
#endif

