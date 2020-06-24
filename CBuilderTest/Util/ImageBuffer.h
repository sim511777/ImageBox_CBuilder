//---------------------------------------------------------------------------

#ifndef ImageBufferH
#define ImageBufferH
#include <vcl.h>
//---------------------------------------------------------------------------
class TImageBuffer {
public:
    BYTE* buf;
    int bw;
    int bh;
    int bytepp;
private:
    bool allocated;
public:
    TImageBuffer(TBitmap* bmp);
    TImageBuffer(String filePath);
    TImageBuffer(BYTE* _buf, int _bw, int _bh, int _bytepp, bool _allocated);
    ~TImageBuffer();
};
#endif
