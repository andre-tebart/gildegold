/* surf.c -- original file name unknown.
   Scaling and pixel format conversion between locked DirectDraw surfaces (used for the snow mipmaps
   in d3_tools.c and by d3_d3d.c). Box filter when shrinking, bilinear interpolation (nearest for
   8-bit) when growing. Extent in GildeGold.exe (inferred, not proven): 0x41ebc0..0x4206d0, right after the
   last Dd_vesa.c function. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/surf.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <ddraw.h>
#include "d3.h"
#include "surf.h"

/* 0x41ebc0, 8-bit nearest-neighbour scaling of src into dst */
void surf_Scale8(LPDDSURFACEDESC2 dst, LPDDSURFACEDESC2 src)
{
    DWORD x, y;
    unsigned char *d, *s;

    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned char *)dst->lpSurface + dst->lPitch * y;
        s = (unsigned char *)src->lpSurface + src->lPitch * (src->dwHeight * y / dst->dwHeight);
        for (x = 0; x < dst->dwWidth; x++)
            d[x] = s[src->dwWidth * x / dst->dwWidth];
    }
}
/* 0x41ec50, 16-bit box filter shrinking src into dst */

void surf_Shrink16(LPDDSURFACEDESC2 dst, LPDDSURFACEDESC2 src)
{
    DWORD m, x, y, x0, x1, y0, y1, i, j, n, r, g, b, c, w;
    int rs, gs, bs, rb, gb, bb;
    unsigned short *d, *s;

    for (rs = 0, m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (rb = 0; m; m >>= 1)
        rb++;
    for (gs = 0, m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (gb = 0; m; m >>= 1)
        gb++;
    for (bs = 0, m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (bb = 0; m; m >>= 1)
        bb++;
    y1 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned short *)((unsigned char *)dst->lpSurface + dst->lPitch * y);
        y0 = y1;
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x1 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x0 = x1;
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            w = x1 - x0;
            n = b = g = r = 0;
            for (j = 0; j < y1 - y0; j++) {
                s = (unsigned short *)((unsigned char *)src->lpSurface + src->lPitch * (y0 + j));
                for (i = 0; i < w; i++) {
                    r += (s[x0 + i] & src->ddpfPixelFormat.dwRBitMask) >> rs;
                    g += (s[x0 + i] & src->ddpfPixelFormat.dwGBitMask) >> gs;
                    b += (s[x0 + i] & src->ddpfPixelFormat.dwBBitMask) >> bs;
                    n++;
                }
            }
            d[x] = (unsigned short)((r / n << rs) | (g / n << gs) | (b / n << bs));
        }
    }
}
/* 0x41ee70, 24-bit box filter shrinking src into dst */

void surf_Shrink24(LPDDSURFACEDESC2 dst, LPDDSURFACEDESC2 src)
{
    DWORD x, y, x0, x1, y0, y1, i, j, n, c0, c1, c2, w;
    unsigned char *d, *s;

    y1 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned char *)dst->lpSurface + dst->lPitch * y;
        y0 = y1;
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x1 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x0 = x1;
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            w = x1 - x0;
            n = c2 = c1 = c0 = 0;
            for (j = 0; j < y1 - y0; j++) {
                s = (unsigned char *)src->lpSurface + src->lPitch * (y0 + j);
                for (i = 0; i < w; i++) {
                    c0 += s[(x0 + i) * 3];
                    c1 += s[(x0 + i) * 3 + 1];
                    c2 += s[(x0 + i) * 3 + 2];
                    n++;
                }
            }
            d[x * 3] = (unsigned char)(c0 / n);
            d[x * 3 + 1] = (unsigned char)(c1 / n);
            d[x * 3 + 2] = (unsigned char)(c2 / n);
        }
    }
}
/* 0x41f000, 32-bit box filter shrinking src into dst (surf_Shrink16's text with DWORD pixels; the bit counts
   rb / gb / bb are unused, VC6 keeps only the shifting loops) */

void surf_Down32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    DWORD m, x, y, x0, x1, y0, y1, i, j, n, r, g, b, w;
    int rs, gs, bs, rb, gb, bb;
    DWORD *d, *s;

    for (rs = 0, m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (rb = 0; m; m >>= 1)
        rb++;
    for (gs = 0, m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (gb = 0; m; m >>= 1)
        gb++;
    for (bs = 0, m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (bb = 0; m; m >>= 1)
        bb++;
    y1 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (DWORD *)((unsigned char *)dst->lpSurface + dst->lPitch * y);
        y0 = y1;
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x1 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x0 = x1;
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            w = x1 - x0;
            n = b = g = r = 0;
            for (j = 0; j < y1 - y0; j++) {
                s = (DWORD *)((unsigned char *)src->lpSurface + src->lPitch * (y0 + j));
                for (i = 0; i < w; i++) {
                    r += (s[x0 + i] & src->ddpfPixelFormat.dwRBitMask) >> rs;
                    g += (s[x0 + i] & src->ddpfPixelFormat.dwGBitMask) >> gs;
                    b += (s[x0 + i] & src->ddpfPixelFormat.dwBBitMask) >> bs;
                    n++;
                }
            }
            d[x] = (r / n << rs) | (g / n << gs) | (b / n << bs);
        }
    }
}
/* 0x41f220 */

