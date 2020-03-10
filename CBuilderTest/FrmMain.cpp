//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <Clipbrd.hpp>
#include <DateUtils.hpp>
#include <jpeg.hpp>
#pragma hdrstop

#include "FrmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
    pbxDraw = new TImageBox(this);
    pbxDraw->Align = alClient;
    pbxDraw->BgColor = clGray;
    pbxDraw->UseDrawDrawTime = true;
    pbxDraw->OnPaint = pbxDrawOnPaint;
    pbxDraw->Parent = this;
    if (ParamCount() > 0) {
        LoadImageFile(ParamStr(1));
    }
}

void BitmapToBuf(Graphics::TBitmap* bmp, BYTE* buf, int bw, int bh, int bytepp)
{
    int scanstep = bw * bytepp;
    for (int y = 0; y < bh; y++) {
        BYTE* sptr = (BYTE*)bmp->ScanLine[y];
        BYTE* dptr = buf + scanstep * y;
        memcpy(dptr, sptr, scanstep);
    }
}
//---------------------------------------------------------------------------

void BufToBitmap(BYTE* buf, Graphics::TBitmap* bmp, int bw, int bh, int bytepp)
{
    int scanstep = bw * bytepp;
    for (int y = 0; y < bh; y++) {
        BYTE* sptr = buf + scanstep * y;
        BYTE* dptr = (BYTE*)bmp->ScanLine[y];
        memcpy(dptr, sptr, scanstep);
    }
}
//---------------------------------------------------------------------------

