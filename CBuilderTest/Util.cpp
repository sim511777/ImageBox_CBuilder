//---------------------------------------------------------------------------
#include <Math.h>
#include <stdio.h>
#pragma hdrstop

#include "Util.h"

#pragma package(smart_init)

double GetTimeMs() {
    static LARGE_INTEGER freq;
    static BOOL r = QueryPerformanceFrequency(&freq);
    LARGE_INTEGER cnt;
    QueryPerformanceCounter(&cnt);
    return cnt.QuadPart * 1000.0 / freq.QuadPart;
}

void FreeBuffer(BYTE** buf) {
    if (*buf != NULL) {
        delete[] *buf;
        *buf = NULL;
    }
}

BYTE* AllocBuffer(size_t size) {
    BYTE* buf = new BYTE[size];
    memset(buf, 0, size);
    return buf;
}

void FloatBufToByte(void* floatBuf, int bw, int bh, int bytepp, void* byteBuf) {
    for (int y = 0; y < bh; y++) {
        BYTE* psrc = (BYTE*)floatBuf + (bw * y) * bytepp;
        BYTE* pdst = (BYTE*)byteBuf + bw * y;
        for (int x = 0; x < bw; x++, pdst++, psrc += bytepp) {
            if (bytepp == 4)
                *pdst = (BYTE)Clamp(*(float*)psrc, 0.0f, 255.0f);
            else if (bytepp == 8)
                *pdst = (BYTE)Clamp(*(double*)psrc, 0.0, 255.0);
        }
    }
}

int TColorToBGRA(TColor color) {
    int r = color & 0xff;
    int g = (color >> 8) & 0xff;
    int b = (color >> 16) & 0xff;
    return (b | g << 8 | r << 16 | 0xff << 24);
}

HPALETTE CreateGrayPalette() {
    BYTE* buf = AllocBuffer(sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 255);
    LOGPALETTE* ppal = (LOGPALETTE*)buf;
    ppal->palVersion = 0x300;
    ppal->palNumEntries = 256;
    for (int i = 0; i < 256; i++) {
        ppal->palPalEntry[i].peRed = i;
        ppal->palPalEntry[i].peGreen = i;
        ppal->palPalEntry[i].peBlue = i;
        ppal->palPalEntry[i].peFlags = PC_RESERVED;
    }
    HPALETTE hPal = CreatePalette(ppal);
    FreeBuffer(&buf);
    return hPal;
}

void BitmapToImageBuffer(Graphics::TBitmap* bmp, BYTE** buf, int* bw, int* bh, int* bytepp)
{
    *bw = bmp->Width;
    *bh = bmp->Height;
    if (bmp->PixelFormat == pf8bit)
        *bytepp = 1;
    else if (bmp->PixelFormat == pf16bit)
        *bytepp = 2;
    else if (bmp->PixelFormat == pf24bit)
        *bytepp = 3;
    else
        *bytepp = 4;

    *buf = AllocBuffer(*bw * *bh * *bytepp);

    int scanstep = *bw * *bytepp;
    for (int y = 0; y < *bh; y++) {
        BYTE* sptr = (BYTE*)bmp->ScanLine[y];
        BYTE* dptr = *buf + scanstep * y;
        memcpy(dptr, sptr, scanstep);
    }
}

Graphics::TBitmap* ImageBufferToBitmap(BYTE* buf, int bw, int bh, int bytepp)
{
    TPixelFormat pixelFormat;
    if (bytepp == 1)
        pixelFormat = pf8bit;
    else if (bytepp == 3)
        pixelFormat = pf24bit;
    else if (bytepp == 4)
        pixelFormat = pf32bit;
    else return NULL;

    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->Width = bw;
    bmp->Height = bh;
    bmp->PixelFormat = pixelFormat;
    if (bmp->PixelFormat == pf8bit) {
        HPALETTE hPal = CreateGrayPalette();
        DeleteObject(bmp->Palette);
        bmp->Palette = hPal;
    }
    int copySize = bw * bytepp;
    int stride = (copySize + 3) / 4 * 4;
    int paddingSize = stride - copySize;
    BYTE paddingBuf[] = { 0, 0, 0, 0 };
    for (int y = 0; y < bh; y++) {
        BYTE* sptr = buf + copySize * y;
        BYTE* dptr = (BYTE*)bmp->ScanLine[y];
        memcpy(dptr, sptr, copySize);
        if (paddingSize > 0)
            memcpy(dptr + copySize, paddingBuf, paddingSize);
    }

    return bmp;
}

