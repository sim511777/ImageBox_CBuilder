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

    //���÷��̿� ����
    Graphics::TBitmap* dispBmp;
    TColor FColor;
    TCanvas* GetCanvas() { return TCustomControl::Canvas; }
    TFont* GetFont() { return TControl::Font; }
    TFont* FPixelValueDispFont;

    //�̹����� ����
    int imgBW;
    int imgBH;
    BYTE* imgBuf;
    int imgBytepp;

    // float Ÿ�� ����
    bool FBufIsFloat;
    bool FFloatPreprocessed;
    BYTE* grayBuf;

    // ���콺 �д�
    double FPanX;
    double FPanY;
    bool FUseMousePanClamp;

    //���콺 �ٿ� ����
    BOOL mouseDown;
    //���콺 ������ ��ġ
    TPoint ptMouseLast;
    //���콺 ����Ŭ���ߴ���
    BOOL mouseDblClicked;   // ����â ����� �̹��� �̵� �ϴ� ���� ������

    void GetZoomFactorComponents(int* exp_num, int* c); // �� ���� ���
    String GetZoomText(); //�� ���� ǥ�� ����
    void WheelScroll(int WheelDelta, TPoint MousePos, BOOL vertical); //�� ��ũ��
    void WheelZoom(int WheelDelta, TPoint MousePos, BOOL fixPanning); //�� ��
    void ResizeDispBuf(); //ǥ�� ���� ����
    void DrawCenterLine(TCanvas* cnv); //�߽ɼ� ǥ��
    void DrawPixelValue(TCanvas* cnv); //�̹��� �ȼ��� ǥ��
    void DrawInfo(TCanvas* cnv); //�»�� ���� ǥ��
    void DrawDrawTime(TCanvas* cnv, String info); //������ �ð� ǥ��
    String GetImagePixelValueText(int x, int y); //�̹��� �ȼ��� ���ڿ� ����
    int GetImagePixelValueAverage(int x, int y); //�̹��� �ȼ��� ��� ���� (0~255)

public: //User declarations
    //������
    __fastcall TImageBox(TComponent* Owner);
    virtual __fastcall ~TImageBox();

    //�̹��� ���� ����
    void SetImgBuf(BYTE* buf, int bw, int bh, int bytepp, BOOL bInvalidate);
    //float, double ���� ����
    void SetFloatBuf(BYTE* buf, int bw, int bh, int bytepp, bool preprocess, BOOL bInvalidate);
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
    __property bool BufIsFloat = {read = FBufIsFloat};
    __property bool FloatPreprocessed = {read = FFloatPreprocessed};

    //ĵ����
    __property TCanvas* Canvas = {read = GetCanvas};
    //��Ʈ
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
    __property int ZoomLevel = {read = FZoomLevel, write = SetZoomLevel};
    __property int ZoomLevelMin = {read = FZoomLevelMin, write = FZoomLevelMin};
    __property int ZoomLevelMax = {read = FZoomLevelMax, write = FZoomLevelMax};
    __property double PanX = {read = FPanX, write = SetPanX};
    __property double PanY = {read = FPanY, write = SetPanY};
    __property bool UseMousePanClamp = {read = FUseMousePanClamp, write = FUseMousePanClamp};
    //�ȼ��� ǥ�� �ܰ�
    __property int PixelValueDispZoomFactor = {read = FPixelValueDispZoomFactor, write = FPixelValueDispZoomFactor};
    //����
    __property TColor BgColor = {read = FColor, write = FColor};
    //����Ʈ �̺�Ʈ
	__property TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};
	__property TMouseEvent OnMouseDown = {read=FOnMouseDown, write=FOnMouseDown};
	__property TMouseMoveEvent OnMouseMove = {read=FOnMouseMove, write=FOnMouseMove};
	__property TMouseEvent OnMouseUp = {read=FOnMouseUp, write=FOnMouseUp};
    void __fastcall SetZoomLevel(const int Value);
    void __fastcall SetPanX(const double Value);
    void __fastcall SetPanY(const double Value);

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

void CopyImageBufferZoom(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomFloat(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomIpl(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomIplFloat(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);

//---------------------------------------------------------------------------
#endif

