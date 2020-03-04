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
#include <XPMan.hpp>
#include <CategoryButtons.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TSplitter *Splitter1;
    TPanel *Panel2;
    TButton *btnLoadImageFile;
    TButton *btnResetZoom;
    TButton *btnLoadClipboard;
    TButton *btnClearImageBuffer;
    TButton *btnImmediateDrawTest;
    TCheckBox *chkRetainedDrawTest;
    TOpenPictureDialog *dlgOpen;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Zoom1;
    TMenuItem *est1;
    TMenuItem *Help1;
    TMenuItem *OpenFile1;
    TMenuItem *PastefromClipboard1;
    TMenuItem *N1;
    void __fastcall btnLoadImageFileClick(TObject *Sender);
    void __fastcall btnResetZoomClick(TObject *Sender);
    void __fastcall btnLoadClipboardClick(TObject *Sender);
    void __fastcall btnClearImageBufferClick(TObject *Sender);
    void __fastcall btnImmediateDrawTestClick(TObject *Sender);
    void __fastcall chkRetainedDrawTestClick(TObject *Sender);
private:	// User declarations
    TImageBox* pbxDraw;
    void __fastcall pbxDrawOnPaint(TObject *Sender);

    void __fastcall LoadBmp(Graphics::TBitmap* bmp);

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