// hra Lolad
BOOL LoadHraFile(char* filePath, BYTE** imgBuf, int* bw, int* bh, int* bytepp) {
    FILE* fp = fopen(filePath, "rb");
    if(fp == NULL)   // 파일오픈 체크
        return FALSE;

    fseek(fp, 252, SEEK_SET);
    fread(bytepp, sizeof(int), 1, fp);
    fread(bw, sizeof(int), 1, fp);
    fread(bh, sizeof(int), 1, fp);

    int bufSize = *bw * *bh * *bytepp;
    *imgBuf = AllocBuffer(bufSize);
    fread(*imgBuf, 1, bufSize, fp);

    fclose(fp);
    return TRUE;
}

// hra save
BOOL SaveHraFile(char* filePath, BYTE* imgBuf, int bw, int bh, int bytepp) {
    FILE* fp = fopen(filePath, "wb");
    if(fp == NULL)   // 파일오픈 체크
        return FALSE;

    BYTE b = 0;
    for (int i = 0; i < 252; i++)
        fwrite(&b, sizeof(BYTE), 1, fp);
    fwrite(&bytepp, sizeof(int), 1, fp);
    fwrite(&bw, sizeof(int), 1, fp);
    fwrite(&bh, sizeof(int), 1, fp);

    int bufSize = bw * bh * bytepp;
    fwrite(imgBuf, sizeof(BYTE), bufSize, fp);

    fclose(fp);
    return TRUE;
}

void CopyImageBufferZoom(void* sbuf, int sbw, int sbh,Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel) {
    // 인덱스 버퍼 생성
    int* siys = new int[dbh];
    int* sixs = new int[dbw];
    for (int y = 0; y < dbh; y++) {
        int siy = (int)floor((y - pany) / zoom);
        siys[y] = (sbuf == NULL || siy < 0 || siy >= sbh) ? -1 : siy;
    }
    for (int x = 0; x < dbw; x++) {
        int six = (int)floor((x - panx) / zoom);
        sixs[x] = (sbuf == NULL || six < 0 || six >= sbw) ? -1 : six;
    }

    // dst 범위만큼 루프를 돌면서 해당 픽셀값 쓰기
    for (int y = 0; y < dbh; y++) {
        int siy = siys[y];
        BYTE* sptr = (BYTE*)sbuf + (INT64)sbw * siy * bytepp;
        int* dp = (int*)dbuf->ScanLine[y];
        for (int x = 0; x < dbw; x++, dp++) {
            int six = sixs[x];
            if (siy == -1 || six == -1) {   // out of boundary of image
                *dp = bgColor;
            } else {
                BYTE* sp = &sptr[six * bytepp];
                if (bytepp == 1) {          // 8bit gray
                    int v = sp[0];
                    *dp = v | v << 8 | v << 16 | 0xff << 24;
                } else if (bytepp == 2) {   // 16bit gray (*.hra)
                    int v = sp[0];
                    *dp = v | v << 8 | v << 16 | 0xff << 24;
                } else if (bytepp == 3) {   // 24bit bgr
                    *dp = sp[0] | sp[1] << 8 | sp[2] << 16 | 0xff << 24;
                } else if (bytepp == 4) {   // 32bit bgra
                    *dp = sp[0] | sp[1] << 8 | sp[2] << 16 | 0xff << 24;
                }
            }
        }
    }

	delete[] siys;
	delete[] sixs;
}

void CopyImageBufferZoomFloat(void* sbuf, int sbw, int sbh,Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel) {
    // 인덱스 버퍼 생성
    int* siys = new int[dbh];
    int* sixs = new int[dbw];
    for (int y = 0; y < dbh; y++) {
        int siy = (int)floor((y - pany) / zoom);
        siys[y] = (sbuf == NULL || siy < 0 || siy >= sbh) ? -1 : siy;
    }
    for (int x = 0; x < dbw; x++) {
        int six = (int)floor((x - panx) / zoom);
        sixs[x] = (sbuf == NULL || six < 0 || six >= sbw) ? -1 : six;
    }

    // dst 범위만큼 루프를 돌면서 해당 픽셀값 쓰기
    for (int y = 0; y < dbh; y++) {
        int siy = siys[y];
        BYTE* sptr = (BYTE*)sbuf + (INT64)sbw * siy * bytepp;
        int* dp = (int*)dbuf->ScanLine[y];
        for (int x = 0; x < dbw; x++, dp++) {
            int six = sixs[x];
            if (siy == -1 || six == -1) {   // out of boundary of image
                *dp = bgColor;
            } else {
                if (bytepp == 4) {
                    float* sp = (float*)&sptr[six * bytepp];
                    int v = Clamp((int)*sp, 0, 255);
                    *dp = v | v << 8 | v << 16 | 0xff << 24;
                } else if (bytepp == 8) {
                    double* sp = (double*)&sptr[six * bytepp];
                    int v = Clamp((int)*sp, 0, 255);
                    *dp = v | v << 8 | v << 16 | 0xff << 24;
                }
            }
        }
    }

	delete[] siys;
	delete[] sixs;
}

