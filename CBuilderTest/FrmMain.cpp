//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <Clipbrd.hpp>
#include <DateUtils.hpp>
#include <jpeg.hpp>
#include <Math.hpp>
#pragma hdrstop

#include "FrmMain.h"
#include "Util.h"
#include "resource.h"
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

void __fastcall TFormMain::LoadBitmap(Graphics::TBitmap* bmp) {
    FreeBuffer(&imgBuf);
    BitmapToImageBuffer(bmp, &imgBuf, &bw, &bh, &bytepp);
    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LoadBitmapFloat(Graphics::TBitmap* bmp) {
    FreeBuffer(&imgBuf);
    BitmapToImageBuffer(bmp, &imgBuf, &bw, &bh, &bytepp);

    // byte -> float convert
    BYTE* floatBuf = AllocBuffer(bw * bh * sizeof(float));
    for (int y = 0; y < bh; y++) {
        BYTE* src = imgBuf + bw * y;
        float* dst = (float*)floatBuf + bw * y;
        for (int x = 0; x < bw; x++, src++, dst++) {
            *dst = *src;
        }
    }
    FreeBuffer(&imgBuf);
    imgBuf = floatBuf;
    bytepp = sizeof(float);

    // SetFloatBuf
    pbxDraw->SetFloatBuf(imgBuf, bw, bh, bytepp, true, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LoadBitmapDouble(Graphics::TBitmap* bmp) {
    FreeBuffer(&imgBuf);
    BitmapToImageBuffer(bmp, &imgBuf, &bw, &bh, &bytepp);

    // byte -> double convert
    BYTE* floatBuf = AllocBuffer(bw * bh * sizeof(double));
    for (int y = 0; y < bh; y++) {
        BYTE* src = imgBuf + bw * y;
        double* dst = (double*)floatBuf + bw * y;
        for (int x = 0; x < bw; x++, src++, dst++) {
            *dst = *src;
        }
    }
    FreeBuffer(&imgBuf);
    imgBuf = floatBuf;
    bytepp = sizeof(double);

    // SetFloatBuf
    pbxDraw->SetFloatBuf(imgBuf, bw, bh, bytepp, true, TRUE);
}
//---------------------------------------------------------------------------

void TFormMain::LoadImageFile(String fileName)
{
    String ext = ExtractFileExt(fileName).LowerCase();
    if (ext == TEXT(".hra")) {
        FreeBuffer(&imgBuf);
        LoadHraFile(fileName.c_str(), &imgBuf, &bw, &bh, &bytepp);
        pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
    } else {
        TGraphic* img;
        if (ext == TEXT(".jpg") || ext == TEXT(".jpeg")) {
            img = new TJPEGImage();
        } else if (ext == TEXT(".bmp")) {
            img = new Graphics::TBitmap();
        }

        img->LoadFromFile(fileName);
	    Graphics::TBitmap *bmp = new Graphics::TBitmap();
        bmp->Assign(img);

        LoadBitmap(bmp);

        delete bmp;
        delete img;
    }
}
//---------------------------------------------------------------------------

void TFormMain::SaveImageFile(String fileName)
{
    Graphics::TBitmap* bmp = ImageBufferToBitmap(imgBuf, bw, bh, bytepp);
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

    String fileName = dlgOpen->FileName;
    LoadImageFile(fileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Copy1Click(TObject *Sender)
{
    if (imgBuf == NULL) {
        return;
    }

    Graphics::TBitmap* bmp = ImageBufferToBitmap(imgBuf, bw, bh, bytepp);
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

    LoadBitmap(bmp);

    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_LENNA);

    LoadBitmap(bmp);

    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Coins1Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_COINS);

    LoadBitmap(bmp);

    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Chess1Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_CHESS);

    LoadBitmap(bmp);

    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Gradient1Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_GRADIENT);

    LoadBitmap(bmp);

    delete bmp;
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
    FreeBuffer(&imgBuf);
    int cb = bw * bh;
    imgBuf = AllocBuffer(cb);
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

void TFormMain::UserDrawTest(TCanvas* g) {
    double st = GetTimeMs();

    Randomize();
    int step = 20;
    if (DrawEllipse1->Checked) {
        TBrushStyle obs = g->Brush->Style;
        g->Brush->Style = bsClear;
        TColor opc = g->Pen->Color;
        g->Pen->Color = (TColor)RGB(RandomRange(0, 256), RandomRange(0, 256), RandomRange(0, 256));
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
        g->Pen->Color = (TColor)RGB(RandomRange(0, 256), RandomRange(0, 256), RandomRange(0, 256));
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
        g->Font->Color = (TColor)RGB(RandomRange(0, 256), RandomRange(0, 256), RandomRange(0, 256));
        TBrushStyle obs = Canvas->Brush->Style;
        g->Brush->Style = bsClear;
        for (int y = 0; y < 1000; y += step) {
            for (int x = 0; x < 1000; x += step) {
                g->TextOut(x, y, TEXT("128"));
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
        TColor col = (TColor)RGB(RandomRange(0, 256), RandomRange(0, 256), RandomRange(0, 256));
        for (int y = 0; y < 50; y++) {
            for (int x = 0; x < 50; x++) {
                pbxDraw->DrawEllipse(x, y, x + 1, y + 1, col);
            }
        }
    }

    double et = GetTimeMs();
    double ms = et - st;
    String text = FormatString(TEXT("DrawTime : %.2f"), ms);
    pbxDraw->DrawString(text, TPointD(255, 2), false, clBlack, true, clWhite);
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
        ShowMessage(TEXT("imgBuf == NULL"));
        return;
    }

    bool r = dlgSave->Execute();
    if (!r)
        return;

    String fileName = dlgSave->FileName;
    SaveImageFile(fileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CoinsFloat1Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_COINS);

    LoadBitmapFloat(bmp);

    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CoinsDouble1Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_COINS);

    LoadBitmapDouble(bmp);

    delete bmp;
}
//---------------------------------------------------------------------------

