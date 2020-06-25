//---------------------------------------------------------------------------

#include <vcl.h>
#include <Clipbrd.hpp>
#pragma hdrstop

#include "FrmAbout.h"
#include "Util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
    : TForm(Owner)
{
    pbx = (TImageBox *)Owner;
    memVersion->Text = TImageBox::VersionHistory;
    opt.FromImageBox(pbx);
    optBackup = opt;
    OptionToForm();
}
//---------------------------------------------------------------------------

bool skipChangeEvent = false;
void TFormAbout::OptionToForm() {
    skipChangeEvent = true;
    chkUseDrawPixelValue->Checked = opt.UseDrawPixelValue;
    chkUseDrawInfo->Checked = opt.UseDrawInfo;
    chkUseDrawCenterLine->Checked = opt.UseDrawCenterLine;
    chkUseDrawDrawTime->Checked = opt.UseDrawDrawTime;
    chkUseInterPorlation->Checked = opt.UseInterPorlation;
    chkUseParallel->Checked = opt.UseParallel;
    edtPixelValueDispZoomFactor->Text = IntToStr(opt.PixelValueDispZoomFactor);
    chkUseMouseMove->Checked = opt.UseMouseMove;
    chkUseMouseWheelZoom->Checked = opt.UseMouseWheelZoom;
    btnFont->Font->Name = opt.DrawFontName;
    btnFont->Font->Size = opt.DrawFontSize;
    btnFont->Caption = btnFont->Font->Name + "," + IntToStr(btnFont->Font->Size);
    btnPixelValueDispFont->Font->Name = opt.PixelValueDispFontName;
    btnPixelValueDispFont->Font->Size = opt.PixelValueDispFontSize;
    btnPixelValueDispFont->Caption = btnPixelValueDispFont->Font->Name + "," + IntToStr(btnPixelValueDispFont->Font->Size);
    colBackColor->Selected = opt.BgColor;
    chkUseMousePanClamp->Checked = opt.UseMousePanClamp;
    edtZoomLevelMin->Text = IntToStr(opt.ZoomLevelMin);
    edtZoomLevelMax->Text = IntToStr(opt.ZoomLevelMax);
    skipChangeEvent = false;
}
//---------------------------------------------------------------------------

void TFormAbout::FormToOption() {
    opt.UseDrawPixelValue = chkUseDrawPixelValue->Checked;
    opt.UseDrawInfo = chkUseDrawInfo->Checked;
    opt.UseDrawCenterLine = chkUseDrawCenterLine->Checked;
    opt.UseDrawDrawTime = chkUseDrawDrawTime->Checked;
    opt.UseInterPorlation = chkUseInterPorlation->Checked;
    opt.UseParallel = chkUseParallel->Checked;
    opt.PixelValueDispZoomFactor = StrToInt(edtPixelValueDispZoomFactor->Text);
    opt.UseMouseMove = chkUseMouseMove->Checked;
    opt.UseMouseWheelZoom = chkUseMouseWheelZoom->Checked;
    opt.BgColor = colBackColor->Selected;
    opt.DrawFontName = btnFont->Font->Name;
    opt.DrawFontSize = btnFont->Font->Size;
    opt.PixelValueDispFontName = btnPixelValueDispFont->Font->Name;
    opt.PixelValueDispFontSize = btnPixelValueDispFont->Font->Size;
    opt.UseMousePanClamp = chkUseMousePanClamp->Checked;
    opt.ZoomLevelMin = StrToInt(edtZoomLevelMin->Text);
    opt.ZoomLevelMax = StrToInt(edtZoomLevelMax->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::btnFontClick(TObject *Sender)
{
    dlgFont->Font->Name = btnFont->Font->Name;
    dlgFont->Font->Size = btnFont->Font->Size;
    if (dlgFont->Execute() == false)
        return;
    btnFont->Font->Name = dlgFont->Font->Name;
    btnFont->Font->Size = dlgFont->Font->Size;
    btnFont->Caption = btnFont->Font->Name + "," + IntToStr(btnFont->Font->Size);
    chkUseDrawPixelValueClick(btnFont);
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::btnPixelValueDispFontClick(TObject *Sender)
{
    dlgFont->Font->Name = btnPixelValueDispFont->Font->Name;
    dlgFont->Font->Size = btnPixelValueDispFont->Font->Size;
    if (dlgFont->Execute() == false)
        return;
    btnPixelValueDispFont->Font->Name = dlgFont->Font->Name;
    btnPixelValueDispFont->Font->Size = dlgFont->Font->Size;
    btnPixelValueDispFont->Caption = btnFont->Font->Name + "," + IntToStr(btnFont->Font->Size);
    chkUseDrawPixelValueClick(dlgFont);
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    try {
        StrToInt(edtPixelValueDispZoomFactor->Text);
        StrToInt(edtZoomLevelMin->Text);
        StrToInt(edtZoomLevelMax->Text);
    } catch (...) {
        ShowMessage("Invalid Value");
        CanClose = false;
        return;
    }
    if (this->ModalResult != mrOk) {
        optBackup.ToImageBox(pbx);
        pbx->Invalidate();
        return;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::btnZoomResetClick(TObject *Sender)
{
    pbx->ZoomReset();
    pbx->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::btnZoomToImageClick(TObject *Sender)
{
    if (pbx->ImgBW <= 0 || pbx->ImgBH <= 0)
        pbx->ZoomReset();
    else
        pbx->ZoomToRect(0, 0, pbx->ImgBW, pbx->ImgBH);
    pbx->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::btnSaveBufferClick(TObject *Sender)
{
    if (pbx->ImgBuf == NULL) {
        ShowMessage("imgBuf == NULL");
        return;
    }

    if (pbx->BufIsFloat) {
        ShowMessage("Floating point image buffer can not be converted to Bitmap object.");
        return;
    }

    bool r = dlgSave->Execute();
    if (!r)
        return;

    Graphics::TBitmap* bmp = ImageBufferToBitmap(pbx->ImgBuf, pbx->ImgBW, pbx->ImgBH, pbx->ImgBytepp);
    bmp->SaveToFile(dlgSave->FileName);
    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::btnCopyBufferClick(TObject *Sender)
{
    if (pbx->ImgBuf == NULL) {
        return;
    }

    if (pbx->BufIsFloat) {
        return;
    }

    Graphics::TBitmap* bmp = ImageBufferToBitmap(pbx->ImgBuf, pbx->ImgBW, pbx->ImgBH, pbx->ImgBytepp);
    Clipboard()->Assign(bmp);
    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::chkUseDrawPixelValueClick(TObject *Sender)
{
    if (skipChangeEvent)
        return;

    FormToOption();
    opt.ToImageBox(pbx);
    pbx->Invalidate();
}
//---------------------------------------------------------------------------