void CopyImageBufferZoomIpl(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel) {
	// 인덱스 버퍼 생성
	int* siy0s = new int[dbh];
	int* siy1s = new int[dbh];
	int* six0s = new int[dbw];
	int* six1s = new int[dbw];
	int* sity0s = new int[dbh];
	int* sity1s = new int[dbh];
	int* sitx0s = new int[dbw];
	int* sitx1s = new int[dbw];
	for (int y = 0; y < dbh; y++) {
		double siy = (y + 0.5 - pany) / zoom - 0.5;
		if (sbuf == NULL || siy < -0.5 || siy >= sbh -0.5) {
			siy0s[y] = -1;
			continue;
		}
		int siy0 = (int)floor(siy);
		int siy1 = siy0 + 1;
		sity0s[y] = (int)((siy1 - siy) * 256);
		sity1s[y] = (int)((siy - siy0) * 256);
		siy0s[y] = Clamp(siy0, 0, sbh - 1);
		siy1s[y] = Clamp(siy1, 0, sbh - 1);
	}
	for (int x = 0; x < dbw; x++) {
		double six = (x + 0.5 - panx) / zoom - 0.5;
		if (sbuf == NULL || six < -0.5 || six >= sbw - 0.5) {
			six0s[x] = -1;
			continue;
		}
		int six0 = (int)floor(six);
		int six1 = six0 + 1;
		sitx0s[x] = (int)((six1 - six) * 256);
		sitx1s[x] = (int)((six - six0) * 256);
		six0s[x] = Clamp(six0, 0, sbw - 1);
		six1s[x] = Clamp(six1, 0, sbw - 1);
	}

	// dst 범위만큼 루프를 돌면서 해당 픽셀값 쓰기
	for (int y = 0; y < dbh; y++) {
		int siy0 = siy0s[y];
		int siy1 = siy1s[y];
		BYTE* sptr0 = (BYTE*)sbuf + (INT64)sbw * siy0 * bytepp;
		BYTE* sptr1 = (BYTE*)sbuf + (INT64)sbw * siy1 * bytepp;
        int* dp = (int*)dbuf->ScanLine[y];
		int ty0 = sity0s[y];
		int ty1 = sity1s[y];
		for (int x = 0; x < dbw; x++, dp++) {
			int six0 = six0s[x];
			int six1 = six1s[x];
			if (siy0 == -1 || six0 == -1) {   // out of boundary of image
				*dp = bgColor;
			} else {
				BYTE* sp00 = &sptr0[six0 * bytepp];
				BYTE* sp01 = &sptr0[six1 * bytepp];
				BYTE* sp10 = &sptr1[six0 * bytepp];
				BYTE* sp11 = &sptr1[six1 * bytepp];
				int tx0 = sitx0s[x];
				int tx1 = sitx1s[x];
				int t00 = ty0 * tx0;
				int t01 = ty0 * tx1;
				int t10 = ty1 * tx0;
				int t11 = ty1 * tx1;
				if (bytepp == 1) {          // 8bit gray
					int v = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					*dp = v | v << 8 | v << 16 | 0xff << 24;
				} else if (bytepp == 2) {   // 16bit gray (*.hra)
					int v = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					*dp = v | v << 8 | v << 16 | 0xff << 24;
				} else if (bytepp == 3) {   // 24bit bgr
					int b = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					int g = (sp00[1] * t00 + sp01[1] * t01 + sp10[1] * t10 + sp11[1] * t11) >> 16;
					int r = (sp00[2] * t00 + sp01[2] * t01 + sp10[2] * t10 + sp11[2] * t11) >> 16;
					*dp = b | g << 8 | r << 16 | 0xff << 24;
				} else if (bytepp == 4) {   // 32bit bgra
					int b = (sp00[0] * t00 + sp01[0] * t01 + sp10[0] * t10 + sp11[0] * t11) >> 16;
					int g = (sp00[1] * t00 + sp01[1] * t01 + sp10[1] * t10 + sp11[1] * t11) >> 16;
					int r = (sp00[2] * t00 + sp01[2] * t01 + sp10[2] * t10 + sp11[2] * t11) >> 16;
					*dp = b | g << 8 | r << 16 | 0xff << 24;
				}
			}
		}
	}

	delete[] siy0s;
	delete[] siy1s;
	delete[] six0s;
	delete[] six1s;
	delete[] sity0s;
	delete[] sity1s;
	delete[] sitx0s;
	delete[] sitx1s;
}

