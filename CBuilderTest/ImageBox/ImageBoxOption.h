//---------------------------------------------------------------------------

#ifndef ImageBoxOptionH
#define ImageBoxOptionH
#include <vcl.h>
#include "ImageBox.h"
//---------------------------------------------------------------------------
class TImageBoxOption {
public:
    bool UseDrawPixelValue;
    bool UseDrawInfo;
    bool UseDrawCenterLine;
    bool UseDrawDrawTime;
    bool UseInterPorlation;
    bool UseParallel;
    int PixelValueDispZoomFactor;
    bool UseMouseMove;
    bool UseMouseWheelZoom;
    String DrawFontName;
    float DrawFontSize;
    String PixelValueDispFontName;
    float PixelValueDispFontSize;
    TColor BgColor;
    bool UseMousePanClamp;
    int ZoomLevelMin;
    int ZoomLevelMax;
    void FromImageBox(TImageBox* pbx);
    void ToImageBox(TImageBox* pbx);
};
#endif
