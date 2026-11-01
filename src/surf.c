/* surf.c -- original file name unknown.
   Scaling and pixel format conversion between locked DirectDraw surfaces (used for the snow mipmaps
   in d3_tools.c and by d3_d3d.c). Box filter when shrinking, bilinear interpolation (nearest for
   8-bit) when growing. Extent in GildeGold.exe (inferred, not proven): 0x41ebc0..0x4206d0, right after the
   last Dd_vesa.c function. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/surf.c); boundaries in decl/text.dcl, notes in notes/units/surf.md */
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
/* 0x41f000 @deep behaviour: no meaning difference against the Ghidra reading (the same trailing-zero shift count per channel, y0/y1 and x0/x1 box bounds, the same sum of masked channels divided by n and OR of b/n << bs | g/n << gs | r/n << rs); the 302 code hunks are scheduling and register/slot allocation (the src parameter in ebp against esi, the slot order, hoisted row bases); the original's shift counters are bytes (unsigned char rs, gs, bs), but that made the diff worse (426 hunks), so it belongs with the shape work; readability met */

void surf_Down32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    unsigned int m;
    unsigned int y, x, y0, y1, x0, x1, n, r, g, b, yy, xx, p;
    unsigned int *sp;
    char *row;
    unsigned int *d;

    rs = 0;
    for (m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (; m; m >>= 1)
        ;
    gs = 0;
    for (m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (; m; m >>= 1)
        ;
    bs = 0;
    for (m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (; m; m >>= 1)
        ;
    y0 = 0;
    for (y = 0; y < dst->dwHeight; y++) {
        d = (unsigned int *)((char *)dst->lpSurface + dst->lPitch * y);
        y1 = src->dwHeight * (y + 1) / dst->dwHeight;
        x0 = 0;
        for (x = 0; x < dst->dwWidth; x++) {
            x1 = src->dwWidth * (x + 1) / dst->dwWidth;
            n = 0;
            r = 0;
            g = 0;
            b = 0;
            if (y1 - y0) {
                row = (char *)src->lpSurface + src->lPitch * y0;
                yy = y1 - y0;
                do {
                    if (x1 - x0) {
                        n += x1 - x0;
                        sp = (unsigned int *)row + x0;
                        xx = x1 - x0;
                        do {
                            p = *sp++;
                        r += (p & src->ddpfPixelFormat.dwRBitMask) >> rs;
                        g += (p & src->ddpfPixelFormat.dwGBitMask) >> gs;
                        b += (p & src->ddpfPixelFormat.dwBBitMask) >> bs;
                        } while (--xx);
                    }
                    row += src->lPitch;
                } while (--yy);
            }
            d[x] = b / n << bs | g / n << gs | r / n << rs;
            x0 = x1;
        }
        y0 = y1;
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
/* 0x41f2b0 @deep read whole against the original: rewritten to its data flow - y0 = y1 / x0 = x1 at the loop tops, right before down, all four neighbours' r/g/b computed up front (the original's 9 slots 0x8c..0xbc + 3 registers), the six row deltas (r10 - r00 ..) computed once before the yy loop, the per-row end values and the three column steps (hr - lr ..) once before the xx loop, the output OR in one 32-bit expression stored as a word; every operation, shift and bound matches. Left: allocation - the original keeps src on the stack and reloads it (mov ecx, [esp+0xcc] before each G/B mask read, only the R mask cached in ebx; ours holds src in ebp and all three masks in registers), the x / y bounds in slots [0x10] (ours in ebx / ecx), frame 0xa0 vs 0xb4; e.g. '+ mov ecx, dword ptr [esp + 0xcc]; + mov ecx, dword ptr [ecx + 0x5c]; + and edx, ecx' against '- mov edx, dword ptr [ebp + 0x5c]' hoisted (size: 367 vs 382 instructions, explained: 15 instructions: the original's src reloads from the stack (one per G / B mask read, 8) and the bound reloads from slots that ours keeps in registers) */

void surf_Up16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    unsigned int m;
    unsigned int y, x, y0, y1, x0, x1, dy, dx, yy, xx, down, right;
    unsigned int p00, p01, p10, p11;
    unsigned int r00, g00, b00, r01, g01, b01, r10, g10, b10, r11, g11, b11;
    unsigned int dr0, dg0, db0, dr1, dg1, db1, ar0, ag0, ab0, ar1, ag1, ab1;
    unsigned int lr, lg, lb, hr, hg, hb, er, eg, eb, sr, sg, sb;
    unsigned short *srow, *d;

    rs = 0;
    for (m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (; m; m >>= 1)
        ;
    gs = 0;
    for (m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (; m; m >>= 1)
        ;
    bs = 0;
    for (m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (; m; m >>= 1)
        ;
    y1 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y0 = y1;
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        srow = (unsigned short *)((char *)src->lpSurface + src->lPitch * y);
        x1 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x0 = x1;
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            right = x + 1 < src->dwWidth;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            p00 = srow[x];
            r00 = (p00 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g00 = (p00 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b00 = (p00 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p01 = srow[right + x];
            r01 = (p01 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g01 = (p01 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b01 = (p01 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p10 = srow[down + x];
            r10 = (p10 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g10 = (p10 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b10 = (p10 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p11 = srow[down + right + x];
            r11 = (p11 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g11 = (p11 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b11 = (p11 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            dr0 = r10 - r00;
            dg0 = g10 - g00;
            db0 = b10 - b00;
            dr1 = r11 - r01;
            dg1 = g11 - g01;
            db1 = b11 - b01;
            ar0 = ag0 = ab0 = ar1 = ag1 = ab1 = 0;
            for (yy = 0; yy < dy; yy++) {
                lr = ar0 / dy + r00;
                lg = ag0 / dy + g00;
                lb = ab0 / dy + b00;
                hr = ar1 / dy + r01;
                hg = ag1 / dy + g01;
                hb = ab1 / dy + b01;
                d = (unsigned short *)((char *)dst->lpSurface + (yy + y0) * dst->lPitch) + x0;
                er = hr - lr;
                eg = hg - lg;
                eb = hb - lb;
                sr = sg = sb = 0;
                for (xx = 0; xx < dx; xx++) {
                    *d++ = (unsigned short)((sb / dx + lb) << bs | (sg / dx + lg) << gs | (sr / dx + lr) << rs);
                    sb += eb;
                    sg += eg;
                    sr += er;
                }
                ar0 += dr0;
                ag0 += dg0;
                ab0 += db0;
                ar1 += dr1;
                ag1 += dg1;
                ab1 += db1;
            }
        }
    }
}
/* 0x41f800 @deep read whole against the original: rewritten to its data flow - y0 = y1 / x0 = x1 at the loop tops, right before down, the four neighbours' bytes loaded up front, under 'if (dy > 0)' / 'if (dx > 0)' (the original's cmp/jbe guards with the deltas and zeroed accumulators after them) the six row deltas once, the per-row end values and the three column steps once, the byte stores 'add al, bl'. Tried a plain for loop with in-loop deltas (509 ins, worse). Left: allocation and placement - the original strength-reduces x * 3 into a stepped slot [0x38] and a p00 pointer in ebx (srow + 2 + 3x, '+ lea ebx, [esi + 2]'), ours recomputes 'lea ebp, [ebp + ebp*2]'; our VC6 sinks the p11 byte loads past the dy guard ('- mov cl, byte ptr [eax]' after 'cmp esi, ebp; jbe') where the original loads them before it; registers / slots otherwise (frame 0x90 vs 0xa8) */

void surf_Up24(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    unsigned int y, x, y0, y1, x0, x1, dy, dx, yy, xx, down, right;
    unsigned char *srow, *p, *d;
    unsigned int r00, g00, b00, r01, g01, b01, r10, g10, b10, r11, g11, b11;
    unsigned int dr0, dg0, db0, dr1, dg1, db1, ar0, ag0, ab0, ar1, ag1, ab1;
    unsigned int lr, lg, lb, hr, hg, hb, er, eg, eb, sr, sg, sb;

    y1 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y0 = y1;
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        srow = (unsigned char *)src->lpSurface + src->lPitch * y;
        x1 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x0 = x1;
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            right = x + 1 < src->dwWidth;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            p = srow + x * 3;
            r00 = p[0];
            g00 = p[1];
            b00 = p[2];
            p = srow + (x + right) * 3;
            r01 = p[0];
            g01 = p[1];
            b01 = p[2];
            p = srow + x * 3 + down;
            r10 = p[0];
            g10 = p[1];
            b10 = p[2];
            p = srow + (x + right) * 3 + down;
            r11 = p[0];
            g11 = p[1];
            b11 = p[2];
            if (dy > 0) {
                dr0 = r10 - r00;
                dg0 = g10 - g00;
                db0 = b10 - b00;
                dr1 = r11 - r01;
                dg1 = g11 - g01;
                db1 = b11 - b01;
                ar0 = ag0 = ab0 = ar1 = ag1 = ab1 = 0;
                yy = 0;
                do {
                    lr = ar0 / dy + r00;
                    lg = ag0 / dy + g00;
                    lb = ab0 / dy + b00;
                    hr = ar1 / dy + r01;
                    hg = ag1 / dy + g01;
                    hb = ab1 / dy + b01;
                    d = (unsigned char *)dst->lpSurface + (yy + y0) * dst->lPitch + x0 * 3;
                    if (dx > 0) {
                        er = hr - lr;
                        eg = hg - lg;
                        eb = hb - lb;
                        sr = sg = sb = 0;
                        xx = dx;
                        do {
                            d[0] = (unsigned char)(sr / dx) + (unsigned char)lr;
                            d[1] = (unsigned char)(sg / dx) + (unsigned char)lg;
                            d[2] = (unsigned char)(sb / dx) + (unsigned char)lb;
                            sr += er;
                            sg += eg;
                            sb += eb;
                            d += 3;
                        } while (--xx);
                    }
                    ar0 += dr0;
                    ag0 += dg0;
                    ab0 += db0;
                    ar1 += dr1;
                    ag1 += dg1;
                    ab1 += db1;
                } while (++yy < dy);
            }
        }
    }
}
/* 0x41fc00 */

void surf_Up32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src)
{
    int rs, gs, bs;
    unsigned int m;
    unsigned int y, x, y0, y1, x0, x1, dy, dx, yy, xx, down, right;
    unsigned int p00, p01, p10, p11;
    unsigned int r00, g00, b00, r01, g01, b01;
    unsigned int al, ag, ab, ar2, ag2, ab2, hr, hg, hb;
    int lr, lg, lb;
    unsigned int *srow, *d;

    rs = 0;
    for (m = src->ddpfPixelFormat.dwRBitMask; !(m & 1); m >>= 1)
        rs++;
    for (; m; m >>= 1)
        ;
    gs = 0;
    for (m = src->ddpfPixelFormat.dwGBitMask; !(m & 1); m >>= 1)
        gs++;
    for (; m; m >>= 1)
        ;
    bs = 0;
    for (m = src->ddpfPixelFormat.dwBBitMask; !(m & 1); m >>= 1)
        bs++;
    for (; m; m >>= 1)
        ;
    y0 = 0;
    for (y = 0; y < src->dwHeight; y++) {
        y1 = dst->dwHeight * (y + 1) / src->dwHeight;
        srow = (unsigned int *)((char *)src->lpSurface + src->lPitch * y);
        x0 = 0;
        for (x = 0; x < src->dwWidth; x++) {
            x1 = dst->dwWidth * (x + 1) / src->dwWidth;
            dy = y1 - y0;
            dx = x1 - x0;
            down = y + 1 < src->dwHeight ? src->lPitch >> 1 : 0;
            right = x + 1 < src->dwWidth;
            p00 = srow[x];
            r00 = (p00 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g00 = (p00 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b00 = (p00 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p01 = srow[right + x];
            r01 = (p01 & src->ddpfPixelFormat.dwRBitMask) >> rs;
            g01 = (p01 & src->ddpfPixelFormat.dwGBitMask) >> gs;
            b01 = (p01 & src->ddpfPixelFormat.dwBBitMask) >> bs;
            p10 = srow[down + x];
            p11 = srow[down + right + x];
            if (dy) {
                al = ag = ab = ar2 = ag2 = ab2 = 0;
                yy = 0;
                do {
                    lr = al / dy + r00;
                    lg = ag / dy + g00;
                    lb = ab / dy + b00;
                    if (dx) {
                        hr = hg = hb = 0;
                        d = (unsigned int *)((char *)dst->lpSurface + (y0 + yy) * dst->lPitch) + x0;
                        xx = dx;
                        do {
                            *d++ = hb / dx + lb << bs | hg / dx + lg << gs | hr / dx + lr << rs;
                            hb += ab2 / dy + b01 - lb;
                            hg += ag2 / dy + g01 - lg;
                            hr += ar2 / dy + r01 - lr;
                        } while (--xx);
                    }
                    al += ((p10 & src->ddpfPixelFormat.dwRBitMask) >> rs) - r00;
                    ag += ((p10 & src->ddpfPixelFormat.dwGBitMask) >> gs) - g00;
                    ab += ((p10 & src->ddpfPixelFormat.dwBBitMask) >> bs) - b00;
                    ar2 += ((p11 & src->ddpfPixelFormat.dwRBitMask) >> rs) - r01;
                    ag2 += ((p11 & src->ddpfPixelFormat.dwGBitMask) >> gs) - g01;
                    ab2 += ((p11 & src->ddpfPixelFormat.dwBBitMask) >> bs) - b01;
                } while (++yy < dy);
            }
            x0 = x1;
        }
        y0 = y1;
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