void CopyImageBufferZoomIplFloat(void* sbuf, int sbw, int sbh, Graphics::TBitmap* dbuf, int dbw, int dbh, INT64 panx, INT64 pany, double zoom, int bytepp, int bgColor, bool useParallel) {
	// 인덱스 버퍼 생성
	int* siy0s = new int[dbh];
	int* siy1s = new int[dbh];
	int* six0s = new int[dbw];
	int* six1s = new int[dbw];
	double* sity0s = new double[dbh];
	double* sity1s = new double[dbh];
	double* sitx0s = new double[dbw];
	double* sitx1s = new double[dbw];
	for (int y = 0; y < dbh; y++) {
		double siy = (y + 0.5 - pany) / zoom - 0.5;
		if (sbuf == NULL || siy < -0.5 || siy >= sbh -0.5) {
			siy0s[y] = -1;
			continue;
		}
		int siy0 = (int)floor(siy);
		int siy1 = siy0 + 1;
		sity0s[y] = siy1 - siy;
		sity1s[y] = siy - siy0;
		siy0s[y] = Clamp(siy0, 0, sbh - 1);
		siy1s[y] = Clamp(siy1, 0, sbh - 1);
	}
	for (int x = 0; x < dbw; x++) {
		double six = (x + 0.5 - panx) / zoom - 0.5;
		if (sbuf == NULL || six < -0.5 || six >= sbw - 0.5) {
			six0s[x] = -1;
			continue;
		}
		int six0 = (int)floor(six);
		int six1 = six0 + 1;
		sitx0s[x] = six1 - six;
		sitx1s[x] = six - six0;
		six0s[x] = Clamp(six0, 0, sbw - 1);
		six1s[x] = Clamp(six1, 0, sbw - 1);
	}

	// dst 범위만큼 루프를 돌면서 해당 픽셀값 쓰기
	for (int y = 0; y < dbh; y++) {
		int siy0 = siy0s[y];
		int siy1 = siy1s[y];
		BYTE* sptr0 = (BYTE*)sbuf + (INT64)sbw * siy0 * bytepp;
		BYTE* sptr1 = (BYTE*)sbuf + (INT64)sbw * siy1 * bytepp;
        int* dp = (int*)dbuf->ScanLine[y];
		double ty0 = sity0s[y];
		double ty1 = sity1s[y];
		for (int x = 0; x < dbw; x++, dp++) {
			int six0 = six0s[x];
			int six1 = six1s[x];
			if (siy0 == -1 || six0 == -1) {   // out of boundary of image
				*dp = bgColor;
			} else {
				BYTE* sp00 = &sptr0[six0 * bytepp];
				BYTE* sp01 = &sptr0[six1 * bytepp];
				BYTE* sp10 = &sptr1[six0 * bytepp];
				BYTE* sp11 = &sptr1[six1 * bytepp];
                double tx0 = sitx0s[x];
                double tx1 = sitx1s[x];
                double t00 = ty0 * tx0;
                double t01 = ty0 * tx1;
                double t10 = ty1 * tx0;
                double t11 = ty1 * tx1;
                if (bytepp == 4) {
                    double v = *(float*)sp00 * t00 + *(float*)sp01 * t01 + *(float*)sp10 * t10 + *(float*)sp11 * t11;
                    int iv = Clamp((int)v, 0, 255);
                    *dp = iv | iv << 8 | iv << 16 | 0xff << 24;
                } else if (bytepp == 8) {
                    double v = *(double*)sp00 * t00 + *(double*)sp01 * t01 + *(double*)sp10 * t10 + *(double*)sp11 * t11;
                    int iv = Clamp((int)v, 0, 255);
                    *dp = iv | iv << 8 | iv << 16 | 0xff << 24;
                }
			}
		}
	}

	delete[] siy0s;
	delete[] siy1s;
	delete[] six0s;
	delete[] six1s;
	delete[] sity0s;
	delete[] sity1s;
	delete[] sitx0s;
	delete[] sitx1s;
}
