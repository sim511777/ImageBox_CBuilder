//---------------------------------------------------------------------------

#ifndef FrmAboutH
#define FrmAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ImageBox.h"
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormAbout : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TButton *btnOk;
    TButton *btnCancel;
    TFontDialog *dlgFont;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TMemo *memVersion;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label2;
    TCheckBox *chkUseDrawPixelValue;
    TCheckBox *chkUseDrawInfo;
    TCheckBox *chkUseDrawCenterLine;
    TCheckBox *chkUseDrawDrawTime;
    TCheckBox *chkUseInterPorlation;
    TCheckBox *chkUseParallel;
    TCheckBox *chkUseMouseMove;
    TCheckBox *chkUseMouseWheelZoom;
    TPanel *btnFont;
    TEdit *edtPixelValueDispZoomFactor;
    TColorBox *colBackColor;
    TButton *btnZoomReset;
    TButton *btnZoomToImage;
    void __fastcall btnFontClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall btnZoomResetClick(TObject *Sender);
    void __fastcall btnZoomToImageClick(TObject *Sender);
private:	// User declarations
    TImageBox *pbx;

    void ImageBoxOptionGet();
    void ImageBoxOptionSet();
public:		// User declarations
    __fastcall TFormAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAbout *FormAbout;
//---------------------------------------------------------------------------
#endif