HPALETTE CreateGrayPalette() {
    BYTE* buf = new BYTE[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 255];
    LOGPALETTE* ppal = (LOGPALETTE*)buf;
    ppal->palVersion = 0x300;
    ppal->palNumEntries = 256;
    for (int i = 0; i < 256; i++) {
        ppal->palPalEntry[i].peRed = i;
        ppal->palPalEntry[i].peGreen = i;
        ppal->palPalEntry[i].peBlue = i;
        ppal->palPalEntry[i].peFlags = PC_RESERVED;
    }
    HPALETTE hPal = CreatePalette(ppal);
    delete[] buf;
    return hPal;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ReadBitmap(Graphics::TBitmap* bmp) {
    bw = bmp->Width;
    bh = bmp->Height;
    if (bmp->PixelFormat == pf8bit)
        bytepp = 1;
    else if (bmp->PixelFormat == pf16bit)
        bytepp = 2;
    else if (bmp->PixelFormat == pf24bit)
        bytepp = 3;
    else
        bytepp = 4;

    if (imgBuf != NULL) {
        delete [] imgBuf;
    }

    imgBuf = new BYTE[bw * bh * bytepp];
    BitmapToBuf(bmp, imgBuf, bw, bh, bytepp);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::WriteBitmap(Graphics::TBitmap* bmp) {
    bmp->Width = bw;
    bmp->Height = bh;
    if (bytepp == 1)
        bmp->PixelFormat = pf8bit;
    else if (bytepp == 2)
        bmp->PixelFormat = pf16bit;
    else if (bytepp == 3)
        bmp->PixelFormat = pf24bit;
    else
        bmp->PixelFormat == pf32bit;

    if (bytepp == 1) {
        HPALETTE hPal = CreateGrayPalette();
        DeleteObject(bmp->Palette);
        bmp->Palette = hPal;
    }

    BufToBitmap(imgBuf, bmp, bw, bh, bytepp);
}
//---------------------------------------------------------------------------

void TFormMain::LoadImageFile(AnsiString fileName)
{
    AnsiString ext = ExtractFileExt(fileName).LowerCase();
    TGraphic* img;
    if (ext == ".jpg" || ext == ".jpeg") {
        img = new TJPEGImage();
    } else {
        img = new Graphics::TBitmap();
    }
    img->LoadFromFile(fileName);
	Graphics::TBitmap *bmp = new Graphics::TBitmap();
    bmp->Assign(img);

    ReadBitmap(bmp);

    delete bmp;
    delete img;
    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void TFormMain::SaveImageFile(AnsiString fileName)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    WriteBitmap(bmp);
    bmp->SaveToFile(fileName);
    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::pbxDrawOnPaint(TObject *Sender)
{
    if (RetainedDrawTest1->Checked) {
        UserDrawTest(pbxDraw->Canvas);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OpenFile1Click(TObject *Sender)
{
    bool r = dlgOpen->Execute();
    if (!r)
        return;

    AnsiString fileName = dlgOpen->FileName;
    LoadImageFile(fileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Copy1Click(TObject *Sender)
{
    if (imgBuf == NULL) {
        return;
    }

    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    WriteBitmap(bmp);
    Clipboard()->Assign(bmp);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PastefromClipboard1Click(TObject *Sender)
{
    TClipboard* clip = Clipboard();
    if (clip->HasFormat(CF_BITMAP) == false)
        return;

	Graphics::TBitmap *bmp = new Graphics::TBitmap();
    bmp->LoadFromClipboardFormat(CF_BITMAP, clip->GetAsHandle(CF_BITMAP), 0);

    ReadBitmap(bmp);

    delete bmp;
    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1Click(TObject *Sender)
{
//    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Lenna41Click(TObject *Sender)
{
//    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Coins1Click(TObject *Sender)
{
//    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Chess1Click(TObject *Sender)
{
//    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LongImage1Click(TObject *Sender)
{
    GenerateBitmap(256, 2000000);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::WideImage1Click(TObject *Sender)
{
     GenerateBitmap(2000000, 256);
}
//---------------------------------------------------------------------------

void TFormMain::GenerateBitmap(int bw, int bh) {
   if (imgBuf != NULL)
       delete[] imgBuf;
   int cb = bw * bh;
   imgBuf = new BYTE[cb];
   for (int y = 0; y < bh; y++) {
       BYTE* ptr = imgBuf + y * bw;
       for (int x = 0; x < bw; x++) {
           ptr[x] = (x + y) % 256;
       }
   }
   pbxDraw->SetImgBuf(imgBuf, bw, bh, 1, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ZoomReset1Click(TObject *Sender)
{
    pbxDraw->ZoomReset();
    pbxDraw->Invalidate();   
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Zoomtoimage1Click(TObject *Sender)
{
    if (pbxDraw->ImgBW <= 0 || pbxDraw->ImgBH <= 0)
        pbxDraw->ZoomReset();
    else
        pbxDraw->ZoomToRect(0, 0, pbxDraw->ImgBW, pbxDraw->ImgBH);
    pbxDraw->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImmediateDrawTest1Click(TObject *Sender)
{
    UserDrawTest(pbxDraw->Canvas);
}
//---------------------------------------------------------------------------

double GetTimeMs() {
    static LARGE_INTEGER freq;
    static BOOL r = QueryPerformanceFrequency(&freq);
    LARGE_INTEGER cnt;
    QueryPerformanceCounter(&cnt);
    return cnt.QuadPart * 1000.0 / freq.QuadPart;
}

void TFormMain::UserDrawTest(TCanvas* g) {
    double st = GetTimeMs();

    Randomize();
    int step = 20;
    if (DrawEllipse1->Checked) {
        TBrushStyle obs = g->Brush->Style;
        g->Brush->Style = bsClear;
        TColor opc = g->Pen->Color;
        g->Pen->Color = (TColor)RGB(Random(256), Random(256), Random(256));
        for (int y = 0; y < 1000; y += step) {
            for (int x = 0; x < 1000; x += step) {
                g->Ellipse(x, y, x + step, y + step);
            }
        }
        g->Brush->Style = obs;
        g->Pen->Color = opc;
    } else if (FillEllipse1->Checked) {
        TBrushStyle obs = g->Brush->Style;
        g->Brush->Style = bsSolid;
        TColor opc = g->Pen->Color;
        g->Pen->Color = (TColor)RGB(Random(256), Random(256), Random(256));
        TColor obc = g->Brush->Color;
        g->Brush->Color = g->Pen->Color;
        for (int y = 0; y < 1000; y += step) {
            for (int x = 0; x < 1000; x += step) {
                g->Ellipse(x, y, x + step, y + step);
            }
        }
        g->Brush->Style = obs;
        g->Pen->Color = opc;
        g->Brush->Color = obc;
    } else if (DrawString1->Checked) {
        TColor ofc = g->Font->Color;
        g->Font->Color = (TColor)RGB(Random(256), Random(256), Random(256));
        TBrushStyle obs = Canvas->Brush->Style;
        g->Brush->Style = bsClear;
        for (int y = 0; y < 1000; y += step) {
            for (int x = 0; x < 1000; x += step) {
                g->TextOut(x, y, "128");
            }
        }
        g->Font->Color = ofc;
        g->Brush->Style = obs;
    } else if (DrawShapes1->Checked) {
        pbxDraw->DrawLine(1, 1, 2, 2, clLime);
        pbxDraw->DrawLine(1, 2, 2, 1, clRed);
        pbxDraw->DrawRectangle(2, 2, 3, 3, clRed, true, clLime);
        pbxDraw->DrawRectangle(2, 2, 3, 3, clRed, false, clRed);
        pbxDraw->DrawEllipse(3, 3, 4, 4, clRed, true, clLime);
        pbxDraw->DrawEllipse(3, 3, 4, 4, clRed, false, clRed);
        pbxDraw->DrawCross(10, 10, 20, clLime, false);
        pbxDraw->DrawPlus(10, 10, 20, clRed, true);
    } else if (DrawPixelCircles1->Checked) {
        TColor col = (TColor)RGB(Random(256), Random(256), Random(256));
        for (int y = 0; y < 100; y++) {
            for (int x = 0; x < 100; x++) {
                pbxDraw->DrawEllipse(x, y, x + 1, y + 1, col);
            }
        }
    }

    double et = GetTimeMs();
    double ms = et - st;
    AnsiString text = AnsiString().sprintf("DrawTime : %.2f", ms);
    pbxDraw->DrawStringScreen(text, 255, 2, clBlack, true, clWhite);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RetainedDrawTest1Click(TObject *Sender)
{
    pbxDraw->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DrawEllipse1Click(TObject *Sender)
{
    pbxDraw->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::AboutImageBox1Click(TObject *Sender)
{
    pbxDraw->ShowAbout();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SaveFile1Click(TObject *Sender)
{
    if (imgBuf == NULL) {
        ShowMessage("imgBuf == NULL");
        return;
    }

    bool r = dlgSave->Execute();
    if (!r)
        return;

    AnsiString fileName = dlgSave->FileName;
    SaveImageFile(fileName);
}
//---------------------------------------------------------------------------

