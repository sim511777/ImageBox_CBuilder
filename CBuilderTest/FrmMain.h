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
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TSplitter *Splitter1;
    TPanel *Panel2;
    TButton *btnLoadImageFile;
    TOpenPictureDialog *dlgOpen;
    TButton *btnResetZoom;
    TButton *btnLoadClipboard;
    TButton *btnClearImageBuffer;
    TButton *btnDrawCircles;
    void __fastcall btnLoadImageFileClick(TObject *Sender);
    void __fastcall btnResetZoomClick(TObject *Sender);
    void __fastcall btnLoadClipboardClick(TObject *Sender);
    void __fastcall btnClearImageBufferClick(TObject *Sender);
    void __fastcall btnDrawCirclesClick(TObject *Sender);
private:	// User declarations
    TImageBox* pbxDraw;
    void __fastcall pbxDrawOnPaint(TObject *Sender);

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
