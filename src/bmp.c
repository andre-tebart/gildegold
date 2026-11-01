/* bmp.c -- original file name unknown: Windows bitmap loader and writer ("!bmp:" strings at
   0x5f1084..0x5f10f8). Extent in GildeGold.exe (inferred): 0x401410..0x401d50.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/bmp.c); boundaries in decl/text.dcl */
#include <stdlib.h>
#include <windows.h>
#include "bmp.h"
#include "qnt.h"
#include "mem.h"

/* the engine's inline-asm clear (D3_ZERO in d3.h, which needs the DirectX headers) */
#define BMP_CAT(a, b) a##b
#define BMP_LABEL(name, n) BMP_CAT(name, n)
#define BMP_ZERO(dst, size) {                                       \
    int _zn = (size); void *_zp = (dst);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle BMP_LABEL(bmp_zero, __LINE__)                         \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm BMP_LABEL(bmp_zero, __LINE__): add ecx, ebx               \
    __asm rep stosb                                                 \
}

/* byte fill (D3E_FILL in d3_engine.h): value and size through temps; the destination is assigned to the
   caller's pointer variable dvar inside the block (a block-local temp never shared the x slot: VC6's code
   needs this form, match 2026-10-23) */
#define BMP_FILL(dvar, dst, val, size) {                            \
    int _fn = (size); int _fv = (val); dvar = (dst);                \
    __asm mov edi, dvar                                             \
    __asm mov eax, _fv                                              \
    __asm mov ebx, _fn                                              \
    __asm mov ah, al                                                \
    __asm shl eax, 16                                               \
    __asm mov ecx, eax                                              \
    __asm shr ecx, 16                                               \
    __asm add eax, ecx                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle BMP_LABEL(bmp_fill, __LINE__)                         \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm BMP_LABEL(bmp_fill, __LINE__): add ecx, ebx               \
    __asm rep stosb                                                 \
}

/* copy (D3_COPY in d3.h); all three named directly in the asm */
#define BMP_COPY(dst, src, size) {                                  \
    __asm mov edi, dst                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, size                                             \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle BMP_LABEL(bmp_copy, __LINE__)                         \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm BMP_LABEL(bmp_copy, __LINE__): add ecx, ebx               \
    __asm rep movsb                                                 \
}

/* 0x401410, size and depth of an uncompressed or RLE8 8-bit or 24-bit bitmap */
unsigned char bmp_GetInfo(char *path, int *w, int *h, int *bpp)
{
    BITMAPINFOHEADER bi;
    unsigned char ret = 0;
    VFSFILE *f;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fseek(f, sizeof(BITMAPFILEHEADER), SEEK_SET);
        vfs_fread(&bi, 1, sizeof(bi), f);
        vfs_fclose(f);
        if (bi.biPlanes == 1 && (bi.biBitCount == 8 || bi.biBitCount == 24)
            && (bi.biCompression == BI_RLE8 || bi.biCompression == BI_RGB)) {
            if (w)
                *w = bi.biWidth;
            if (h)
                *h = abs(bi.biHeight);
            if (bpp)
                *bpp = bi.biBitCount;
            ret = 1;
        }
    }
    return ret;
}
/* 0x4014d0, loads a bitmap into data (and its palette into pal), converting as the BMP_* flags say */

