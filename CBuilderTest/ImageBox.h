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
    static const AnsiString VersionHistory;
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
    int FPixelValueDispZoomFactor;
    TNotifyEvent FOnPaint;

    //디스플레이용 버퍼
    Graphics::TBitmap* dispBmp;
    TColor FColor;
    TCanvas* GetCanvas() { return TCustomControl::Canvas; }

    //이미지용 버퍼
    int imgBW;
    int imgBH;
    BYTE* imgBuf;
    int imgBytepp;

    // 마우스 패닝
    double FPanX;
    double FPanY;

    //마우스 다운 여부
    BOOL mouseDown;
    //마우스 마지막 위치
    TPoint ptMouseLast;

    void GetZoomFactorComponents(int* exp_num, int* c); // 줌 배율 계산
    AnsiString GetZoomText(); //줌 배율 표시 리턴
    void WheelScroll(int WheelDelta, TPoint MousePos, BOOL vertical); //휠 스크롤
    void WheelZoom(int WheelDelta, TPoint MousePos, BOOL fixPanning); //휠 줌
    void ResizeDispBuf(); //표시 버퍼 생성
    void DrawCenterLine(); //중심선 표시
    void DrawPixelValue(); //이미지 픽셀값 표시
    void DrawInfo(); //좌상단 정보 표시
    void DrawDrawTime(AnsiString info); //렌더링 시간 표시
    AnsiString GetImagePixelValueText(int x, int y); //이미지 픽셀값 문자열 리턴
    int GetImagePixelValueAverage(int x, int y); //이미지 픽셀값 평균 리턴 (0~255)

public: //User declarations
    //생성자
    __fastcall TImageBox(TComponent* Owner);
    virtual __fastcall ~TImageBox();

    //이미지 버퍼 세팅
    void SetImgBuf(BYTE* buf, int bw, int bh, int bytepp, BOOL bInvalidate);
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
    //캔버스
    __property TCanvas* Canvas = {read = GetCanvas};

    // draw methods
    void DrawPixel(TPointD pt, TColor color);
    void DrawPixel(double x, double y, TColor color);
    void DrawLine(TPointD pt1, TPointD pt2, TColor color, TPenStyle ps = psSolid);
    void DrawLine(double x1, double y1, double x2, double y2, TColor color, TPenStyle ps = psSolid);
    void DrawRectangle(TPointD pt1, TPointD pt2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawRectangle(double x1, double y1, double x2, double y2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawEllipse(TPointD pt1, TPointD pt2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawEllipse(double x1, double y1, double x2, double y2, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawSquare(TPointD pt, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawSquare(double x, double y, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawCircle(TPointD pt, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawCircle(double x, double y, double size, TColor color, bool fixSize, bool fill = false, TColor fillColor = clWhite);
    void DrawPlus(TPointD pt, double size, TColor color, bool fixSize);
    void DrawPlus(double x, double y, double size, TColor color, bool fixSize);
    void DrawCross(TPointD pt, double size, TColor color, bool fixSize);
    void DrawCross(double x, double y, double size, TColor color, bool fixSize);
    void DrawString(AnsiString text, TPointD pt, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawString(AnsiString text, double x, double y, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawStringScreen(AnsiString text, TPoint pt, TColor color, bool fill = false, TColor fillColor = clWhite);
    void DrawStringScreen(AnsiString text, int x, int y, TColor color, bool fill = false, TColor fillColor = clWhite);

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
    __property int ZoomLevel = {read = FZoomLevel, write = FZoomLevel};
    __property double PanX = {read = FPanX, write = FPanX};
    __property double PanY = {read = FPanY, write = FPanY};
    //픽셀값 표시 줌값
    __property int PixelValueDispZoomFactor = {read = FPixelValueDispZoomFactor, write = FPixelValueDispZoomFactor};
    //배경색
    __property TColor BgColor = {read = FColor, write = FColor};
    //폰트
    __property TFont* DrawFont = {read = Font, write = Font};
    //페인트 이벤트
	__property TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};

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

//---------------------------------------------------------------------------
#endif

