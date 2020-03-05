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

void __fastcall TFormMain::btnResetZoomClick(TObject *Sender)
{
    pbxDraw->ZoomReset();
    pbxDraw->Invalidate();
}
//---------------------------------------------------------------------------

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

void __fastcall TFormMain::LoadBmp(Graphics::TBitmap* bmp) {
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
    memset(imgBuf, 0, bw * bh * bytepp);
    BitmapToBuf(bmp, imgBuf, bw, bh, bytepp);
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

    LoadBmp(bmp);

    delete bmp;
    delete img;
    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadImageFileClick(TObject *Sender)
{
    bool r = dlgOpen->Execute();
    if (!r)
        return;

    AnsiString fileName = dlgOpen->FileName;
    LoadImageFile(fileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadClipboardClick(TObject *Sender)
{
    TClipboard* clip = Clipboard();
    if (clip->HasFormat(CF_BITMAP) == false)
        return;

	Graphics::TBitmap *bmp = new Graphics::TBitmap();
    bmp->LoadFromClipboardFormat(CF_BITMAP, clip->GetAsHandle(CF_BITMAP), 0);

    LoadBmp(bmp);

    delete bmp;
    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnClearImageBufferClick(TObject *Sender)
{
    delete[] imgBuf;
    imgBuf = NULL;
    pbxDraw->SetImgBuf(NULL, 0, 0, 1, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::pbxDrawOnPaint(TObject *Sender)
{
    if (chkRetainedDrawTest->Checked) {
        for (int y = 0; y < 100; y++) {
            for (int x = 0; x < 100; x++) {
                pbxDraw->DrawEllipse(x, y, x + 1, y + 1, clRed);
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnImmediateDrawTestClick(TObject *Sender)
{
    TDateTime st = Now();
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            pbxDraw->DrawEllipse(x, y, x + 1, y + 1, clLime);
        }
    }
    INT64 dt = MilliSecondsBetween(Now(), st);
    AnsiString text = AnsiString().sprintf("%d", dt);
    pbxDraw->DrawStringScreen(text, 200, 0, clBlack, true, clWhite);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::chkRetainedDrawTestClick(TObject *Sender)
{
    pbxDraw->Invalidate();
}
//---------------------------------------------------------------------------

