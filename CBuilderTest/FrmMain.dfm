object FormMain: TFormMain
  Left = 345
  Top = 197
  Width = 820
  Height = 624
  Caption = 'ImageBox Test'
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
  object MainMenu1: TMainMenu
    Left = 72
    Top = 8
    object File1: TMenuItem
      Caption = 'File'
      object OpenFile1: TMenuItem
        Caption = 'Open File'
        ShortCut = 16463
        OnClick = OpenFile1Click
      end
      object SaveFile1: TMenuItem
        Caption = 'Save File'
        ShortCut = 16467
        OnClick = SaveFile1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Copy1: TMenuItem
        Caption = 'Copy'
        ShortCut = 16451
        OnClick = Copy1Click
      end
      object PastefromClipboard1: TMenuItem
        Caption = 'Paste'
        ShortCut = 16470
        OnClick = PastefromClipboard1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object N1: TMenuItem
        Caption = 'Lenna'
        ShortCut = 16465
        OnClick = N1Click
      end
      object Coins1: TMenuItem
        Caption = 'Coins'
        ShortCut = 16453
        OnClick = Coins1Click
      end
      object Chess1: TMenuItem
        Caption = 'Chess'
        ShortCut = 16466
        OnClick = Chess1Click
      end
      object Gradient1: TMenuItem
        Caption = 'Gradient'
        ShortCut = 16468
        OnClick = Gradient1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object LongImage1: TMenuItem
        Caption = 'Long Image'
        ShortCut = 16473
        OnClick = LongImage1Click
      end
      object WideImage1: TMenuItem
        Caption = 'Wide Image'
        ShortCut = 16469
        OnClick = WideImage1Click
      end
    end
    object Zoom1: TMenuItem
      Caption = 'Zoom'
      object ZoomReset1: TMenuItem
        Caption = 'Zoom Reset'
        ShortCut = 16432
        OnClick = ZoomReset1Click
      end
      object Zoomtoimage1: TMenuItem
        Caption = 'Zoom to image'
        ShortCut = 16441
        OnClick = Zoomtoimage1Click
      end
    end
    object est1: TMenuItem
      Caption = 'Test'
      object ImmediateDrawTest1: TMenuItem
        Caption = 'Immediate Draw Test'
        ShortCut = 16464
        OnClick = ImmediateDrawTest1Click
      end
      object RetainedDrawTest1: TMenuItem
        AutoCheck = True
        Caption = 'Retained Draw Test'
        ShortCut = 16463
        OnClick = RetainedDrawTest1Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object DrawEllipse1: TMenuItem
        AutoCheck = True
        Caption = 'DrawEllipse'
        Checked = True
        RadioItem = True
        OnClick = DrawEllipse1Click
      end
      object FillEllipse1: TMenuItem
        AutoCheck = True
        Caption = 'FillEllipse'
        RadioItem = True
        OnClick = DrawEllipse1Click
      end
      object DrawString1: TMenuItem
        AutoCheck = True
        Caption = 'DrawString'
        RadioItem = True
        OnClick = DrawEllipse1Click
      end
      object DrawShapes1: TMenuItem
        AutoCheck = True
        Caption = 'Draw Shapes'
        RadioItem = True
        OnClick = DrawEllipse1Click
      end
      object DrawPixelCircles1: TMenuItem
        AutoCheck = True
        Caption = 'Draw Pixel Circles'
        RadioItem = True
        OnClick = DrawEllipse1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object AboutImageBox1: TMenuItem
        Caption = 'About ImageBox'
        OnClick = AboutImageBox1Click
      end
    end
  end
  object dlgOpen: TOpenDialog
    Filter = 
      'All (*.jpg;*.jpeg;*.bmp;*.hra)|*.jpg;*.jpeg;*.bmp;*.hra|JPEG Ima' +
      'ge File (*.jpg)|*.jpg|JPEG Image File (*.jpeg)|*.jpeg|Bitmaps (*' +
      '.bmp)|*.bmp|Hims Raw Images|*.hra'
    Left = 8
    Top = 8
  end
  object dlgSave: TSaveDialog
    DefaultExt = 'bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 40
    Top = 8
  end
end