void surf_Up8(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    unsigned int y, x;
    unsigned char *s, *d;

    for (y = 0; y < src->dwHeight; y++) {
        s = (unsigned char *)src->lpSurface + src->lPitch * y;
        d = (unsigned char *)dst->lpSurface + dst->dwHeight * y / src->dwHeight * dst->lPitch;
        for (x = 0; x < src->dwWidth; x++)
            d[dst->dwWidth * x / src->dwWidth] = s[x];
    }
}
/* 0x41f2b0, 16-bit bilinear growing of src into dst (surf_Up32 with WORD pixels): per source pixel the four
   neighbours per channel (r[0] the pixel, [1] right, [2] down, [3] down right; right / down clamped at the edge),
   each row of the box interpolated between them by j / dy, each pixel by i / dx. The bit counts rb / gb / bb are
   unused. */

void surf_Up16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    DWORD m, x, y, x0, x1, y0, y1, i, j, dx, dy, right, down;
    DWORD r[4], g[4], b[4], lr, lg, lb, hr, hg, hb;
    int rs, gs, bs, rb, gb, bb;
    WORD *s, *d;

    for (rs = 0, m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (rb = 0; m; m >>= 1)
        rb++;
    for (gs = 0, m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (gb = 0; m; m >>= 1)
        gb++;
    for (bs = 0, m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (bb = 0; m; m >>= 1)
        bb++;
    y1 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y0 = y1;
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        s = (WORD *)((unsigned char *)src->lpSurface + src->lPitch * y);
        x1 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x0 = x1;
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            right = x + 1 < src->dwWidth;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            r[0] = (s[x] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[0] = (s[x] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[0] = (s[x] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            r[1] = (s[x + right] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[1] = (s[x + right] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[1] = (s[x + right] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            r[2] = (s[x + down] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[2] = (s[x + down] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[2] = (s[x + down] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            r[3] = (s[x + right + down] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[3] = (s[x + right + down] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[3] = (s[x + right + down] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            for (j = 0; j < dy; j++) {
                lr = r[0] + (r[2] - r[0]) * j / dy;
                lg = g[0] + (g[2] - g[0]) * j / dy;
                lb = b[0] + (b[2] - b[0]) * j / dy;
                hr = r[1] + (r[3] - r[1]) * j / dy;
                hg = g[1] + (g[3] - g[1]) * j / dy;
                hb = b[1] + (b[3] - b[1]) * j / dy;
                d = (WORD *)((unsigned char *)dst->lpSurface + (y0 + j) * dst->lPitch);
                for (i = 0; i < dx; i++)
                    d[x0 + i] = (lr + (hr - lr) * i / dx) << rs | (lg + (hg - lg) * i / dx) << gs | (lb + (hb - lb) * i / dx) << bs;
            }
        }
    }
}
/* 0x41f800, 24-bit bilinear growing of src into dst: per source pixel the four neighbours per channel (r[0] the
   pixel, [1] right, [2] down, [3] down right; right / down clamped at the edge), each row of the box interpolated
   between them by j / dy, each pixel by i / dx.
   @original down is lPitch >> 1 bytes, half a row (surf_Up16's element count for 16-bit pixels): the "down"
   neighbours are read from the middle of the same row */

void surf_Up24(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    DWORD x, y, x0, x1, y0, y1, i, j, dx, dy, right, down;
    DWORD r[4], g[4], b[4], lr, lg, lb, hr, hg, hb;
    unsigned char *s, *d;

    y1 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y0 = y1;
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        s = (unsigned char *)src->lpSurface + src->lPitch * y;
        x1 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x0 = x1;
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            right = x + 1 < src->dwWidth;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            r[0] = s[x * 3];
            g[0] = s[x * 3 + 1];
            b[0] = s[x * 3 + 2];
            r[1] = s[(x + right) * 3];
            g[1] = s[(x + right) * 3 + 1];
            b[1] = s[(x + right) * 3 + 2];
            r[2] = s[x * 3 + down];
            g[2] = s[x * 3 + down + 1];
            b[2] = s[x * 3 + down + 2];
            r[3] = s[(x + right) * 3 + down];
            g[3] = s[(x + right) * 3 + down + 1];
            b[3] = s[(x + right) * 3 + down + 2];
            for (j = 0; j < dy; j++) {
                lr = r[0] + (r[2] - r[0]) * j / dy;
                lg = g[0] + (g[2] - g[0]) * j / dy;
                lb = b[0] + (b[2] - b[0]) * j / dy;
                hr = r[1] + (r[3] - r[1]) * j / dy;
                hg = g[1] + (g[3] - g[1]) * j / dy;
                hb = b[1] + (b[3] - b[1]) * j / dy;
                d = (unsigned char *)dst->lpSurface + (y0 + j) * dst->lPitch;
                for (i = 0; i < dx; i++) {
                    d[(x0 + i) * 3] = (unsigned char)(lr + (hr - lr) * i / dx);
                    d[(x0 + i) * 3 + 1] = (unsigned char)(lg + (hg - lg) * i / dx);
                    d[(x0 + i) * 3 + 2] = (unsigned char)(lb + (hb - lb) * i / dx);
                }
            }
        }
    }
}
/* 0x41fc00, 32-bit bilinear growing of src into dst (surf_Up24 with the channels masked and shifted as in
   surf_Shrink16; the bit counts rb / gb / bb are unused).
   @original down is lPitch >> 1 pixels, two rows further down for 32-bit pixels (surf_Up16's element count) */

void surf_Up32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    DWORD m, x, y, x0, x1, y0, y1, i, j, dx, dy, right, down;
    DWORD r[4], g[4], b[4], lr, lg, lb, hr, hg, hb;
    int rs, gs, bs, rb, gb, bb;
    DWORD *s, *d;

    for (rs = 0, m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (rb = 0; m; m >>= 1)
        rb++;
    for (gs = 0, m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (gb = 0; m; m >>= 1)
        gb++;
    for (bs = 0, m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (bb = 0; m; m >>= 1)
        bb++;
    y1 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y0 = y1;
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        s = (DWORD *)((unsigned char *)src->lpSurface + src->lPitch * y);
        x1 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x0 = x1;
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            right = x + 1 < src->dwWidth;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            r[0] = (s[x] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[0] = (s[x] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[0] = (s[x] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            r[1] = (s[x + right] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[1] = (s[x + right] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[1] = (s[x + right] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            r[2] = (s[x + down] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[2] = (s[x + down] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[2] = (s[x + down] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            r[3] = (s[x + right + down] & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g[3] = (s[x + right + down] & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b[3] = (s[x + right + down] & src->ddpfPixelFormat.dwBBitMask) >> bs;
            for (j = 0; j < dy; j++) {
                lr = r[0] + (r[2] - r[0]) * j / dy;
                lg = g[0] + (g[2] - g[0]) * j / dy;
                lb = b[0] + (b[2] - b[0]) * j / dy;
                hr = r[1] + (r[3] - r[1]) * j / dy;
                hg = g[1] + (g[3] - g[1]) * j / dy;
                hb = b[1] + (b[3] - b[1]) * j / dy;
                d = (DWORD *)((unsigned char *)dst->lpSurface + (y0 + j) * dst->lPitch);
                for (i = 0; i < dx; i++)
                    d[x0 + i] = (lr + (hr - lr) * i / dx) << rs | (lg + (hg - lg) * i / dx) << gs | (lb + (hb - lb) * i / dx) << bs;
            }
        }
    }
}
/* 0x420140 */

void surf_ScaleBlit(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int bpp;
    unsigned int pal8, y;
    int n;

    bpp = src->ddpfPixelFormat.dwRGBBitCount;
    if (bpp == dst->ddpfPixelFormat.dwRGBBitCount
        && ((pal8 = src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) || bpp == 16 || bpp == 24 || bpp == 32)) {
        if (dst->dwWidth < src->dwWidth) {
            if (pal8) {
                surf_Scale8(dst, src);
                return;
            }
            if (bpp == 16) {
                surf_Shrink16(dst, src);
                return;
            }
            if (bpp == 24) {
                surf_Shrink24(dst, src);
                return;
            }
            if (bpp == 32) {
                surf_Down32(dst, src);
                return;
            }
        } else if (dst->dwWidth == src->dwWidth) {
            if (pal8)
                n = src->dwWidth;
            else if (bpp == 16)
                n = src->dwWidth * 2;
            else if (bpp == 24)
                n = src->dwWidth * 3;
            else if (bpp == 32)
                n = src->dwWidth * 4;
            for (y = 0; y < dst->dwWidth; y++)
                SURF_COPY((char *)dst->lpSurface + dst->lPitch * y, (char *)src->lpSurface + src->lPitch * y, n);
        } else {
            if (pal8) {
                surf_Up8(dst, src);
                return;
            }
            if (bpp == 16) {
                surf_Up16(dst, src);
                return;
            }
            if (bpp == 24) {
                surf_Up24(dst, src);
                return;
            }
            if (bpp == 32)
                surf_Up32(dst, src);
        }
    }
}
/* 0x4202d0, converts the 8-bit palette-indexed src surface to the 16-bit dst surface: each palette
   entry of pal is scaled down from 8 bits to the dst format's channel widths and placed at their bit
   positions; writes dst only, returns nothing */

void surf_Pal8To16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src, PALETTEENTRY *pal)
{
    int rs, gs, bs;
    int rb, gb, bb;
    unsigned int m, y, x;
    PALETTEENTRY *c;
    unsigned char *s;
    unsigned short *d;

    rs = 0;
    for (m = dst->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    rb = 0;
    while (m) {
        m >>= 1;
        rb++;
    }
    gs = 0;
    for (m = dst->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    gb = 0;
    while (m) {
        m >>= 1;
        gb++;
    }
    bs = 0;
    for (m = dst->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    bb = 0;
    while (m) {
        m >>= 1;
        bb++;
    }
    for (y = 0; y < src->dwHeight; y++) {
        s = (unsigned char *)src->lpSurface + src->lPitch * y;
        d = (unsigned short *)((char *)dst->lpSurface + dst->lPitch * y);
        for (x = 0; x < src->dwWidth; x++) {
            c = &pal[s[x]];
            d[x]
                = (unsigned short)(c->peRed >> (BYTE)(8 - rb)) << rs | (unsigned short)(c->peGreen >> (BYTE)(8 - gb)) << gs
                | (unsigned short)(c->peBlue >> (BYTE)(8 - bb)) << bs;
        }
    }
}
/* 0x420450, converts the 24-bit (three bytes per pixel) src surface to the 16-bit dst surface: each
   8-bit channel of the source pixel is scaled down to the dst format's channel width and placed at its
   bit position; writes dst only, returns nothing */

void surf_24To16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    int rb, gb, bb;
    unsigned int m, y, x;
    unsigned char *p;
    unsigned short *d;

    rs = 0;
    for (m = dst->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    rb = 0;
    while (m) {
        m >>= 1;
        rb++;
    }
    gs = 0;
    for (m = dst->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    gb = 0;
    while (m) {
        m >>= 1;
        gb++;
    }
    bs = 0;
    for (m = dst->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    bb = 0;
    while (m) {
        m >>= 1;
        bb++;
    }
    for (y = 0; y < src->dwHeight; y++) {
        p = (unsigned char *)src->lpSurface + src->lPitch * y;
        d = (unsigned short *)((char *)dst->lpSurface + dst->lPitch * y);
        for (x = 0; x < src->dwWidth; x++) {
            d[x]
                = (unsigned short)(p[x * 3] >> (BYTE)(8 - rb)) << rs | (unsigned short)(p[x * 3 + 1] >> (BYTE)(8 - gb)) << gs
                | (unsigned short)(p[x * 3 + 2] >> (BYTE)(8 - bb)) << bs;
        }
    }
}
/* 0x4205c0, copies a same-size src surface into dst: a straight row copy when the pixel formats match
   (or both are 8-bit palette-indexed), otherwise converts 8-bit palette-indexed src through srcpal to
   16-bit dst or 24-bit src to 16-bit dst; writes dst only, returns nothing */

void surf_Blit(DDSURFACEDESC2 *dst, PALETTEENTRY *dstpal, DDSURFACEDESC2 *src, PALETTEENTRY *srcpal)
{
    unsigned int y;
    int n;

    if (src->dwWidth == dst->dwWidth && src->dwHeight == dst->dwHeight) {
        if (src->ddpfPixelFormat.dwRGBBitCount == dst->ddpfPixelFormat.dwRGBBitCount
            || ((src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
                && (dst->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8))) {
            if (src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
                n = src->dwWidth;
            else if (src->ddpfPixelFormat.dwRGBBitCount == 16)
                n = src->dwWidth * 2;
            else if (src->ddpfPixelFormat.dwRGBBitCount == 24)
                n = src->dwWidth * 3;
            else if (src->ddpfPixelFormat.dwRGBBitCount == 32)
                n = src->dwWidth * 4;
            else
                return;
            for (y = 0; y < src->dwHeight; y++)
                SURF_COPY((char *)dst->lpSurface + dst->lPitch * y, (char *)src->lpSurface + src->lPitch * y, n);
        } else if ((src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
                   && dst->ddpfPixelFormat.dwRGBBitCount == 16) {
            surf_Pal8To16(dst, src, srcpal);
        } else if (src->ddpfPixelFormat.dwRGBBitCount == 24 && dst->ddpfPixelFormat.dwRGBBitCount == 16) {
            surf_24To16(dst, src);
        }
    }
}
