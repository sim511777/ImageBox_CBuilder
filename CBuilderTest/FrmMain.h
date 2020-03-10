//---------------------------------------------------------------------------

#ifndef FrmMainH
#define FrmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ImageBox.h"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TOpenPictureDialog *dlgOpen;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Zoom1;
    TMenuItem *est1;
    TMenuItem *Help1;
    TMenuItem *OpenFile1;
    TMenuItem *PastefromClipboard1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *Lenna41;
    TMenuItem *Coins1;
    TMenuItem *Chess1;
    TMenuItem *N3;
    TMenuItem *LongImage1;
    TMenuItem *WideImage1;
    TMenuItem *ZoomReset1;
    TMenuItem *Zoomtoimage1;
    TMenuItem *ImmediateDrawTest1;
    TMenuItem *RetainedDrawTest1;
    TMenuItem *N4;
    TMenuItem *DrawEllipse1;
    TMenuItem *FillEllipse1;
    TMenuItem *DrawString1;
    TMenuItem *DrawShapes1;
    TMenuItem *DrawPixelCircles1;
    TMenuItem *AboutImageBox1;
    TMenuItem *N5;
    TMenuItem *Copy1;
    TMenuItem *Gradient1;
    TMenuItem *SaveFile1;
    TSavePictureDialog *dlgSave;
    void __fastcall OpenFile1Click(TObject *Sender);
    void __fastcall PastefromClipboard1Click(TObject *Sender);
    void __fastcall N1Click(TObject *Sender);
    void __fastcall Lenna41Click(TObject *Sender);
    void __fastcall Coins1Click(TObject *Sender);
    void __fastcall Chess1Click(TObject *Sender);
    void __fastcall LongImage1Click(TObject *Sender);
    void __fastcall WideImage1Click(TObject *Sender);
    void __fastcall ZoomReset1Click(TObject *Sender);
    void __fastcall Zoomtoimage1Click(TObject *Sender);
    void __fastcall ImmediateDrawTest1Click(TObject *Sender);
    void __fastcall RetainedDrawTest1Click(TObject *Sender);
    void __fastcall DrawEllipse1Click(TObject *Sender);
    void __fastcall AboutImageBox1Click(TObject *Sender);
    void __fastcall Copy1Click(TObject *Sender);
    void __fastcall SaveFile1Click(TObject *Sender);
private:	// User declarations
    TImageBox* pbxDraw;
    void __fastcall pbxDrawOnPaint(TObject *Sender);
    void LoadImageFile(AnsiString fileName);
    void SaveImageFile(AnsiString fileName);
    void __fastcall ReadBitmap(Graphics::TBitmap* bmp);
    void __fastcall WriteBitmap(Graphics::TBitmap* bmp);
    void GenerateBitmap(int bw, int bh);

    BYTE* imgBuf;
    int bw;
    int bh;
    int bytepp;

public:		// User declarations
    __fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
