//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <Clipbrd.hpp>
#include <DateUtils.hpp>
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
}

void __fastcall TFormMain::btnResetZoomClick(TObject *Sender)
{
    pbxDraw->ZoomReset();
    pbxDraw->Invalidate();
}
//---------------------------------------------------------------------------

void BitmapToBuf(Graphics::TBitmap* bmp, BYTE** pbuf, int* pbw, int* pbh, int* pbytepp)
{
    if (*pbuf != NULL)
        delete[] *pbuf;

    *pbw = bmp->Width;
    *pbh = bmp->Height;
    if (bmp->PixelFormat == pf8bit)
        *pbytepp = 1;
    else if (bmp->PixelFormat == pf16bit)
        *pbytepp = 2;
    else if (bmp->PixelFormat == pf24bit)
        *pbytepp = 3;
    else
        *pbytepp = 4;

    int bufstep = *pbw * *pbytepp;
    *pbuf = new BYTE[bufstep * *pbh];

    for (int y = 0; y < *pbh; y++) {
        BYTE* sptr = (BYTE*)bmp->ScanLine[y];
        BYTE* dptr = *pbuf + bufstep * y;
        memcpy(dptr, sptr, bufstep);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadImageFileClick(TObject *Sender)
{
    bool r = dlgOpen->Execute();
    if (!r)
        return;

	Graphics::TBitmap *bmp = new Graphics::TBitmap();
	bmp->LoadFromFile(dlgOpen->FileName);
    BitmapToBuf(bmp, &imgBuf, &bw, &bh, &bytepp);
    delete bmp;

    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadClipboardClick(TObject *Sender)
{
    TClipboard* clip = Clipboard();
    if (clip->HasFormat(CF_BITMAP) == false)
        return;

	Graphics::TBitmap *bmp = new Graphics::TBitmap();
    bmp->LoadFromClipboardFormat(CF_BITMAP, clip->GetAsHandle(CF_BITMAP), 0);
    BitmapToBuf(bmp, &imgBuf, &bw, &bh, &bytepp);
    delete bmp;

    pbxDraw->SetImgBuf(imgBuf, bw, bh, bytepp, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnClearImageBufferClick(TObject *Sender)
{
    delete imgBuf;
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

