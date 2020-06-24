//---------------------------------------------------------------------------

#pragma hdrstop

#include "ImageBuffer.h"
#include "Util.h"
#include <jpeg.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
TImageBuffer::TImageBuffer(TBitmap* bmp) {
    BitmapToImageBuffer(bmp, &buf, &bw, &bh, &bytepp);
    allocated = true;
}

TImageBuffer::TImageBuffer(String filePath) {
    String ext = ExtractFileExt(filePath).LowerCase();
    if (ext == TEXT(".hra")) {
        LoadHraFile(filePath.c_str(), &buf, &bw, &bh, &bytepp);
    } else {
        TGraphic* img;
        if (ext == TEXT(".jpg") || ext == TEXT(".jpeg")) {
            img = new TJPEGImage();
        } else if (ext == TEXT(".bmp")) {
            img = new Graphics::TBitmap();
        }

        img->LoadFromFile(filePath);
	    Graphics::TBitmap *bmp = new Graphics::TBitmap();
        bmp->Assign(img);

        BitmapToImageBuffer(bmp, &buf, &bw, &bh, &bytepp);

        delete bmp;
        delete img;
    }
    allocated = true;
}

TImageBuffer::TImageBuffer(BYTE* _buf, int _bw, int _bh, int _bytepp, bool _allocated) {
    buf = _buf;
    bw = _bw;
    bh = _bh;
    bytepp = _bytepp;
    allocated = _allocated;
}

TImageBuffer::~TImageBuffer() {
    if (allocated)
        delete[] buf;
}
