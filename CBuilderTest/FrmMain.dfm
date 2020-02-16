object FormMain: TFormMain
  Left = 306
  Top = 140
  Width = 1143
  Height = 690
  Caption = 'FormMain'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 875
    Top = 0
    Width = 3
    Height = 651
    Cursor = crHSplit
    Align = alRight
  end
  object Panel1: TPanel
    Left = 878
    Top = 0
    Width = 249
    Height = 651
    Align = alRight
    Caption = 'Panel1'
    TabOrder = 0
    object btnLoadImageFile: TButton
      Left = 7
      Top = 8
      Width = 106
      Height = 25
      Caption = 'Load Image File'
      TabOrder = 0
      OnClick = btnLoadImageFileClick
    end
    object btnResetZoom: TButton
      Left = 119
      Top = 8
      Width = 107
      Height = 25
      Caption = 'Reset Zoom'
      TabOrder = 1
      OnClick = btnResetZoomClick
    end
    object btnLoadClipboard: TButton
      Left = 6
      Top = 39
      Width = 106
      Height = 25
      Caption = 'Load Clipboard Image'
      TabOrder = 2
      OnClick = btnLoadClipboardClick
    end
    object btnClearImageBuffer: TButton
      Left = 6
      Top = 70
      Width = 106
      Height = 25
      Caption = 'Clear Image Buffer'
      TabOrder = 3
      OnClick = btnClearImageBufferClick
    end
    object btnImmediateDrawTest: TButton
      Left = 6
      Top = 101
      Width = 107
      Height = 25
      Caption = 'Immediate Draw Test'
      TabOrder = 4
      OnClick = btnImmediateDrawTestClick
    end
    object chkRetainedDrawTest: TCheckBox
      Left = 8
      Top = 136
      Width = 121
      Height = 17
      Caption = 'Retained Draw Test'
      TabOrder = 5
      OnClick = chkRetainedDrawTestClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 875
    Height = 651
    Align = alClient
    Caption = 'Panel2'
    TabOrder = 1
  end
  object dlgOpen: TOpenPictureDialog
    Left = 8
    Top = 8
  end
end