unsigned char bmp_Load(char *path, unsigned char *data, unsigned char *pal, int flags)
{
    unsigned char qpal[3][256];
    BITMAPINFOHEADER bi;
    unsigned char *buf = 0;
    unsigned char *bpal = 0;
    unsigned char *tmp;
    unsigned char *top, *bot;
    unsigned char rle[2];
    VFSFILE *f;
    char is24, want24, load, getpal;
    int pitch, dither, height;
    int i, x, y, n;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (!f)
        goto fail;
    vfs_fseek(f, sizeof(BITMAPFILEHEADER), SEEK_SET);
    vfs_fread(&bi, 1, sizeof(bi), f);
    if (!data)
        flags &= ~BMP_LOAD;
    if ((flags & BMP_RGBA) == BMP_RGBA)
        flags |= BMP_RGB;
    if (!pal || ((flags & BMP_RGB) == BMP_RGB && (flags & BMP_PAL) == BMP_PAL))
        flags &= ~BMP_PAL;
    is24 = 0;
    want24 = 0;
    load = 0;
    getpal = 0;
    if (bi.biPlanes == 1) {
        if (bi.biBitCount == 24 && bi.biCompression == BI_RGB)
            is24 = 1;
        else {
            is24 = 0;
            if (bi.biBitCount != 8) {
                vfs_fclose(f);
                goto fail;
            }
            if (bi.biCompression != BI_RLE8 && bi.biCompression != BI_RGB) {
                vfs_fclose(f);
                goto fail;
            }
            if (!bi.biClrUsed)
                bi.biClrUsed = 256;
        }
        want24 = (flags & BMP_RGB) == BMP_RGB;
    }
    pitch = bi.biWidth;
    if (is24)
        pitch *= 3;
    if ((dither = flags & BMP_LOAD) == BMP_LOAD)
        load = 1;
    if ((flags & BMP_PAL) == BMP_PAL) {
        getpal = 1;
        if (is24 && !load)
            load = 1;
    }
    if (load) {
        if (is24 == want24 && data)
            buf = data;
        else {
            buf = mem_Alloc(abs(bi.biHeight) * pitch, "!bmp:LoadBMPBuffer");
            if (!is24)
                goto readpal;
        }
    }
    if (getpal && !is24) {
readpal:
        if (want24 || !pal)
            bpal = mem_Alloc(0x300, "!bmp:tLoadBMPPal");
        else
            bpal = pal;
        tmp = mem_Alloc(0x400, "!bmp:tLoadBMP4bPal");
        vfs_fseek(f, 0x36, SEEK_SET);
        vfs_fread(tmp, 4, bi.biClrUsed, f);
        for (i = 0; i < (int)bi.biClrUsed; i++) {
            bpal[i * 3] = tmp[i * 4 + 2];
            bpal[i * 3 + 1] = tmp[i * 4 + 1];
            bpal[i * 3 + 2] = tmp[i * 4];
        }
        mem_Free(tmp);
    }
    if (load) {
        if (!is24) {
            vfs_fseek(f, bi.biClrUsed * 4 + 0x36, SEEK_SET);
            if (bi.biCompression == BI_RLE8) {
                x = 0;
                for (y = 0; y < abs(bi.biHeight); ) {
                    vfs_fread(rle, 1, 2, f);
                    if (rle[0] == 0) {
                        switch (rle[1]) {
                        case 0:
                            x = 0;
                            y++;
                            break;
                        case 1:
                            x = 0;
                            y = abs(bi.biHeight);
                            break;
                        case 2:
                            vfs_fread(rle, 1, 2, f);
                            x += rle[0];
                            y += rle[1];
                            break;
                        default:
                            vfs_fread(buf + y * bi.biWidth + x, (rle[1] & 1) + rle[1], 1, f);
                            x += rle[1];
                            break;
                        }
                    } else {
                        BMP_FILL(tmp, buf + y * bi.biWidth + x, rle[1], rle[0]);
                        x += rle[0];
                    }
                }
            } else
                vfs_fread(buf, pitch, abs(bi.biHeight), f);
        } else {
            vfs_fseek(f, 0x36, SEEK_SET);
            vfs_fread(buf, pitch, abs(bi.biHeight), f);
            for (i = 0; i < abs(bi.biHeight) * bi.biWidth; i++) {
                rle[0] = buf[i * 3];
                buf[i * 3] = buf[i * 3 + 2];
                buf[i * 3 + 2] = rle[0];
            }
        }
        if (bi.biHeight > 0) {
            tmp = mem_Alloc(pitch, "!bmp:tGetBMPFlip");
            height = abs(bi.biHeight);
            for (i = 0; i < height / 2; i++) {
                {
                    unsigned char *s = buf + i * pitch;
                    BMP_COPY(tmp, s, pitch);
                }
                {
                    unsigned char *s = buf + (height - 1 - i) * pitch;
                    unsigned char *d = buf + i * pitch;
                    BMP_COPY(d, s, pitch);
                }
                {
                    unsigned char *d = buf + (height - 1 - i) * pitch;
                    BMP_COPY(d, tmp, pitch);
                }
            }
            mem_Free(tmp);
        }
    }
    vfs_fclose(f);
    if (is24 != want24 && dither) {
        if (want24) {
            n = abs(bi.biHeight) * pitch;
            for (i = 0; i < n; i++) {
                data[i * 3] = bpal[buf[i] * 3];
                data[i * 3 + 1] = bpal[buf[i] * 3 + 1];
                data[i * 3 + 2] = bpal[buf[i] * 3 + 2];
            }
        } else {
            if (!bpal)
                bpal = pal;
            qnt_Quantize(buf, data, bi.biWidth, abs(bi.biHeight), 256, 1, qpal[0]);
            if (bpal) {
                for (i = 0; i < 256; i++) {
                    bpal[i * 3] = qpal[0][i];
                    bpal[i * 3 + 1] = qpal[1][i];
                    bpal[i * 3 + 2] = qpal[2][i];
                }
            }
        }
    }
    if (buf != data)
        mem_Free(buf);
    if (bpal && (want24 || !pal))
        mem_Free(bpal);
    if ((flags & BMP_RGBA) == BMP_RGBA && data) {
        unsigned int count;
        unsigned char *dst;
        unsigned char *src;
        unsigned char r, g, b;

        count = abs(bi.biHeight) * bi.biWidth;
        src = data + count * 3 - 3;
        dst = data + count * 4 - 4;
        if ((flags & BMP_BGR) == BMP_BGR) {
            for (; count > 0; count--) {
                r = src[0];
                g = src[1];
                b = src[2];
                dst[0] = b;
                dst[1] = g;
                dst[2] = r;
                dst[3] = 0;
                src -= 3;
                dst -= 4;
            }
        } else {
            for (; count > 0; count--) {
                r = src[0];
                g = src[1];
                b = src[2];
                dst[0] = r;
                dst[1] = g;
                dst[2] = b;
                dst[3] = 0;
                src -= 3;
                dst -= 4;
            }
        }
    }
    return 1;
fail:
    return 0;
}
/* 0x401b90, writes a bottom-up 24-bit bitmap from top-down RGB data */

unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data)
{
    struct {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD bmiColors[1];
    } bi;
    BITMAPFILEHEADER bf;
    VFSFILE *f;
    unsigned char *row, *src;
    int x, y;

    BMP_ZERO(&bi, sizeof(bi));
    BMP_ZERO(&bf, sizeof(bf));
    bf.bfType = 0x4d42;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biYPelsPerMeter = 1;
    bf.bfSize = w * h * 3 + sizeof(bf) + sizeof(bi);
    bf.bfReserved2 = 0;
    bf.bfReserved1 = 0;
    bi.bmiColors[0].rgbRed = 0;
    bi.bmiColors[0].rgbGreen = 0;
    bi.bmiColors[0].rgbBlue = 0;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;
    bi.bmiHeader.biXPelsPerMeter = 1;
    bf.bfOffBits = sizeof(bf) + sizeof(bi);
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = h;
    bi.bmiHeader.biBitCount = 24;
    f = vfs_Tfopen(path, "wbn", (int)vfs_Root);
    if (f) {
    row = mem_Alloc(w * 3, "!bmp:SaveBMP24(temp_row)");
    vfs_fwrite(&bf, sizeof(bf), 1, f);
    vfs_fwrite(&bi, sizeof(bi), 1, f);
    for (y = h - 1; y >= 0; y--) {
        for (x = 0; x < w; x++) {
            row[x * 3] = data[(y * w + x) * 3 + 2];
            row[x * 3 + 1] = data[(y * w + x) * 3 + 1];
            row[x * 3 + 2] = data[(y * w + x) * 3];
        }
        vfs_fwrite(row, w * 3, 1, f);
    }
    vfs_fclose(f);
    mem_Free(row);
    return 1;
    }
    return 0;
}
