#ifndef UtilH
#define UtilH

#include <vcl.h>

template<class T>
T Clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

template<class T>
T Min(T a, T b)
{
    if (a < b) return a;
    return b;
}

template<class T>
T Max(T a, T b)
{
    if (a > b) return a;
    return b;
}

double GetTimeMs();
int TColorToBGRA(TColor color);
HPALETTE CreateGrayPalette();
void BitmapToImageBuffer(Graphics::TBitmap* bmp, BYTE** buf, int* bw, int* bh, int* bytepp);
Graphics::TBitmap* ImageBufferToBitmap(BYTE* buf, int bw, int bh, int bytepp);
void CopyImageBufferZoom(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);
void CopyImageBufferZoomIpl(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel);

#endif
