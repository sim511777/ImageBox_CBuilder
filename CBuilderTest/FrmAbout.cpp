//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrmAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
    : TForm(Owner)
{
    pbx = (TImageBox *)Owner;
    memVersion->Text = TImageBox::VersionHistory;
    ImageBoxOptionGet();
}
//---------------------------------------------------------------------------

void TFormAbout::ImageBoxOptionGet() {
    chkUseDrawPixelValue->Checked = pbx->UseDrawPixelValue;
    chkUseDrawInfo->Checked = pbx->UseDrawInfo;
    chkUseDrawCenterLine->Checked = pbx->UseDrawCenterLine;
    chkUseDrawDrawTime->Checked = pbx->UseDrawDrawTime;
    chkUseInterPorlation->Checked = pbx->UseInterPorlation;
    chkUseParallel->Checked = pbx->UseParallel;
    edtPixelValueDispZoomFactor->Text = IntToStr(pbx->PixelValueDispZoomFactor);
    chkUseMouseMove->Checked = pbx->UseMouseMove;
    chkUseMouseWheelZoom->Checked = pbx->UseMouseWheelZoom;
    colBackColor->Selected = pbx->BgColor;
    btnFont->Font->Name = pbx->DrawFont->Name;
    btnFont->Font->Size = pbx->DrawFont->Size;

    btnFont->Caption = btnFont->Font->Name + "," + IntToStr(btnFont->Font->Size);
}
//---------------------------------------------------------------------------

void TFormAbout::ImageBoxOptionSet() {
    pbx->UseDrawPixelValue = chkUseDrawPixelValue->Checked;
    pbx->UseDrawInfo = chkUseDrawInfo->Checked;
    pbx->UseDrawCenterLine = chkUseDrawCenterLine->Checked;
    pbx->UseDrawDrawTime = chkUseDrawDrawTime->Checked;
    pbx->UseInterPorlation = chkUseInterPorlation->Checked;
    pbx->UseParallel = chkUseParallel->Checked;
    pbx->PixelValueDispZoomFactor = StrToInt(edtPixelValueDispZoomFactor->Text);
    pbx->UseMouseMove = chkUseMouseMove->Checked;
    pbx->UseMouseWheelZoom = chkUseMouseWheelZoom->Checked;
    pbx->BgColor = colBackColor->Selected;
    pbx->DrawFont->Name = btnFont->Font->Name;
    pbx->DrawFont->Size = btnFont->Font->Size;
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
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    try {
        int check = StrToInt(edtPixelValueDispZoomFactor->Text);
    } catch (...) {
        ShowMessage("Invalid PixelValueDispZoomFactor");
        CanClose = false;
        return;
    }
    if (this->ModalResult != mrOk)
        return;

    ImageBoxOptionSet();
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

