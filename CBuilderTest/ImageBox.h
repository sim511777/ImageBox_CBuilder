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

    //���÷��̿� ����
    Graphics::TBitmap* dispBmp;
    TColor FColor;
    TCanvas* GetCanvas() { return TCustomControl::Canvas; }

    //�̹����� ����
    int imgBW;
    int imgBH;
    BYTE* imgBuf;
    int imgBytepp;

    // ���콺 �д�
    double FPanX;
    double FPanY;

    //���콺 �ٿ� ����
    BOOL mouseDown;
    //���콺 ������ ��ġ
    TPoint ptMouseLast;

    void GetZoomFactorComponents(int* exp_num, int* c); // �� ���� ���
    AnsiString GetZoomText(); //�� ���� ǥ�� ����
    void WheelScroll(int WheelDelta, TPoint MousePos, BOOL vertical); //�� ��ũ��
    void WheelZoom(int WheelDelta, TPoint MousePos, BOOL fixPanning); //�� ��
    void ResizeDispBuf(); //ǥ�� ���� ����
    void DrawCenterLine(); //�߽ɼ� ǥ��
    void DrawPixelValue(); //�̹��� �ȼ��� ǥ��
    void DrawInfo(); //�»�� ���� ǥ��
    void DrawDrawTime(AnsiString info); //������ �ð� ǥ��
    AnsiString GetImagePixelValueText(int x, int y); //�̹��� �ȼ��� ���ڿ� ����
    int GetImagePixelValueAverage(int x, int y); //�̹��� �ȼ��� ��� ���� (0~255)

public: //User declarations
    //������
    __fastcall TImageBox(TComponent* Owner);
    virtual __fastcall ~TImageBox();

    //�̹��� ���� ����
    void SetImgBuf(BYTE* buf, int bw, int bh, int bytepp, BOOL bInvalidate);
    // �� ���� ����
    double GetZoomFactor();
    //�簢���� ���� �ǵ��� �� ����
    void ZoomToRect(int x, int y, int width, int height);
    //�� ����
    void ZoomReset();
    //ȭ�� ��ǥ��  �̹��� ��ǥ�� ��ȯ
    TPointD DispToImg(TPointD pt);
    // �̹��� ��ǥ�� ȭ�� ��ǥ�� ��ȯ
    TPointD ImgToDisp(TPointD pt);
    // ���� â ǥ��
    void ShowAbout();

    //�̹����� ����
    __property int ImgBW = {read = imgBW};
    __property int ImgBH = {read = imgBH};
    __property BYTE* ImgBuf = {read = imgBuf};
    __property int ImgBytepp = {read = imgBytepp};
    //ĵ����
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
    //ȭ�� ǥ�� �ɼ�
    __property bool UseDrawPixelValue = {read = FUseDrawPixelValue, write = FUseDrawPixelValue};
    __property bool UseDrawInfo = {read = FUseDrawInfo, write = FUseDrawInfo};
    __property bool UseDrawCenterLine = {read = FUseDrawCenterLine, write = FUseDrawCenterLine};
    __property bool UseDrawDrawTime = {read = FUseDrawDrawTime, write = FUseDrawDrawTime};
    __property bool UseInterPorlation = {read = FUseInterPorlation, write = FUseInterPorlation};
    __property bool UseParallel = {read = FUseParallel, write = FUseParallel};
    //���콺 ���� �ɼ�
    __property bool UseMouseMove = {read = FUseMouseMove, write = FUseMouseMove};
    __property bool UseMouseWheelZoom = {read = FUseMouseWheelZoom, write = FUseMouseWheelZoom};
    //�� ����
    __property int ZoomLevel = {read = FZoomLevel, write = FZoomLevel};
    __property double PanX = {read = FPanX, write = FPanX};
    __property double PanY = {read = FPanY, write = FPanY};
    //�ȼ��� ǥ�� �ܰ�
    __property int PixelValueDispZoomFactor = {read = FPixelValueDispZoomFactor, write = FPixelValueDispZoomFactor};
    //����
    __property TColor BgColor = {read = FColor, write = FColor};
    //��Ʈ
    __property TFont* DrawFont = {read = Font, write = Font};
    //����Ʈ �̺�Ʈ
	__property TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};

protected:
    //�������� �Ҷ�
    DYNAMIC void __fastcall Resize();
    //����Ʈ �Ҷ�
    virtual void __fastcall Paint();
    //���콺 ��
	DYNAMIC void __fastcall MouseWheelHandler(TMessage &Message);
    //���콺 �ٿ�
    DYNAMIC void __fastcall MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
    //���콺 ����
    DYNAMIC void __fastcall MouseMove(TShiftState Shift, int X, int Y);
    //���콺 ��
    DYNAMIC void __fastcall MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
    //���콺 ����Ŭ��
    DYNAMIC void __fastcall DblClick(void);
};

//---------------------------------------------------------------------------
#endif

