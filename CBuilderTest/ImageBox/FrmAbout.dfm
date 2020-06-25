object FormAbout: TFormAbout
  Left = 511
  Top = 193
  BorderStyle = bsSizeToolWin
  Caption = 'ImageBox for C++Builder'
  ClientHeight = 336
  ClientWidth = 349
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 293
    Width = 349
    Height = 43
    Align = alBottom
    TabOrder = 0
    DesignSize = (
      349
      43)
    object btnOk: TButton
      Left = 183
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Ok'
      ModalResult = 1
      TabOrder = 0
    end
    object btnCancel: TButton
      Left = 264
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object btnSaveBuffer: TButton
      Left = 4
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Save Buffer'
      TabOrder = 2
      OnClick = btnSaveBufferClick
    end
    object btnCopyBuffer: TButton
      Left = 85
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Copy Buffer'
      TabOrder = 3
      OnClick = btnCopyBufferClick
    end
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 349
    Height = 293
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 1
    object TabSheet2: TTabSheet
      Caption = 'Option'
      ImageIndex = 1
      object Label1: TLabel
        Left = 13
        Top = 121
        Width = 125
        Height = 13
        Caption = 'PixelValueDispZoomFactor'
      end
      object Label3: TLabel
        Left = 13
        Top = 230
        Width = 47
        Height = 13
        Caption = 'BackColor'
      end
      object Label2: TLabel
        Left = 8
        Top = 177
        Width = 22
        Height = 13
        Caption = 'Font'
      end
      object Label4: TLabel
        Left = 8
        Top = 202
        Width = 47
        Height = 13
        Caption = 'Pixel Font'
      end
      object Label5: TLabel
        Left = 13
        Top = 148
        Width = 91
        Height = 13
        Caption = 'ZoomLevelMin,Max'
      end
      object chkUseDrawPixelValue: TCheckBox
        Left = 8
        Top = 6
        Width = 130
        Height = 17
        Caption = 'UseDrawPixelValue'
        TabOrder = 0
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseDrawInfo: TCheckBox
        Left = 8
        Top = 29
        Width = 130
        Height = 17
        Caption = 'UseDrawInfo'
        TabOrder = 1
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseDrawCenterLine: TCheckBox
        Left = 8
        Top = 52
        Width = 130
        Height = 17
        Caption = 'UseDrawCenterLine'
        TabOrder = 2
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseDrawDrawTime: TCheckBox
        Left = 8
        Top = 75
        Width = 130
        Height = 17
        Caption = 'UseDrawDrawTime'
        TabOrder = 3
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseInterPorlation: TCheckBox
        Left = 200
        Top = 6
        Width = 130
        Height = 17
        Caption = 'UseInterPorlation'
        TabOrder = 4
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseParallel: TCheckBox
        Left = 200
        Top = 29
        Width = 130
        Height = 17
        Caption = 'UseParallel'
        TabOrder = 5
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseMouseMove: TCheckBox
        Left = 200
        Top = 52
        Width = 130
        Height = 17
        Caption = 'UseMouseMove'
        TabOrder = 6
        OnClick = chkUseDrawPixelValueClick
      end
      object chkUseMouseWheelZoom: TCheckBox
        Left = 200
        Top = 75
        Width = 130
        Height = 17
        Caption = 'UseMouseWheelZoom'
        TabOrder = 7
        OnClick = chkUseDrawPixelValueClick
      end
      object edtPixelValueDispZoomFactor: TEdit
        Left = 200
        Top = 118
        Width = 49
        Height = 21
        TabOrder = 8
        Text = '0'
        OnChange = chkUseDrawPixelValueClick
      end
      object colBackColor: TColorBox
        Left = 69
        Top = 228
        Width = 121
        Height = 22
        TabOrder = 9
        OnChange = chkUseDrawPixelValueClick
      end
      object btnZoomReset: TButton
        Left = 205
        Top = 197
        Width = 121
        Height = 25
        Caption = 'Zoom Reset'
        TabOrder = 10
        OnClick = btnZoomResetClick
      end
      object btnZoomToImage: TButton
        Left = 205
        Top = 228
        Width = 121
        Height = 25
        Caption = 'Zoom to Image'
        TabOrder = 11
        OnClick = btnZoomToImageClick
      end
      object btnFont: TButton
        Left = 69
        Top = 172
        Width = 121
        Height = 25
        TabOrder = 12
        OnClick = btnFontClick
      end
      object btnPixelValueDispFont: TButton
        Left = 69
        Top = 197
        Width = 121
        Height = 25
        TabOrder = 13
        OnClick = btnPixelValueDispFontClick
      end
      object chkUseMousePanClamp: TCheckBox
        Left = 8
        Top = 98
        Width = 130
        Height = 17
        Caption = 'UseMousePanClamp'
        TabOrder = 14
        OnClick = chkUseDrawPixelValueClick
      end
      object edtZoomLevelMin: TEdit
        Left = 200
        Top = 145
        Width = 49
        Height = 21
        TabOrder = 15
        Text = '0'
        OnChange = chkUseDrawPixelValueClick
      end
      object edtZoomLevelMax: TEdit
        Left = 255
        Top = 145
        Width = 49
        Height = 21
        TabOrder = 16
        Text = '0'
        OnChange = chkUseDrawPixelValueClick
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Version'
      object memVersion: TMemo
        Left = 0
        Top = 0
        Width = 341
        Height = 265
        Align = alClient
        Lines.Strings = (
          'memVersion')
        ReadOnly = True
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
  end
  object dlgFont: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Left = 112
  end
  object dlgSave: TSaveDialog
    DefaultExt = 'bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 144
  end
end
