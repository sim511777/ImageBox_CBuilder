object FormAbout: TFormAbout
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = 'ImageBox for C++Builder'
  ClientHeight = 477
  ClientWidth = 401
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
  object memVersion: TMemo
    Left = 0
    Top = 0
    Width = 401
    Height = 240
    Align = alClient
    Lines.Strings = (
      'memVersion')
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 1
    ExplicitWidth = 355
    ExplicitHeight = 121
  end
  object Panel2: TPanel
    Left = 0
    Top = 432
    Width = 401
    Height = 45
    Align = alBottom
    TabOrder = 2
    ExplicitWidth = 446
    DesignSize = (
      401
      45)
    object btnOk: TButton
      Left = 235
      Top = 12
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Ok'
      ModalResult = 1
      TabOrder = 0
      ExplicitLeft = 280
      ExplicitTop = 20
    end
    object btnCancel: TButton
      Left = 316
      Top = 12
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      ExplicitLeft = 361
      ExplicitTop = 20
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 240
    Width = 401
    Height = 192
    Align = alBottom
    TabOrder = 0
    ExplicitTop = 234
    object Label1: TLabel
      Left = 8
      Top = 98
      Width = 125
      Height = 13
      Caption = 'PixelValueDispZoomFactor'
    end
    object Label3: TLabel
      Left = 8
      Top = 162
      Width = 47
      Height = 13
      Caption = 'BackColor'
    end
    object Label2: TLabel
      Left = 8
      Top = 130
      Width = 22
      Height = 13
      Caption = 'Font'
    end
    object chkUseDrawPixelValue: TCheckBox
      Left = 8
      Top = 6
      Width = 130
      Height = 17
      Caption = 'UseDrawPixelValue'
      TabOrder = 0
    end
    object chkUseDrawInfo: TCheckBox
      Left = 8
      Top = 29
      Width = 130
      Height = 17
      Caption = 'UseDrawInfo'
      TabOrder = 1
    end
    object chkUseDrawCenterLine: TCheckBox
      Left = 8
      Top = 52
      Width = 130
      Height = 17
      Caption = 'UseDrawCenterLine'
      TabOrder = 2
    end
    object chkUseDrawDrawTime: TCheckBox
      Left = 8
      Top = 75
      Width = 130
      Height = 17
      Caption = 'UseDrawDrawTime'
      TabOrder = 3
    end
    object chkUseInterPorlation: TCheckBox
      Left = 184
      Top = 6
      Width = 130
      Height = 17
      Caption = 'UseInterPorlation'
      TabOrder = 4
    end
    object chkUseParallel: TCheckBox
      Left = 184
      Top = 29
      Width = 130
      Height = 17
      Caption = 'UseParallel'
      TabOrder = 5
    end
    object chkUseMouseMove: TCheckBox
      Left = 184
      Top = 52
      Width = 130
      Height = 17
      Caption = 'UseMouseMove'
      TabOrder = 6
    end
    object chkUseMouseWheelZoom: TCheckBox
      Left = 184
      Top = 75
      Width = 130
      Height = 17
      Caption = 'UseMouseWheelZoom'
      TabOrder = 7
    end
    object btnFont: TPanel
      Left = 184
      Top = 125
      Width = 121
      Height = 26
      TabOrder = 8
      OnClick = btnFontClick
    end
    object btnBackColor: TPanel
      Left = 184
      Top = 157
      Width = 121
      Height = 26
      TabOrder = 9
      OnClick = btnBackColorClick
    end
    object edtPixelValueDispZoomFactor: TEdit
      Left = 184
      Top = 98
      Width = 49
      Height = 21
      TabOrder = 10
      Text = '0'
    end
  end
  object dlgFont: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Left = 8
    Top = 16
  end
  object dlgBackColor: TColorDialog
    Left = 40
    Top = 16
  end
end
