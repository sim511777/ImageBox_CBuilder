object FormMain: TFormMain
  Left = 306
  Top = 140
  Caption = 'FormMain'
  ClientHeight = 651
  ClientWidth = 1127
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 875
    Top = 0
    Height = 651
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
    ExplicitLeft = 881
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
  object MainMenu1: TMainMenu
    Left = 40
    Top = 8
    object File1: TMenuItem
      Caption = 'File'
      object OpenFile1: TMenuItem
        Caption = 'Open File'
      end
      object PastefromClipboard1: TMenuItem
        Caption = 'Paste from Clipboard'
        ShortCut = 16470
      end
      object N1: TMenuItem
        Caption = '--'
      end
    end
    object Zoom1: TMenuItem
      Caption = 'Zoom'
    end
    object est1: TMenuItem
      Caption = 'Test'
    end
    object Help1: TMenuItem
      Caption = 'Help'
    end
  end
end
