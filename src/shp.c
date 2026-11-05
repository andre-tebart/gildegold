/* shp.c -- 0x479d00..0x47d600, the shape ("shp") TU of the d2 layer (memory tags "d2:shp:...", "shp_t:..."):
   light table and colour masks, grabbing a rectangle of a surface into a run-length coded shape (8, 16 or
   24 bit), clipped drawing, conversion between 8 / 16 / 24 bit shapes and of whole banks, shape banks,
   animated shapes, two 16 bit drawing loops in inline assembly. Its own TU: its .data starts at 0x5fed74
   (colour keys, 640 / 480 / 800) after the GFX.C strings. The 0xcc after 0x47d210 is the linker's fill after
   that function's unpadded section (see its pragma), not an object end. Names are ours.
   Open: the end boundary to shp_asm.c rests on the subject only.
   unit of rea 8cef7d5b (src/game/shp.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "err.h"
#include "di_input.h"
#include "txt.h"
#include "dd_vesa.h"
#include "tb.h"
#define d2_DrawShapeHalfDark d2h_Unk47c770_void
#define d2_DrawShapeMode2 d2h_Unk47c970_void
#define d2_StartAnim d2h_Unk47cd20_int
#include "d2.h"
#undef d2_DrawShapeHalfDark
#undef d2_DrawShapeMode2
#undef d2_StartAnim
#include "shp.h"
#include "unk_47c5e0.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRACE */
#endif
#include "data_views.h"
#include "ahm.h"
#include "gfx.h"
#include "mem.h"

int d2_ColourFormatSet = 0;  /* 0x6f0450 */
extern unsigned int d2_HighBitMask32;

/* 0x479d00, makes the light table: every 16 bit colour brightened by add @deep 38 code hunks / 22 regs, register allocation only, and the unit note's Open work records this exact case as tried (r=g=b=0, temp pointer, assign-in-if): the original materialises the zero in ebx at entry (xor ebx,ebx), stores the three byte locals as bl, compares the mem_Alloc result with cmp eax,ebx and uses edi for lim / esi for the 0x10000 counter; ours stores immediates, uses test eax,eax and swaps lim / counter (esi / edi). The rest of the body read against the original 0x479d00..0x479def: mem_Alloc(0x20000, 'd2:shp:shp_LightTable'), lim = 0xff - add, the 0x10000 loop with dd_SplitRGB(i,&r,&g,&b), the three b/g/r clamps against lim (add or 0xff), dd_RGB and the 16 bit table store, return 1 / 0 - every constant, callee and condition corresponds. Readability: Q1 only (the mem tag gives the role). */
int d2_MakeLightTable(char add)
{
    int i;
    int lim;
    unsigned char r, g, b;

    r = g = b = 0;
    d2_ShpLightTable = mem_Alloc(0x20000, "d2:shp:shp_LightTable");
    if (d2_ShpLightTable != 0) {
        lim = 0xff - add;
        for (i = 0; i < 0x10000; i++) {
            dd_SplitRGB(i, &r, &g, &b);
            if (b < lim)
                b += add;
            else
                b = 0xff;
            if (g < lim)
                g += add;
            else
                g = 0xff;
            if (r < lim)
                r += add;
            else
                r = 0xff;
            d2_ShpLightTable[i] = (unsigned short)dd_RGB(r, g, b);
        }
        return 1;
    }
    return 0;
}
/* 0x479df0, sets the colour format of the current display mode: the high-bit mask of a 16 bit pixel from the
   dd_ color shifts / losses and the same mask in both halves for the 32 bit (two pixel) reads, the palette
   depth and the light-table flag. With external 0 it (re)builds the light table with 0x30; the formatted debug
   text is unused. Returns 1. */

int d2_SetColourFormat(int external, int paletteDepth)
{
    char buf[132];

    if (external == 0)
        d2_MakeLightTable(0x30);
    d2_PaletteDepth = paletteDepth;
    d2_LightTableExternal = external;
    d2_ColourFormatSet = 1;
    d2_HighBitMask = (unsigned short)((((1 << (7 - dd_RLoss)) - 1) << dd_RShift)
                                    | (((1 << (7 - dd_GLoss)) - 1) << dd_GShift)
                                    | (((1 << (7 - dd_BLoss)) - 1) << dd_BShift));
    d2_HighBitMask32 = d2_HighBitMask | (d2_HighBitMask << 16);
    sprintf(buf, "HighBitMask:%i rShift:%i gShift:%i bShift:%i", d2_HighBitMask, dd_RShift, dd_GShift, dd_BShift);
    return 1;
}
/* 0x479ee0, frees the light table at shutdown unless it is external (d2_LightTableExternal); clears the
   handle */

void d2_FreeLightTable(void)
{
    if (d2_LightTableExternal == 0 && d2_ShpLightTable != 0) {
        mem_Free(d2_ShpLightTable);
        d2_ShpLightTable = 0;
    }
}
/* 0x479f10, copies the row table to the end of a shape */

void d2_SetLineTable(int *rows, D2SHP *shp)
{
    memcpy((char *)shp + shp->lineoff, rows, shp->h * 4);
}
/* 0x479f40, makes a run length shape from 24 bit pixels @deep 679 code hunks, the unit note's Open work records this function as unsolved (0x479f40 / 0x47a460 / 0x47a7d0: the original computes the row pointer from scratch every row, ours strength-reduces it into a running pointer plus a [pitch*3] slot); the byte numbers are unchanged since that round (1159/1328). Read against the original 0x479f40..0x47a45f: the 24 bit pixel walk, the bounds scan (minx/maxx/miny/maxy), the key compare, the segment runs and the d2_SetLineTable upload correspond; what differs is the per-row pointer computation / register webs and the r/g/b local homes. Readability: Q1 name, Q2 d2_CompareColour / d2_SetColour / d2_EndMarkerColour / d2_StartMarkerColour / d2_TransparentColour. */

D2SHP *d2_GrabShape24Runs(int x, int y, int w, int h, unsigned char *pix, unsigned char flag, unsigned short pitch)
{
    D2SHP *shp, *ns;
    unsigned int minx, maxx, miny, maxy;
    unsigned char key[3];
    int tab[864];
    int r, c, nseg;
    unsigned char *p;
    int *seg, *rowp;
    int base, base2;

    r = 0;
    key[0] = 0;
    key[1] = 0;
    key[2] = 0;
    maxx = 0;
    minx = w;
    maxy = 0;
    miny = h;
    nseg = 0;
    shp = mem_Alloc((w * 0xc + 4) * h + 0x32, "shp_t:GrabBit24:ShapeHeader");
    for (r = 0; r < h; r++) {
        p = pix + ((r + y) * (pitch & 0xffff) + x) * 3;
        for (c = 0; c < w; c++) {
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c > (int)(maxx & 0xffff))
                maxx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c < (int)(minx & 0xffff))
                minx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r > (int)(maxy & 0xffff))
                maxy = r;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r < (int)(miny & 0xffff))
                miny = r;
            p += 3;
        }
    }
    if ((unsigned short)minx > (unsigned short)maxx || (unsigned short)miny > (unsigned short)maxy) {
        maxx = 0;
        minx = 0;
        maxy = 0;
        miny = 0;
    }
    shp->x0 = (unsigned short)minx;
    shp->ax0 = (unsigned short)minx;
    shp->w = (unsigned short)maxx + 1;
    shp->y0 = (unsigned short)miny;
    shp->h = (unsigned short)maxy + 1;
    shp->ay0 = (unsigned short)miny;
    shp->ax1 = (unsigned short)maxx + 1;
    shp->ay1 = (unsigned short)maxy + 1;
    if (d2_CompareColour((unsigned char *)&d2_EndMarkerColour, key) != 0) {
        for (r = 0; r < h; r++) {
            p = pix + ((y + r) * (pitch & 0xffff) + x) * 3;
            for (c = 0; c < w; c++) {
                if (d2_CompareColour((unsigned char *)&d2_StartMarkerColour, key) != 0 && d2_CompareColour(p, (unsigned char *)&d2_StartMarkerColour) == 0) {
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                    minx = c;
                    miny = r;
                }
                if (d2_CompareColour(p, (unsigned char *)&d2_EndMarkerColour) == 0) {
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                    maxy = r;
                    maxx = c;
                }
                p += 3;
            }
        }
    }
    shp->by0 = (unsigned short)miny;
    shp->bx0 = (unsigned short)minx;
    shp->bx1 = (unsigned short)maxx;
    shp->by1 = (unsigned short)maxy;
    shp->size = 0x32;
    miny = 0;
    if (shp->h != 0) {
        do {
            maxx = 0;
            rowp = (int *)((char *)shp + shp->size);
            shp->size += 4;
            tab[miny] = (char *)rowp - (char *)shp;
            minx = 0;
            if (shp->w != 0) {
                base = (miny + y) * (pitch & 0xffff);
                base2 = x + base;
                do {
                    seg = (int *)((char *)shp + shp->size);
                    seg[0] = 0;
                    seg[1] = 0;
                    if ((int)minx < shp->w) {
                        p = pix + (minx + base + x) * 3;
                        do {
                            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0)
                                break;
                            minx++;
                            p += 3;
                            seg[0]++;
                        } while ((int)minx < shp->w);
                    }
                    seg[0] *= 3;
                    if (seg[1] + minx < shp->w) {
                        do {
                            if (d2_CompareColour(pix + (minx + base + x + seg[1]) * 3, (unsigned char *)&d2_TransparentColour) == 0)
                                break;
                            seg[1]++;
                        } while (seg[1] + minx < shp->w);
                    }
                    memcpy(seg + 2, pix + (base2 + minx) * 3, seg[1] * 3);
                    shp->npix += seg[1];
                    minx += seg[1];
                    shp->size = shp->size + seg[1] * 3 + 8;
                    nseg++;
                    maxx++;
                } while ((int)minx < shp->w);
            }
            miny++;
            *rowp = maxx;
        } while ((int)miny < shp->h);
    }
    shp->lineoff = shp->size;
    shp->size += shp->h * 4;
    d2_SetLineTable(tab, shp);
    shp->mode = flag;
    shp->nruns = nseg;
    ns = mem_Alloc(shp->size, "shp_t:ShapeHeader");
    memcpy(ns, shp, shp->size);
    mem_Free(shp);
    return ns;
}
/* 0x47a460, grabs a rectangle of a 24 bit surface into a shape without run lengths (memory tag
   "shp_t:GrabBit24NoRS:ShapeHeader"): scans the rectangle for the bounding box of the pixels that are not the
   transparent colour, then (when the end marker colour is set) replaces the start / end marker pixels with the
   transparent colour and takes the box from those, copies the plain pixel rows into the shape, and returns an
   exactly sized copy of it (allocated with mem_Alloc; the scratch shape is freed).
   @deep behaviour draft (446 code hunks, 774/912 bytes): every hunk is the per-row pixel pointer - ours strength-reduces both scan loops (hoists (pitch&0xffff)*y, keeps pitch*3 in a slot and advances p by it per row), the original recomputes pix + ((y+r)*(pitch&0xffff)+x)*3 with an imul every row (0x47a4c2..0x47a4e5, 0x47a623..0x47a645); the rest is the frame/register shape it causes (frame 0x1c vs 0x14, r/c/p in other registers); reset test, bbox+header stores, marker loop, the two rep-movs copies and the tail order all correspond. Tried: fewer locals (w1/h1 removed), comma-increment inner loop, mask on pitch removed - identical bytes */

D2SHP *d2_GrabShape24NoRuns(int x, int y, int w, int h, unsigned char *pix, unsigned char flag, unsigned short pitch)
{
    D2SHP *shp, *ns;
    unsigned int minx, maxx, miny, maxy;
    unsigned char key[3];
    int r, c, n;
    unsigned char *p, *src, *dst;

    r = 0;
    key[0] = 0;
    key[1] = 0;
    key[2] = 0;
    maxx = 0;
    minx = w;
    maxy = 0;
    miny = h;
    shp = mem_Alloc((w * 0xc + 4) * h + 0x32, "shp_t:GrabBit24NoRS:ShapeHeader");
    for (r = 0; r < h; r++) {
        p = pix + ((r + y) * (pitch & 0xffff) + x) * 3;
        for (c = 0; c < w; c++, p += 3) {
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c > (int)(maxx & 0xffff))
                maxx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c < (int)(minx & 0xffff))
                minx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r > (int)(maxy & 0xffff))
                maxy = r;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r < (int)(miny & 0xffff))
                miny = r;
        }
    }
    if ((unsigned short)minx > (unsigned short)maxx || (unsigned short)miny > (unsigned short)maxy) {
        maxx = 0;
        minx = 0;
        maxy = 0;
        miny = 0;
    }
    shp->x0 = (short)minx;
    shp->ax0 = (short)minx;
    shp->w = (short)maxx + 1;
    shp->y0 = (short)miny;
    shp->h = (short)maxy + 1;
    shp->ay0 = (short)miny;
    shp->ax1 = (short)maxx + 1;
    shp->ay1 = (short)maxy + 1;
    if (d2_CompareColour((unsigned char *)&d2_EndMarkerColour, key) != 0) {
        for (r = 0; r < h; r++) {
            p = pix + ((y + r) * (pitch & 0xffff) + x) * 3;
            for (c = 0; c < w; c++, p += 3) {
                if (d2_CompareColour(p, (unsigned char *)&d2_StartMarkerColour) == 0) {
                    minx = c;
                    miny = r;
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                }
                if (d2_CompareColour(p, (unsigned char *)&d2_EndMarkerColour) == 0) {
                    maxx = c;
                    maxy = r;
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                }
            }
        }
    }
    shp->bx0 = (unsigned short)minx;
    shp->by0 = (unsigned short)miny;
    shp->bx1 = (unsigned short)maxx;
    shp->by1 = (unsigned short)maxy;
    shp->size = 0x32;
    dst = (unsigned char *)(shp + 1);
    w = 0;
    if (shp->h != 0) {
        n = shp->w * 3;
        src = pix + ((pitch & 0xffff) * y + x) * 3;
        do {
            memcpy(dst, src, n);
            n = shp->w * 3;
            shp->npix += shp->w;
            src += (pitch & 0xffff) * 3;
            shp->size += n;
            dst += n;
            w++;
        } while (w < shp->h);
    }
    shp->lineoff = 0;
    shp->mode = flag;
    shp->nruns = -1;
    ns = mem_Alloc(shp->size, "shp_t:ShapeHeader");
    memcpy(ns, shp, shp->size);
    mem_Free(shp);
    return ns;
}
/* 0x47a7d0, grabs a rectangle of a 16 bit surface into a run length coded shape (memory tag
   "shp_t:GrabBit8:ShapeHeader"): scans the rectangle for the bounding box of the non-zero pixels, then stores
   every row as runs of the transparent colour (skip runs) alternating with literal runs, builds the table of
   row offsets, and returns an exactly sized copy of the shape (allocated with mem_Alloc; the scratch shape is
   freed).
   @deep behaviour draft (466 code hunks, 771/848 bytes): the bounds scan strength-reduces like 0x47a460 (ours hoists pitch*2 and keeps the row pointer running, the original recomputes pix + (y+r)*(pitch&0xffff)+x each row, 0x47a830..0x47a859) and keeps maxx/maxy in registers where the original keeps them in the slots 0x10/0x14 (minx/miny the other way round); the run-length loop differs the same way (base/base+x slots, seg pointer, the key in di vs a slot). Read against the original 47a7d0..47ab2f: the bbox reset, the header stores, the dd_RGB key, the two scan guards, the run skip/count/seg[0]*=2/memcpy/npix/size updates, the line table upload and the tail all correspond. Tried: loop-form variants on the sibling 0x47a460 (fewer locals, comma increment, mask removal) changed no byte */

D2SHP *d2_GrabShape16Runs(int x, int y, int w, int h, unsigned short *pix, unsigned char flag, unsigned short pitch)
{
    D2SHP *shp, *ns;
    unsigned int minx, maxx, miny, maxy;
    unsigned short ux, uy;
    unsigned short key;
    short w1, h1;
    int tab[864];
    int r, c, nseg;
    unsigned short *p;
    int *seg, *rowp;
    int base;

    maxx = 0;
    maxy = 0;
    nseg = 0;
    shp = mem_Alloc((w * 8 + 4) * h + 0x32, "shp_t:GrabBit8:ShapeHeader");
    r = 0;
    minx = w;
    miny = h;
    for (r = 0; r < h; r++) {
        p = pix + (y + r) * (pitch & 0xffff) + x;
        for (c = 0; c < w; c++) {
            if (*p != 0) {
                if (c > (int)(maxx & 0xffff))
                    maxx = c;
                if (c < (int)(minx & 0xffff))
                    minx = c;
                if (r > (int)(maxy & 0xffff))
                    maxy = r;
                if (r < (int)(miny & 0xffff))
                    miny = r;
            }
            p++;
        }
    }
    uy = (unsigned short)miny;
    ux = (unsigned short)minx;
    if ((unsigned short)maxx < ux || (unsigned short)maxy < uy) {
        ux = 0;
        uy = 0;
        maxy = 0;
        maxx = 0;
    }
    shp->x0 = ux;
    w1 = (short)(maxx + 1);
    shp->w = w1;
    shp->y0 = uy;
    h1 = (short)(maxy + 1);
    shp->h = h1;
    shp->ax0 = ux;
    shp->ay0 = uy;
    shp->ax1 = w1;
    shp->ay1 = h1;
    shp->bx0 = ux;
    shp->by0 = uy;
    shp->bx1 = (short)maxx;
    shp->by1 = (short)maxy;
    shp->size = 0x32;
    key = (unsigned short)dd_RGB(d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
    r = 0;
    if (shp->h != 0) {
        do {
            maxx = 0;
            rowp = (int *)((char *)shp + shp->size);
            minx = shp->size + 4;
            shp->size = minx;
            tab[r] = (char *)rowp - (char *)shp;
            miny = 0;
            if (shp->w != 0) {
                base = (r + y) * (pitch & 0xffff);
                do {
                    seg = (int *)(minx + (int)shp);
                    seg[0] = 0;
                    seg[1] = 0;
                    if (pix[x + base + miny] == key) {
                        p = pix + base + miny + x;
                        do {
                            if ((int)miny >= shp->w)
                                break;
                            miny++;
                            p++;
                            seg[0]++;
                        } while (*p == key);
                    }
                    seg[0] *= 2;
                    if (miny < shp->w) {
                        do {
                            if (pix[base + seg[1] + miny + x] == key)
                                break;
                            c = seg[1] + 1;
                            seg[1] = c;
                        } while (c + miny < shp->w);
                    }
                    minx = seg[1];
                    memcpy(seg + 2, pix + x + base + miny, minx * 2);
                    c = seg[1];
                    if (c != 0)
                        miny--;
                    shp->npix += c;
                    minx = shp->size + seg[1] * 2 + 8;
                    shp->size = minx;
                    nseg++;
                    maxx++;
                    miny += c + 1;
                } while ((int)miny < shp->w);
            }
            *rowp = maxx;
            r++;
        } while (r < shp->h);
    }
    shp->lineoff = shp->size;
    shp->size += shp->h * 4;
    d2_SetLineTable(tab, shp);
    shp->mode = flag;
    shp->nruns = nseg;
    ns = mem_Alloc(shp->size, "shp_t:ShapeHeader");
    memcpy(ns, shp, shp->size);
    mem_Free(shp);
    return ns;
}
/* 0x47ab30, grabs a rectangle of a 16 bit surface into a shape with plain rows (no run lengths; memory tag
   "shp_t:GrabBit8:ShapeHeader"): scans the rectangle for the bounding box of the non-zero pixels, copies the
   plain pixel rows into the shape and returns an exactly sized copy of it (allocated with mem_Alloc; the
   scratch shape is freed).
   @deep behaviour draft (319 code hunks, 493/544 bytes): the pixel scan keeps p in a stack slot (load/add/store per pixel, [esp+0x10]) where the original keeps it in edx (one add edx,2), and the bounds homes differ (original maxx/maxy in the slots 0x10/0x14, minx/miny in esi/edi; ours maxx=edi, minx=edx, miny=esi, maxy=slot); frame 0x10 vs 0x8, 544 vs 528 bytes. Read against 47ab30..47ad3f: the bbox conditions and reset, the header store order (x0,w,y0,h,ax0,ay0,ax1,ay1,bx0,by0,bx1,by1), the unused dd_RGB call, the row copy (rep movs, npix/size/p/dst updates), lineoff/mode/nruns and the alloc-copy-free tail all correspond. Tried: unsigned int bounds with (int)(x & 0xffff) compares (the original's dword stores), declaration orders, tail counter on w instead of i - no byte change */

D2SHP *d2_GrabShape16NoRuns(int x, int y, int w, int h, unsigned short *src, unsigned char mode, unsigned short pitch)
{
    int i, j;
    unsigned int maxx, maxy, minx, miny;
    unsigned short *p;
    D2SHP *s, *r;
    char *dst;

    maxx = 0;
    maxy = 0;
    s = mem_Alloc((w * 8 + 4) * h + 0x32, "shp_t:GrabBit8:ShapeHeader");
    minx = w;
    miny = h;
    for (i = 0; i < h; i++) {
        p = src + (y + i) * (pitch & 0xffff) + x;
        for (j = 0; j < w; j++, p++) {
            if (*p != 0) {
                if (j > (int)(maxx & 0xffff))
                    maxx = j;
                if (j < (int)(minx & 0xffff))
                    minx = j;
                if (i > (int)(maxy & 0xffff))
                    maxy = i;
                if (i < (int)(miny & 0xffff))
                    miny = i;
            }
        }
    }
    if ((unsigned short)maxx < (unsigned short)minx || (unsigned short)maxy < (unsigned short)miny) {
        minx = 0;
        miny = 0;
        maxx = 0;
        maxy = 0;
    }
    s->x0 = (unsigned short)minx;
    s->w = (unsigned short)maxx + 1;
    s->y0 = (unsigned short)miny;
    s->h = (unsigned short)maxy + 1;
    s->ax0 = (unsigned short)minx;
    s->ay0 = (unsigned short)miny;
    s->ax1 = (unsigned short)maxx + 1;
    s->ay1 = (unsigned short)maxy + 1;
    s->bx0 = (unsigned short)minx;
    s->by0 = (unsigned short)miny;
    s->bx1 = (unsigned short)maxx;
    s->by1 = (unsigned short)maxy;
    s->size = 0x32;
    dd_RGB(d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
    dst = (char *)s + s->size;
    p = src + y * pitch + x;
    w = 0;
    if (s->h != 0) {
        do {
            memcpy(dst, p, s->w * 2);
            s->npix += s->w;
            s->size += s->w * 2;
            p += pitch;
            dst += s->w * 2;
            w++;
        } while (w < s->h);
    }
    s->lineoff = 0;
    s->mode = mode;
    s->nruns = -1;
    r = mem_Alloc(s->size, "shp_t:ShapeHeader");
    memcpy(r, s, s->size);
    mem_Free(s);
    return r;
}
/* 0x47ad40, grabs a rectangle of an 8 bit surface into a run-length coded shape (memory tag
   "d2:shp:_GrabBit8:ShapeHeader"): scans the rectangle for the bounding box of the non-zero pixels, clears the
   palette pixels of the transparent colour and of the start / end marker colours (the markers move the box),
   then stores every row as skip runs of zero pixels alternating with literal runs, builds the table of row
   offsets, and returns an exactly sized copy of the shape (allocated with mem_Alloc; the scratch shape is
   freed). @deep match-base 2026-10-31: read against the original (346 vs 353 instructions, was 336; differing 177, was 333). The bounding-box scan re-reads src[(y + i) * pitch + x + j] != 0 in each of the four tests (the original re-tests dl after every store; a char local let VC6 thread them away); the marker scan reads the pixel into c (unsigned char) and stores minx / miny (maxx / maxy) before clearing p[j]; the run loop counts rows with i (int, lines[i] unmasked) and the row coordinate j = y + i, keeps run as unsigned int * computed after every size change, the literal run tests src[run[1] + t + col + x] directly, adds run[1] in both arms of if (run[1] > 0) (the - 1 in one), and the tests are s->h > 0 / s->w > 0 (jbe). Left: w / h in edi / esi swapped against ours and the box variables in other slots (frame 0xda0 vs 0xd9c: the original keeps the masked pitch in a stack temp [esp+0x2c] and recomputes j * pitch for the skip-run row where ours reuses t), scheduling of the stores around mem_Alloc. */

D2SHP *d2_GrabShape8Runs(int x, int y, int w, int h, unsigned char *src, unsigned char mode, unsigned short pitch)
{
    int i, j, cnt, runs, col, t;
    unsigned short minx, miny, maxx, maxy;
    int lines[864];
    unsigned char *p, *row;
    unsigned char c;
    D2SHP *s, *r;
    int *line;
    unsigned int *run;

    minx = w;
    maxx = 0;
    maxy = 0;
    cnt = 0;
    miny = h;
    s = mem_Alloc((w * 8 + 4) * h + 0x32, "d2:shp:_GrabBit8:ShapeHeader");
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (src[(y + i) * pitch + x + j] != 0 && j > maxx)
                maxx = j;
            if (src[(y + i) * pitch + x + j] != 0 && j < minx)
                minx = j;
            if (src[(y + i) * pitch + x + j] != 0 && i > maxy)
                maxy = i;
            if (src[(y + i) * pitch + x + j] != 0 && i < miny)
                miny = i;
        }
    }
    if (maxx < minx || maxy < miny) {
        maxx = 0;
        minx = 0;
        maxy = 0;
        miny = 0;
    }
    s->x0 = minx;
    s->y0 = miny;
    s->w = (unsigned short)maxx + 1;
    s->h = (unsigned short)maxy + 1;
    s->ay1 = (unsigned short)maxy + 1;
    s->ax0 = minx;
    s->ay0 = miny;
    s->ax1 = (unsigned short)maxx + 1;
    for (i = 0; i < h; i++) {
        for (j = w; j > 0; j--) {
            t = src[i] * 4;
            if (d2_Palette[0][t] == d2_TransparentColour.r && d2_Palette[0][t + 1] == (unsigned char)d2_TransparentColour.gb
                && d2_Palette[0][t + 2] == (unsigned char)(d2_TransparentColour.gb >> 8))
                src[i] = 0;
        }
    }
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            p = src + (i + y) * pitch + x;
            c = p[j];
            t = c * 4;
            if (d2_Palette[0][t] == d2_StartMarkerColour.r && d2_Palette[0][t + 1] == (unsigned char)d2_StartMarkerColour.gb
                && d2_Palette[0][t + 2] == (unsigned char)(d2_StartMarkerColour.gb >> 8)) {
                minx = j;
                miny = i;
                p[j] = 0;
            }
            if (d2_Palette[0][t] == d2_EndMarkerColour.r && d2_Palette[0][t + 1] == (unsigned char)d2_EndMarkerColour.gb
                && d2_Palette[0][t + 2] == (unsigned char)(d2_EndMarkerColour.gb >> 8)) {
                maxx = j;
                maxy = i;
                p[j] = 0;
            }
        }
    }
    s->bx1 = (unsigned short)maxx;
    s->bx0 = (unsigned short)minx;
    s->by0 = (unsigned short)miny;
    s->by1 = (unsigned short)maxy;
    s->size = 0x32;
    i = 0;
    if (s->h > 0) {
        j = y;
        do {
            runs = 0;
            line = (int *)((char *)s + s->size);
            s->size += 4;
            run = (unsigned int *)((char *)s + s->size);
            lines[i] = (char *)line - (char *)s;
            col = 0;
            if (s->w > 0) {
                t = j * pitch;
                row = src + x + t;
                do {
                    run[0] = 0;
                    run[1] = 0;
                    if (row[col] == 0) {
                        row = src + j * pitch + x;
                        do {
                            if (col >= s->w)
                                break;
                            col++;
                            run[0]++;
                        } while (row[col] == 0);
                    }
                    while (src[run[1] + t + col + x] != 0 && run[1] + col < s->w)
                        run[1]++;
                    memcpy(run + 2, row + col, run[1]);
                    if (run[1] > 0)
                        col += run[1] - 1;
                    else
                        col += run[1];
                    s->npix += run[1];
                    s->size += run[1] + 8;
                    run = (unsigned int *)((char *)s + s->size);
                    cnt++;
                    runs++;
                    col++;
                } while (col < s->w);
            }
            *line = runs;
            i++;
            j++;
        } while (i < s->h);
    }
    s->lineoff = s->size;
    s->size += s->h * 4;
    d2_SetLineTable(lines, s);
    s->mode = mode;
    s->nruns = cnt;
    r = mem_Alloc(s->size, "shp_t:ShapeHeader");
    memcpy(r, s, s->size);
    mem_Free(s);
    return r;
}
/* 0x47b220, grabs a rectangle of a surface into a shape; the colour depth of the surface selects the grabber */

D2SHP *d2_GrabShape(int x, int y, int w, int h, D2SURF *surf, unsigned char mode, char grab8)
{
    D2SHP *r;
    int type;

    r = 0;
    if (grab8 != 0) {
        switch ((char)surf->bpp) {
        case 8:
            r = d2_GrabShape8Runs(x, y, w, h, (unsigned char *)surf->bits, mode, (unsigned short)surf->w);
            type = 0;
            break;
        case 16:
            r = d2_GrabShape16Runs(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 1;
            break;
        case 24:
            r = d2_GrabShape24Runs(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 2;
            break;
        default:
            break;
        }
    } else {
        switch ((char)surf->bpp) {
        case 16:
            r = d2_GrabShape16NoRuns(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 1;
            break;
        case 24:
            r = d2_GrabShape24NoRuns(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 2;
            break;
        default:
            break;
        }
    }
    if (r != 0) {
        r->type = type;
        r->mode = mode;
    }
    return r;
}
/* 0x47b350, draws a 16 bit run-length shape at 1/n size, clipped to d2_ClipLeft / d2_ClipRight / d2_ClipTop /
   d2_ClipBottom: skip runs are divided by n with the fractional accumulator carried across runs and rows, and
   every n-th source pixel of a literal run lands on the destination; returns 0 when the shape is completely
   clipped, else 1. @deep match-base 2026-10-31: read the whole function against the original; 270 vs 270 instructions (was 210), differing 24. The setup was missing from our source (cp / line / run, the four clip early-outs against ClipTop / ClipRight / ClipLeft and h clipped to ClipBottom): reconstructed; n is an unsigned char (and edi,0xff) and y an int (signed compares); the unclipped branch comes first (y >= d2_ClipTop); the run and pixel loops are for loops (k < *cp, m < (run->n + acc) / n: the jbe entry tests) and the row ends with cp = run; run++ (no next pointer). Left (allocation only): in the clipped branch ours keeps row in ebp and zeroes it with its own xor ebp,ebp where the original reloads row from its slot [esp+0x2c] and uses ebp for row % n (hunks 0x47b40d, 0x47b516, 0x47b5f0, 0x47b60e). */

int d2_ShpBlit16Scaled(int x, int y, D2SURF *surf, D2SHP *s, unsigned char n)
{
    int h, row, first;
    unsigned int acc, k, m;
    unsigned int *cp;
    D2RUN *run;
    unsigned short *line, *d, *px;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "Scaled", s->w, s->h, x, y, s->mode, surf->bpp));
#endif
    cp = (unsigned int *)(s + 1);
    line = surf->bits + (surf->pitch * y + x);
    run = (D2RUN *)((unsigned int *)(s + 1) + 1);
    if (s->h + y < d2_ClipTop)
        return 0;
    if (s->w / n + x > d2_ClipRight)
        return 0;
    if (s->w + x < d2_ClipLeft)
        return 0;
    if (s->h / n + y > d2_ClipBottom)
        h = (d2_ClipBottom - y) * n;
    else
        h = s->h;
    acc = 0;
    if (y >= d2_ClipTop) {
        for (row = 0; row < h; row++) {
            first = row % n == 0;
            d = line;
            for (k = 0; k < *cp; k++) {
                d += ((run->skip >> 1) + acc) / n;
                acc++;
                acc = (run->skip >> acc) % n;
                if (first) {
                    px = (unsigned short *)(run + 1);
                    for (m = 0; m < (run->n + acc) / n; m++) {
                        *d = *px;
                        px += n;
                        d++;
                    }
                    acc = (run->n + acc) % n;
                }
                run = (D2RUN *)((unsigned short *)(run + 1) + run->n);
            }
            if (first)
                line += surf->pitch;
            cp = (unsigned int *)run;
            run = (D2RUN *)(cp + 1);
        }
    } else {
        for (row = 0; row < h; row++) {
            first = row % n == 0;
            d = line;
            for (k = 0; k < *cp; k++) {
                d += ((run->skip >> 1) + acc) / n;
                acc++;
                acc = (run->skip >> acc) % n;
                if (first) {
                    px = (unsigned short *)(run + 1);
                    for (m = 0; m < (run->n + acc) / n; m++) {
                        if (row / n + y >= d2_ClipTop)
                            *d = *px;
                        px += n;
                        d++;
                    }
                    acc = (run->n + acc) % n;
                }
                run = (D2RUN *)((unsigned short *)(run + 1) + run->n);
            }
            if (first)
                line += surf->pitch;
            cp = (unsigned int *)run;
            run = (D2RUN *)(cp + 1);
        }
    }
    return 1;
}
/* 0x47b640, the light-table variant of d2_ShpBlit16Scaled: the same 1/n scaled, clipped draw of a 16 bit
   run-length shape, but every destination pixel goes through d2_ShpLightTable (used when the shape was grabbed
   with a palette); returns 0 when completely clipped, else 1.
   @deep behaviour draft (332 code hunks, 619/720 bytes): the light-table twin of 0x47b350 - every meaning read against 47b640..47b8ff and corresponds (the acc arithmetic made explicit: acc++; acc = (run->skip >> acc) % n, byte-neutral; the two clipped branches, d += ((*run>>1)+acc)/n, first = row%n==0, the literal run through d2_ShpLightTable[*px] every nth source pixel, the acc remainder, the run chain, line += pitch on first, early-outs vs ClipTop/ClipRight/ClipLeft/ClipBottom). The remaining hunks are the same codegen-only register/slot homes as 0x47b350 (line/acc/row spilled into parameter slots here, other homes in the original); no source meaning fix found */

int d2_ShpBlit16ScaledLit(int x, unsigned int y, D2SURF *surf, D2SHP *s, unsigned int n)
{
    int h, row, first;
    unsigned int acc, k, m;
    unsigned int *cp, *next;
    D2RUN *run;
    unsigned short *line, *d, *px;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "ScaledLit", s->w, s->h, x, y, s->mode, surf->bpp));
#endif

    cp = (unsigned int *)(s + 1);
    run = (D2RUN *)((unsigned int *)(s + 1) + 1);
    line = surf->bits + (surf->pitch * y + x);
    h = s->h;
    if ((int)(h + y) < d2_ClipTop)
        return 0;
    if ((int)(h / n + y) > d2_ClipBottom)
        h = (d2_ClipBottom - y) * n;
    acc = 0;
    if ((int)y < d2_ClipTop) {
        for (row = 0; row < h; row++) {
            first = row % n == 0;
            d = line;
            next = (unsigned int *)run;
            if (*cp != 0) {
                k = 0;
                do {
                    d += ((run->skip >> 1) + acc) / n;
                    acc++;
                    acc = (run->skip >> acc) % n;
                    if (first) {
                        px = (unsigned short *)(run + 1);
                        m = 0;
                        if ((run->n + acc) / n != 0) {
                            do {
                                if (d2_ClipTop <= row / n + (int)y)
                                    *d = d2_ShpLightTable[*px];
                                px += n;
                                d++;
                                m++;
                            } while (m < (run->n + acc) / n);
                        }
                        acc = (run->n + acc) % n;
                    }
                    k++;
                    run = (D2RUN *)((unsigned short *)(run + 1) + run->n);
                    next = (unsigned int *)run;
                } while (k < *cp);
            }
            if (first)
                line += surf->pitch;
            run = (D2RUN *)(next + 1);
            cp = next;
        }
    } else {
        for (row = 0; row < h; row++) {
            first = row % n == 0;
            d = line;
            next = (unsigned int *)run;
            if (*cp != 0) {
                k = 0;
                do {
                    d += ((run->skip >> 1) + acc) / n;
                    acc++;
                    acc = (run->skip >> acc) % n;
                    if (first) {
                        px = (unsigned short *)(run + 1);
                        m = 0;
                        if ((run->n + acc) / n != 0) {
                            do {
                                *d = d2_ShpLightTable[*px];
                                px += n;
                                d++;
                                m++;
                            } while (m < (run->n + acc) / n);
                        }
                        acc = (run->n + acc) % n;
                    }
                    k++;
                    run = (D2RUN *)((unsigned short *)(run + 1) + run->n);
                    next = (unsigned int *)run;
                } while (k < *cp);
            }
            if (first)
                line += surf->pitch;
            run = (D2RUN *)(next + 1);
            cp = next;
        }
    }
    return 1;
}
/* 0x47b900, draws a 16 bit run-length shape half dark: every pixel of the literal runs is replaced by its grey
   (r + b) * 0.2f + g * 0.59f through dd_RGB, the skip runs are stepped over (the x / y parameters are reused as
   the run / pixel counters).
   @deep behaviour draft (186 code hunks, 302/336 bytes, frame 0x18 vs 0x1c): the meaning was read against 47b900..47ba5f and corresponds - line = bits + (pitch*y + x), the row loop over s->h reusing the x and y parameters as the run / pixel counters, d += run->skip >> 1, the pixel loop with dd_SplitRGB(*px, &r,&g,&b), v = (int)((r + b) * 0.2f + g * 0.59f) (the original's fild/fiadd/fmul 0.2f, fild/fmul 0.59f, faddp, _ftol), dd_RGB(v,v,v), the run chain run + run->n*2 + 8 and cp = run, line += pitch. What differs is codegen only: frame 0x18 vs 0x1c and other register/slot homes for cp/run/line/d; no source meaning fix found */

void d2_ShpBlit16HalfDark(unsigned int x, unsigned int y, D2SURF *surf, D2SHP *s)
{
    unsigned int *cp;
    D2RUN *run;
    unsigned int i;
    unsigned short *line, *d;
    unsigned short *px;
    int r, g, b;
    int v;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "HalfDark", s->w, s->h, x, y, s->mode, surf->bpp));
#endif

    cp = (unsigned int *)(s + 1);
    line = surf->bits + (surf->pitch * y + x);
    run = (D2RUN *)((unsigned int *)(s + 1) + 1);
    for (i = 0; i < s->h; i++) {
        d = line;
        for (x = 0; x < *cp; x++) {
            px = (unsigned short *)(run + 1);
            d += run->skip >> 1;
            for (y = 0; y < run->n; y++) {
                dd_SplitRGB(*px, (unsigned char *)&r, (unsigned char *)&g, (unsigned char *)&b);
                v = (int)(((float)(r & 0xff) + (b & 0xff)) * 0.2f + (g & 0xff) * 0.59f);
                *d = (unsigned short)dd_RGB(v, v, v);
                px++;
                d++;
            }
            run = (D2RUN *)((unsigned short *)(run + 1) + run->n);
        }
        cp = (unsigned int *)run;
        line += surf->pitch;
        run = (D2RUN *)(cp + 1);
    }
}
/* 0x47ba60, draws a 16 bit shape made of plain rows (nruns == -1) onto a surface, clipped to the ClipLeft /
   ClipRight / ClipTop / ClipBottom rectangle: mode 0 copies the non-zero pixels, mode 2 halves them through
   d2_HighBitMask, mode 3 maps them through d2_ShpLightTable; d2_ShpW / d2_ShpH are set to the shape size.
   @deep behaviour draft (402 code hunks, 670/768 bytes): every meaning was read against 47ba60..47bd6f and corresponds - the x clip (ClipLeft -> skipl, ClipRight -> skipr, return when outside or when w+x is left of ClipLeft), the y clip (ClipTop -> rows, ClipBottom -> h = ClipBottom - y, the two early returns), clipped = 1 for both, d = bits + (pitch*y + x), the six loop copies (clipped/unclipped x mode 0/2/3; mode 2 is *d = *d >> 1 & d2_HighBitMask, mode 3 is d2_ShpLightTable[*d], both only when *p != 0), p += w*rows / skipl / skipr, d += pitch - vis, and the d2_ShpH / d2_ShpW stores. What remains is codegen / register allocation only (the 6 loop copies compile with other register and slot homes, 12 regs hunks) - no source meaning fix found */

void d2_ShpBlit16Rows(int x, int y, D2SURF *surf, D2SHP *s)
{
    unsigned int w, h, n;
    int clipped, skipl, skipr, vis, rows;
    unsigned short *d, *p;
    char mode;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d bpp=%d\n", GetTickCount(), "Rows", s->w, s->h, x, y, s->mode, surf->bpp));
#endif

    w = s->w;
    h = s->h;
    clipped = 0;
    skipl = 0;
    skipr = 0;
    vis = w;
    if (x < d2_ClipLeft) {
        rows = d2_ClipLeft - x;
        x = d2_ClipLeft;
        skipl = rows;
    clip1:
        clipped = 1;
        vis = w - rows;
    } else {
        if (x > d2_ClipRight)
            return;
        if ((int)(w + x) < d2_ClipLeft)
            return;
        if ((int)(w + x) > d2_ClipRight) {
            rows = (w - d2_ClipRight) + x;
            skipr = rows;
            goto clip1;
        }
    }
    if (y < d2_ClipTop) {
        rows = d2_ClipTop - y;
        h = h - rows;
        y = d2_ClipTop;
    } else {
        if (y > d2_ClipBottom)
            return;
        if ((int)(h + y) < d2_ClipTop)
            return;
        rows = 0;
        if ((int)(h + y) <= d2_ClipBottom)
            goto clip2;
        h = d2_ClipBottom - y;
    }
    clipped = 1;
clip2:
    d = surf->bits + (surf->pitch * y + x);
    mode = s->mode;
    p = (unsigned short *)(s + 1);
    if (clipped) {
        if (mode == 0) {
            p = p + w * rows;
            for (; h > 0; h--) {
                p = p + skipl;
                for (n = vis; n > 0; n--) {
                    if (*p != 0)
                        *d = *p;
                    d++;
                    p++;
                }
                p = p + skipr;
                d = d + (surf->pitch - vis);
            }
        } else if (mode == 2) {
            p = p + w * rows;
            for (; h > 0; h--) {
                p = p + skipl;
                for (n = vis; n > 0; n--) {
                    if (*p != 0)
                        *d = *d >> 1 & d2_HighBitMask;
                    d++;
                    p++;
                }
                p = p + skipr;
                d = d + (surf->pitch - vis);
            }
        } else if (mode == 3) {
            p = p + w * rows;
            for (; h > 0; h--) {
                p = p + skipl;
                for (n = vis; n > 0; n--) {
                    if (*p != 0)
                        *d = d2_ShpLightTable[*d];
                    d++;
                    p++;
                }
                p = p + skipr;
                d = d + (surf->pitch - vis);
            }
        }
    } else if (mode == 0) {
        for (; h > 0; h--) {
            for (n = vis; n > 0; n--) {
                if (*p != 0)
                    *d = *p;
                d++;
                p++;
            }
            d = d + (surf->pitch - vis);
        }
    } else if (mode == 2) {
        for (; h > 0; h--) {
            for (n = vis; n > 0; n--) {
                if (*p != 0)
                    *d = *d >> 1 & d2_HighBitMask;
                d++;
                p++;
            }
            d = d + (surf->pitch - vis);
        }
    } else if (mode == 3) {
        for (; h > 0; h--) {
            for (n = vis; n > 0; n--) {
                if (*p != 0)
                    *d = d2_ShpLightTable[*d];
                d++;
                p++;
            }
            d = d + (surf->pitch - vis);
        }
    }
    d2_ShpH = s->h;
    d2_ShpW = s->w;
}
/* 0x47bd70, draws a run-length shape at x / y onto the surface, dispatching on the shape type: 8 bit to the
   palette blitters, 16 bit to d2_ShpBlit16ClipXY (clipped horizontally) or d2_ShpBlit16ClipY, 24 bit is not
   drawn at all; sets d2_SurfPitch from the surface for the blitters, restores it on the not-drawn path.
   Returns 1 if the shape was drawn (or nothing was to be drawn), 0 otherwise. */

int d2_ShpBlit(int x, int y, D2SURF *surf, D2SHP *s)
{
    int right, old, w;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit %dx%d at %d,%d type=%d mode=%d nruns=%d size=%u surf %dx%d bpp=%d pitch=%d clip %d,%d-%d,%d lte=%d\n",
                        GetTickCount(), s->w, s->h, x, y, s->type, s->mode, s->nruns, s->size, surf->w, surf->h,
                        surf->bpp, surf->pitch, d2_ClipLeft, d2_ClipTop, d2_ClipRight, d2_ClipBottom, d2_LightTableExternal));
#endif
    if (d2_LightTableExternal == 0) {
        if (s->nruns != -1) {
            w = s->w;                     /* the local keeps the original's register layout (w in edx) */
            if (x <= d2_ClipRight && w + x >= d2_ClipLeft) {
                right = w + x;            /* the original computes the sum once for the check and keeps it in right */
                old = d2_SurfPitch;
                d2_SurfPitch = surf->pitch;
                switch (s->type) {
                case 0:
                    if (d2_PaletteDepth == 0) {
                        d2_ShpBlit8(x, y, surf->bits, s);
                        return 1;
                    }
                    d2_ShpBlit8To16(x, y, surf->bits, s);
                    return 1;
                case 1:
                    if (x < d2_ClipLeft) {
                        d2_ShpBlit16ClipXY(x, y, surf->bits, s);
                        return 1;
                    }
                    if (right > d2_ClipRight) {
                        d2_ShpBlit16ClipXY(x, y, surf->bits, s);
                        return 1;
                    }
                    d2_ShpBlit16ClipY(x, y, surf->bits, s);
                    return 1;
                case 2:
                    return 1;
                }
                d2_SurfPitch = old;
            }
        } else {
            switch (s->type) {
            case 0:
                return 0;
            case 2:
                return 1;
            case 1:
                d2_ShpBlit16Rows(x, y, surf, s);
                return 1;
            }
        }
    }
    return 0;
}
/* 0x47bec0, converts an 8 bit shape (palette at 0xf8a120) into a 16 bit one @deep B3, 301 code hunks: the whole function's register allocation differs - the shape pointer s is in ebp where the original has ebx, the result r in ebx where the original has ebp (prologue 0x47beca / 0x47beef), and every read hunk (alloc, memcpy, run counts, pixel loop) is that swap plus scheduling; no meaning error found: the offsets 0x26 / 0x2e / 0x32 / 0x36, the dd_RGB arguments, the mem tag and the loop bounds correspond */

D2SHP *d2_ShpConvert8To16(D2SHP *s)
{
    unsigned short pal[256];
    unsigned short buf[0x480];
    int lines[864];
    int i, n;
    unsigned int j, k, cnt;
    unsigned int *src, *srccnt, *dst, *dstcnt;
    unsigned char *pix;
    D2SHP *r;

    if (s->nruns == -1) {
        err_Message("shp_Convert8To16: Converting of NoReadAndSkip Shapes not surported...");
        return 0;
    }
    r = mem_Alloc(s->size + s->npix, "d2:shp:NewShape");
    memcpy(r, s, 0x32);
    r->nruns = 0;
    r->npix = 0;
    r->size = 0x32;
    r->type = 1;
    for (i = 0; i < 256; i++)
        pal[i] = dd_RGB(d2_Palette[i][0], d2_Palette[i][1], d2_Palette[i][2]);
    src = (unsigned int *)(s + 1) + 1;
    dst = (unsigned int *)(r + 1);
    dstcnt = (unsigned int *)(r + 1) + 1;
    srccnt = (unsigned int *)(s + 1);
    for (i = 0; i < s->h; i++) {
        *dst = *srccnt;
        lines[i] = (char *)dst - (char *)r;
        r->size += 4;
        if (*srccnt != 0) {
            for (j = 0; j < *srccnt; j++) {
                *dstcnt = *src * 2;
                dstcnt[1] = src[1];
                n = src[1];
                if (n != 0) {
                    for (k = 0; k < (unsigned int)n; k++) {
                        buf[k] = pal[((unsigned char *)src)[k + 8]];
                        if (s->mode == 1)
                            buf[k] = buf[k] >> 1 & d2_HighBitMask;
                    }
                }
                memcpy(dstcnt + 2, buf, n * 2);
                r->nruns++;
                r->npix += src[1];
                r->size += src[1] * 2 + 8;
                src = (unsigned int *)((char *)src + src[1] + 8);
                dstcnt = (unsigned int *)((char *)dstcnt + dstcnt[1] * 2 + 8);
            }
        }
        dst = dstcnt;
        dstcnt = dstcnt + 1;
        srccnt = src;
        src = src + 1;
    }
    r->lineoff = r->size;
    r->size += r->h * 4;
    d2_SetLineTable(lines, r);
    return r;
}
/* 0x47c0e0, converts a 24 bit shape into a 16 bit one @deep B3, 39 code + 30 order hunks after the unsigned guard fix (0 < src->n, jbe now matches): the rest is scheduling (npix/size loads hoisted before the branch, the lea placed before vs after the stores, xor edi,edi hoisted) and one addressing form (the original keeps pix+1 and reads -1/+0/+1, ours pix with 0/+1/+2), no meaning error found in the read hunks */

D2SHP *d2_ShpConvert24To16(D2SHP *s)
{
    unsigned short buf[0x480];
    int lines[864];
    int i, j;
    unsigned int k, m;
    unsigned int *srccnt, *dst, *dstcnt;
    D2RUN *src;
    unsigned char *pix;
    unsigned short c;
    D2SHP *r;

    if (s->nruns != -1) {
        r = mem_Alloc(s->size - s->npix, "d2:shp:NewShape");
        memcpy(r, s, 0x32);
        r->size = 0x32;
        r->type = 1;
        r->nruns = 0;
        r->npix = 0;
        src = (D2RUN *)((unsigned int *)(s + 1) + 1);
        dst = (unsigned int *)(r + 1);
        dstcnt = (unsigned int *)(r + 1) + 1;
        srccnt = (unsigned int *)(s + 1);
        for (i = 0; i < s->h; i++) {
            *dst = *srccnt;
            lines[i] = (char *)dst - (char *)r;
            r->size += 4;
            m = 0;
            if (0 < *srccnt) {
                do {
                    *dstcnt = (src->skip * 2) / 3;
                    dstcnt[1] = src->n;
                    if (0 < src->n) {
                        pix = (unsigned char *)(src + 1);
                        k = 0;
                        do {
                            c = dd_RGB(pix[0], pix[1], pix[2]);
                            buf[k] = c;
                            if (c == 0)
                                buf[k] = dd_RGB(5, 5, 5);
                            if (s->mode == 1)
                                buf[k] = buf[k] >> 1 & d2_HighBitMask;
                            k++;
                            pix += 3;
                        } while (k < src->n);
                    }
                    memcpy(dstcnt + 2, buf, src->n * 2);
                    r->nruns++;
                    r->npix += src->n;
                    r->size += src->n * 2 + 8;
                    src = (D2RUN *)((unsigned char *)(src + 1) + src->n * 3);
                    dstcnt = (unsigned int *)((char *)dstcnt + dstcnt[1] * 2 + 8);
                    m++;
                } while (m < *srccnt);
            }
            dst = dstcnt;
            dstcnt = dstcnt + 1;
            srccnt = (unsigned int *)src;
            src = (D2RUN *)(srccnt + 1);
        }
        r->lineoff = r->size;
        r->size += r->h * 4;
        d2_SetLineTable(lines, r);
#ifdef REA_FRAME_DUMP
        if (r->w > 300) {
            char rd_path[64];
            REA_DUMP_TRACE((rd_f, "%u Conv24 %dx%d mode=%d nruns=%d npix=%d size=%u -> nruns=%d size=%u\n", GetTickCount(),
                            s->w, s->h, s->mode, s->nruns, s->npix, s->size, r->nruns, r->size));
            sprintf(rd_path, "dump/shape24src_%dx%d_%u.bin", s->w, s->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, s, s->size);
            sprintf(rd_path, "dump/shape16_%dx%d_%u.bin", r->w, r->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, r, r->size);
        }
#endif
        return r;
    } else {
        r = mem_Alloc(s->size - s->npix, "d2:shp:NewShape");
        memcpy(r, s, 0x32);
        r->nruns = -1;
        r->size = 0x32;
        r->type = 1;
        r->npix = 0;
        dst = (unsigned int *)(r + 1);
        for (i = 0; i < s->h; i++) {
            for (j = 0; j < s->w; j++) {
                pix = (unsigned char *)(s + 1) + (s->w * i + j) * 3;
                buf[j] = dd_RGB(pix[0], pix[1], pix[2]);
                if (s->mode == 1)
                    buf[j] = buf[j] >> 1 & d2_HighBitMask;
            }
            memcpy(dst, buf, s->w * 2);
            dst = (unsigned int *)((char *)dst + s->w * 2);
            r->size += s->w * 2;
        }
        r->lineoff = 0;
#ifdef REA_FRAME_DUMP
        if (r->w > 300) {
            char rd_path[64];
            REA_DUMP_TRACE((rd_f, "%u Conv24 %dx%d mode=%d nruns=%d npix=%d size=%u -> nruns=%d size=%u\n", GetTickCount(),
                            s->w, s->h, s->mode, s->nruns, s->npix, s->size, r->nruns, r->size));
            sprintf(rd_path, "dump/shape24src_%dx%d_%u.bin", s->w, s->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, s, s->size);
            sprintf(rd_path, "dump/shape16_%dx%d_%u.bin", r->w, r->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, r, r->size);
        }
#endif
        return r;
    }
}
/* 0x47c3d0, converts a shape to 16 bit (p must be 1); 0 if it is of another kind */

void *d2_ShpConvertTo16(D2SHP *s, int p)
{
    if (s->type == 2 && p == 1)
        return d2_ShpConvert24To16(s);
    if (s->type == 0 && p == 1)
        return d2_ShpConvert8To16(s);
    return 0;
}
/* 0x47c410, converts a bank of shapes to 16 bit (a = 1); b = 0 frees the old bank @deep B3, 17/464 bytes with 32 code + 32 regs hunks: only the size / offset-pointer register pair differs (original size in ecx, pointer in edx); local copies, pointer and countdown loops, declaration order and size+total were tried in the d2s round (unit note), so what is left is the allocator, not the C */

void *d2_BankConvertTo16(void *pic, int a, int b)
{
    D2BANK *bank, *nb;
    int i, total, size;
    D2SHP *sh;

    bank = (D2BANK *)pic;
    size = bank->size;
    total = 0;
    if (size == 0)
        return 0;
    for (i = 0; i < bank->n; i++)
        total += ((D2SHP *)((char *)bank + bank->offs[i]))->npix;
    if (bank->kind == 2 && a == 1) {
        nb = mem_Alloc(size - total, "d2:shp:NewBank");
        memcpy(nb, bank, 0x845);
        nb->size = 0;
        nb->kind = 1;
        nb->n = 0;
        for (i = 0; i < bank->n; i++) {
            sh = d2_ShpConvertTo16((D2SHP *)((char *)bank + bank->offs[i]), 1);
            d2_BankAppendShape(sh, nb);
            if (sh != 0)
                mem_Free(sh);
        }
        if (bank->pal != 0) {
            nb->pal = nb->size;
            memcpy((char *)nb + nb->size, (char *)bank + bank->pal, bank->npal * 8);
        }
        if (b == 0) {
            mem_Free(bank);
            return nb;
        }
    } else {
        if (bank->kind != 0 || a != 1)
            return bank;
        nb = mem_Alloc(total + size, "d2:shp:NewBank");
        memcpy(nb, bank, 0x845);
        nb->size = 0;
        nb->kind = 1;
        nb->n = 0;
        for (i = 0; i < bank->n; i++) {
            sh = d2_ShpConvertTo16((D2SHP *)((char *)bank + bank->offs[i]), 1);
            d2_BankAppendShape(sh, nb);
            if (sh != 0)
                mem_Free(sh);
        }
        if (bank->pal != 0) {
            nb->pal = nb->size;
            memcpy((char *)nb + nb->size, (char *)bank + bank->pal, bank->npal * 8);
        }
        if (b == 0)
            mem_Free(bank);
    }
    return nb;
}

/* d2.h has other types for these three */
int d2_DrawShapeHalfDark(int x, int y, void *surf, void *pic, char i);              /* 0x47c770, d2.h has void */
int d2_StartAnim(short x, short y, void *data, unsigned char z);             /* 0x47cd20, starts an animation of a bank at (x, y); its number or -1 */
int d2_DrawShapeMode2(int x, int y, void *surf, void *pic, unsigned char i);     /* 0x47c970, d2.h has void */

/* 0x47c5e0, stores a palette into a bank */
int d2_BankSetPalette(D2BANK *bank, void *pal)
{
    if (bank == 0)
        return 0;
    memcpy(bank->palette, pal, 0x400);
    return 1;
}
/* 0x47c610, appends a shape to a bank */

int d2_BankAppendShape(D2SHP *s, D2BANK *bank)
{
    char tag[] = "SHAPBANK";

    if (bank->n >= 0xff)
        return 0;
    if (bank->size == 0 || bank->n == 0) {
        bank->size = 0x845;
        memcpy(bank, tag, 8);
        bank->version = 1;
        bank->kind = s->type;
        if (bank->kind == 0)
            d2_BankSetPalette(bank, d2_Palette);
    }
    if ((unsigned char)bank->kind != s->type)
        return 0;
    memcpy((char *)bank + bank->size, s, s->size);
    bank->offs[bank->n] = bank->size;
    bank->size += s->size;
    if (s->w > bank->w)
        bank->w = s->w;
    if (s->h > bank->h)
        bank->h = s->h;
    bank->n++;
    return 1;
}
/* 0x47c6f0, draws shape i of a bank; 1 if drawn */

int d2_DrawShape(int x, int y, void *surf, void *pic, unsigned char i)
{
    D2BANK *bank = (D2BANK *)pic;
    char buf[128];

    if (bank == 0)
        return 0;
    if (i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", bank->name);
        return 0;
    }
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[i]));
    return 1;
}
/* 0x47c770, draws a half dark shape i of a bank */

int d2_DrawShapeHalfDark(int x, int y, void *surf, void *pic, char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    int old;
    char buf[128];

    if (bank == 0)
        return 0;
    if ((unsigned char)i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", bank->name);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]);
    if (d2_LightTableExternal == 0) {
        old = d2_SurfPitch;
        d2_SurfPitch = ((D2SURF *)surf)->pitch;
        switch (s->type) {
        case 0:
            return 0;
        case 1:
            d2_ShpBlit16HalfDark(x, y, (D2SURF *)surf, s);
            return 1;
        case 2:
            return 1;
        }
        d2_SurfPitch = old;
    }
    return 1;
}
/* 0x47c830, draws shape i of a bank scaled down (mode < 0) @deep B3, 150 code hunks: pure register allocation and scheduling - the original pushes edi at entry (keeps the surface pointer / bank in edi across the switch) and puts eax=1 early, ours uses esi/ ebp and shifts every stack slot by the missing push; the read hunks are the same operations with other registers (and eax,0xff vs and ecx,0xff, dec eax vs dec edx, mov [..],edx vs mov [..],edi) while the callees, constants, the switch on s->type and the mode / on conditions correspond; the ret=1 variable form gave the same bytes */

int d2_DrawShapeScaled(int x, int y, void *surf, void *pic, char i, char mode, int on)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    int n, old;
    char buf[128];

    if (bank == 0)
        return 0;
    if ((unsigned char)i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", bank->name);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]);
    if (d2_LightTableExternal == 0) {
        old = d2_SurfPitch;
        d2_SurfPitch = ((D2SURF *)surf)->pitch;
        switch (s->type) {
        case 0:
            return 0;
        case 1:
            if (mode >= 0)
                return 1;
            if (on != 0)
                return d2_ShpBlit16ScaledLit(x, y, (D2SURF *)surf, s, abs(mode));
            return d2_ShpBlit16Scaled(x, y, (D2SURF *)surf, s, abs(mode));
        case 2:
            return 1;
        }
        d2_SurfPitch = old;
    }
    n = abs(mode);
    d2_ShpW = s->w / n;
    d2_ShpH = s->h / n;
    return 1;
}
/* 0x47c970, draws shape i of a bank in mode 2 (half dark) */

int d2_DrawShapeMode2(int x, int y, void *surf, void *pic, unsigned char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    char old;
    char buf[128];

    if (bank == 0)
        return 0;
    if (i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", bank->name);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[i]);
    old = s->mode;
    s->mode = 2;
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[i]));
    s->mode = old;
    return 1;
}
/* 0x47ca10, sets the colour of mode 5 shapes */

void d2_SetMode5Colour(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int c = dd_RGB(r, g, b);

    d2_Mode5Colour = c << 16 | c;
}
/* 0x47ca40, draws shape i of a bank in mode 5 (one colour) */

int d2_DrawShapeMode5(int x, int y, void *surf, void *pic, unsigned char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    char old;
    char buf[128];

    if (bank == 0)
        return 0;
    if (i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", bank->name);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[i]);
    old = s->mode;
    s->mode = 5;
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[i]));
    s->mode = old;
    return 1;
}
/* 0x47cae0, draws shape i of a bank in mode 3 (light table) */

int d2_DrawShapeMode3(int x, int y, void *surf, void *pic, char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    char old;

    if ((unsigned char)i > bank->n)
        return 0;
    s = (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]);
    old = s->mode;
    s->mode = 3;
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]));
    s->mode = old;
    return 1;
}
/* 0x47cb40, sets the clip rectangle */

void d2_SetClipRect(int x0, int y0, int x1, int y1)
{
    d2_ClipLeft = x0;
    d2_ClipTop = y0;
    d2_ClipRight = x1;
    d2_ClipBottom = y1;
}
/* 0x47cb70, shape i of a bank */

D2FRAME *d2_GetFrame(void *pic, short i)
{
    D2BANK *bank = (D2BANK *)pic;

    if (bank == 0)
        return 0;
    return (D2FRAME *)((char *)bank + bank->offs[(unsigned short)i]);
}
/* 0x47cb90, steps all animations */

void d2_StepAnims(void)
{
    int i, ok;
    int step;
    char back;
    D2ANIM *a;
    D2BANK *b;
    unsigned char *t;

    ok = 1;
    if (tb_AnimTick - d2_LastAnimTick != 0) {
        if (tb_AnimTick - d2_LastAnimTick < 1)
            step = 1;
        else
            step = tb_AnimTick - d2_LastAnimTick;
        d2_LastAnimTick = tb_AnimTick;
        for (i = 0; i < 16; i++) {
            a = &d2_Anims[i];
            b = a->bank;
            if (b != 0 && a->frame >= 0) {
                a->timer = a->timer + step;
                t = (unsigned char *)b + b->pal;
                if ((unsigned char)a->timer > t[a->frame * 8 + 1]) {
                    back = a->back;
                    if (back == 0)
                        a->frame++;
                    if (a->back == 1)
                        a->frame--;
                    a->timer = 0;
                    if (a->frame >= b->npal && back == 0) {
                        if (b->pad42 == 0) {
                            a->frame = 0;
                            a->shape = t[0];
                        }
                        if (b->pad42 == 1) {
                            a->frame = -1;
                            a->shape = t[b->npal * 8 - 8];
                        }
                        if (b->pad42 == 3) {
                            a->frame = 0;
                            a->shape = t[0];
                            d2_FreeAnim(i);
                        }
                        ok = 0;
                    }
                    if (a->frame < 0 && a->back == 1) {
                        if (b->pad42 == 0) {
                            a->frame = b->npal - 2;
                            a->shape = t[a->frame * 8];
                        }
                        if (b->pad42 == 1) {
                            a->frame = -1;
                            a->shape = t[0];
                        }
                        if (b->pad42 == 3) {
                            a->frame = 0;
                            a->shape = t[0];
                            d2_FreeAnim(i);
                        }
                        ok = 0;
                    } else if (ok) {
                        a->shape = t[a->frame * 8];
                    }
                }
            }
        }
    }
}
/* 0x47cd20, starts an animation of a bank at (x, y); its number or -1 @deep B3, 24 code hunks, no meaning error: the same fields are stored with the same values and order (x, bank = data, frame 0, timer 0, y, shape from the bank palette) and the returns are i / -1; the hunks are the branch shape (the original keeps two inline or eax,-1; ret tails where VC6 merges ours into one shared tail, jge vs jl) and the parameter-load scheduling (dx / si / bl load points), i.e. codegen */

int d2_StartAnim(short x, short y, void *data, unsigned char z)
{
    int i;
    D2ANIM *a;

    for (i = 0, a = d2_Anims; (int)a < (int)((unsigned)d2_Anims + 0x110); i++, a++) {
        if (a->bank == 0) {
            if (i >= 16)
                return -1;
            d2_Anims[i].x = x;
            d2_Anims[i].bank = (D2BANK *)data;
            d2_Anims[i].frame = 0;
            d2_Anims[i].timer = 0;
            d2_Anims[i].y = y;
            d2_Anims[i].shape = 0;
            d2_Anims[i].shape = *((unsigned char *)data + ((D2BANK *)data)->pal);
            if (z & 1)
                d2_Anims[i].shadow = 1;
            return i;
        }
    }
    return -1;
}
/* 0x47cdb0, draws animation n */

void d2_DrawAnim(int n, void *surf)
{
    short x, y;

    if (d2_Anims[n].bank != 0) {
        x = d2_Anims[n].x;
        y = d2_Anims[n].y;
        if (d2_Anims[n].shadow != 0)
            d2_DrawShapeMode2(x + 6, y + 6, surf, d2_Anims[n].bank, d2_Anims[n].shape);
        d2_DrawShape(x, y, surf, d2_Anims[n].bank, d2_Anims[n].shape);
    }
}
/* 0x47ce30, frees animation n */

void d2_FreeAnim(int n)
{
    memset(&d2_Anims[n], 0, 0x11);
}
/* 0x47ce60, fills a rectangle (clipped at the top and bottom) */

void d2_FillRect(int x, int y, int w, int h, void *surf)
{
    int old;
    D2SURF *s = (D2SURF *)surf;

#ifdef REA_FRAME_DUMP
    if (w > 300)
        REA_DUMP_TRACE((rd_f, "%u FillRect %dx%d at %d,%d bpp=%d\n", GetTickCount(), w, h, x, y, s->bpp));
#endif
    if (y < d2_ClipTop) {
        h = h + (y - d2_ClipTop);
        y = 0;
    }
    if (y + h > d2_ClipBottom)
        h = d2_ClipBottom - y - 1;
    old = d2_SurfPitch;
    d2_SurfPitch = s->pitch;
    d2_ShpDarken16(s->bits + (s->pitch * y + x), w, h);
    d2_SurfPitch = old;
}
/* 0x47ced0, stores three parameters */

void d2_SetBlitParams(void *dst, void *src, void *mask, short w, short h, short n)
{
    d2_BlitH = (unsigned short)h;
    d2_BlitW = (unsigned short)w;
    d2_BlitN = (unsigned char)n;
}
/* 0x47cf00, draws a 16 bit shape, clipped at top and bottom only */

void d2_ShpBlit16ClipY(int x, int y, void *bits, D2SHP *s)
{
#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d surfpitch=%d\n", GetTickCount(), "ClipY", s->w, s->h, x, y, s->mode, d2_SurfPitch));
#endif
    __asm {
                push edi
                push esi
                push eax
                push ebx
                push edx
                push ecx
                push ebp
                mov eax,dword ptr [ebp+0x8]
                mov edx,dword ptr [ebp+0xc]
                mov ebx,dword ptr [ebp+0x10]
                mov ecx,dword ptr [ebp+0x14]
                mov edi,ebx
                mov esi,ecx
                shl eax,1
                xor ecx,ecx
                mov ebx,edx
                mov cx,word ptr [esi+0xa]
                add ebx,ecx
                cmp ebx,dword ptr d2_ClipTop
                jle LclipY_done
                cmp edx,dword ptr d2_ClipBottom
                jge LclipY_done
                push edx
                mov ebp,dword ptr d2_SurfPitch
                sub bp,word ptr [esi+0x6]
                shl ebp,1
                mov dword ptr d2_ShpRowGap,ebp
                xor ebx,ebx
                mov bx,word ptr [esi+0xa]
                mov dword ptr d2_ShpRows,ebx
                mov dword ptr d2_ShpDataOfs,0x32
                add edi,eax
                mov ebx,edx
                mov ecx,dword ptr d2_ClipTop
                sub ecx,ebx
                cmp ecx,0x0
                jle LclipY_afterTop
                mov eax,esi
                add eax,dword ptr [esi+0x2a]
                mov edx,dword ptr [eax+ecx*4]
                mov dword ptr d2_ShpDataOfs,edx
                sub dword ptr d2_ShpRows,ecx
                mov ebx,dword ptr d2_ClipTop
    LclipY_afterTop:
                mov ecx,dword ptr d2_ClipBottom
                pop edx
                mov eax,edx
                add ax,word ptr [esi+0xa]
                sub eax,ecx
                jle LclipY_afterBottom
                sub dword ptr d2_ShpRows,eax
                cmp dword ptr d2_ShpRows,0x0
                jle LclipY_done
    LclipY_afterBottom:
                mov eax,d2_SurfPitch
                shl eax,1
                mul ebx
                add edi,eax
                mov ebx,dword ptr d2_ShpRows
                cmp ebx,0x0
                je LclipY_done
                mov ax,word ptr [esi+0x6]
                mov d2_ShpW,ax
                mov ax,word ptr [esi+0xa]
                mov d2_ShpH,ax
                xor eax,eax
                mov al,BYTE ptr [esi+0xd]
                add esi,dword ptr d2_ShpDataOfs
                cmp al,0x0
                je LclipY_mode0_row
                cmp al,0x1
                je LclipY_mode1_entry
                cmp al,0x2
                je LclipY_mode2_entry
                cmp al,0x3
                je LclipY_mode3_entry
                cmp al,0x4
                je LclipY_mode4_entry
                cmp al,0x5
                je LclipY_mode5_entry
    LclipY_mode0_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode0_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode0_nextRun
                bt ecx,0x0
                jae LclipY_mode0_oddDone
                mov ax,word ptr [esi]
                mov word ptr [edi],ax
                add edi,0x2
                add esi,0x2
    LclipY_mode0_oddDone:
                shr ecx,1
                rep movsd
    LclipY_mode0_nextRun:
                dec edx
                jne LclipY_mode0_run
                add edi,ebp
                dec ebx
                jne LclipY_mode0_row
                jmp LclipY_done
    LclipY_mode3_entry:
                xor ebp,ebp
                mov eax,dword ptr d2_ShpLightTable
    LclipY_mode3_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode3_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode3_nextRun
    LclipY_mode3_pixel:
                mov bp,word ptr [edi]
                mov bp,word ptr [eax+ebp*2]
                mov word ptr [edi],bp
                add edi,0x2
                dec ecx
                jne LclipY_mode3_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    LclipY_mode3_nextRun:
                dec edx
                jne LclipY_mode3_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode3_row
                jmp LclipY_done
    LclipY_mode1_entry:
                xor ebp,ebp
    LclipY_mode1_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode1_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode1_nextRun
                bt ecx,0x0
                jae LclipY_mode1_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipY_mode1_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode1_nextRun
    LclipY_mode1_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipY_mode1_pair
    LclipY_mode1_nextRun:
                dec edx
                jne LclipY_mode1_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode1_row
                jmp LclipY_done
    LclipY_mode4_entry:
                xor ebp,ebp
    LclipY_mode4_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode4_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode4_nextRun
                bt ecx,0x0
                jae LclipY_mode4_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipY_mode4_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode4_nextRun
    LclipY_mode4_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipY_mode4_pair
    LclipY_mode4_nextRun:
                dec edx
                jne LclipY_mode4_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode4_row
                jmp LclipY_done
    LclipY_mode2_entry:
                xor ebp,ebp
    LclipY_mode2_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode2_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode2_nextRun
                bt ecx,0x0
                jae LclipY_mode2_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,word ptr d2_HighBitMask
                mov word ptr [edi],bp
                add edi,0x2
    LclipY_mode2_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode2_nextRun
    LclipY_mode2_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,dword ptr d2_HighBitMask32
                mov dword ptr [edi],ebp
                add edi,0x4
                dec ecx
                jne LclipY_mode2_pair
    LclipY_mode2_nextRun:
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
                dec edx
                jne LclipY_mode2_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode2_row
                jmp LclipY_done
    LclipY_mode5_entry:
                xor ebp,ebp
                mov ebp,dword ptr d2_Mode5Colour
    LclipY_mode5_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipY_mode5_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipY_mode5_nextRun
                bt ecx,0x0
                jae LclipY_mode5_oddDone
                mov word ptr [edi],bp
                add edi,0x2
    LclipY_mode5_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipY_mode5_nextRun
    LclipY_mode5_pair:
                mov dword ptr [edi],ebp
                add edi,0x4
                dec ecx
                jne LclipY_mode5_pair
    LclipY_mode5_nextRun:
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
                dec edx
                jne LclipY_mode5_run
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipY_mode5_row
                jmp LclipY_done
    LclipY_done:
                pop ebp
                pop ecx
                pop edx
                pop ebx
                pop eax
                pop esi
                pop edi
    }
}

#pragma optimize("", off)   /* the original has no alignment pad after this function: its section ends at the
                               last byte and the linker fills 0xcc; VC6 pads only under the default optimization, so a
                               pragma stood here (optimize("s", on) gives the same bytes) */
/* 0x47d210, draws a 16 bit shape, clipped left and right too (its dead variant blocks, je +0 dispatch, are in the asm verbatim) */
void d2_ShpBlit16ClipXY(int x, int y, void *bits, D2SHP *s)
{
#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit16%s %dx%d at %d,%d mode=%d surfpitch=%d\n", GetTickCount(), "ClipXY", s->w, s->h, x, y, s->mode, d2_SurfPitch));
#endif
    __asm {
                push edi
                push esi
                push eax
                push ebx
                push edx
                push ecx
                push ebp
                mov eax,dword ptr [ebp+0x8]
                mov edx,dword ptr [ebp+0xc]
                mov ebx,dword ptr [ebp+0x10]
                mov ecx,dword ptr [ebp+0x14]
                mov edi,ebx
                mov esi,ecx
                mov d2_ShpX,eax
                mov dword ptr d2_ShpY,edx
                mov d2_ShpRowX,eax
                shl eax,1
                xor ecx,ecx
                mov ebx,edx
                mov cx,word ptr [esi+0xa]
                add ebx,ecx
                cmp ebx,dword ptr d2_ClipTop
                jle LclipXY_done
                cmp edx,dword ptr d2_ClipBottom
                jge LclipXY_done
                push edx
                mov ebp,dword ptr d2_SurfPitch
                sub bp,word ptr [esi+0x6]
                shl ebp,1
                mov dword ptr d2_ShpRowGap,ebp
                xor ebx,ebx
                mov bx,word ptr [esi+0xa]
                mov dword ptr d2_ShpRows,ebx
                mov dword ptr d2_ShpDataOfs,0x32
                add edi,eax
                mov ebx,edx
                mov ecx,dword ptr d2_ClipTop
                sub ecx,ebx
                cmp ecx,0x0
                jle LclipXY_afterTop
                mov eax,esi
                add eax,dword ptr [esi+0x2a]
                mov edx,dword ptr [eax+ecx*4]
                mov dword ptr d2_ShpDataOfs,edx
                sub dword ptr d2_ShpRows,ecx
                mov ebx,dword ptr d2_ClipTop
    LclipXY_afterTop:
                mov ecx,dword ptr d2_ClipBottom
                pop edx
                mov eax,edx
                add ax,word ptr [esi+0xa]
                sub eax,ecx
                jle LclipXY_afterBottom
                sub dword ptr d2_ShpRows,eax
                cmp dword ptr d2_ShpRows,0x0
                jle LclipXY_done
    LclipXY_afterBottom:
                mov eax,d2_SurfPitch
                shl eax,1
                mul ebx
                add edi,eax
                mov ebx,dword ptr d2_ShpRows
                cmp ebx,0x0
                je LclipXY_done
                mov ax,word ptr [esi+0x6]
                mov d2_ShpW,ax
                mov ax,word ptr [esi+0xa]
                mov d2_ShpH,ax
                mov al,BYTE ptr [esi+0xd]
                add esi,dword ptr d2_ShpDataOfs
                cmp al,0x0
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x1
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x2
                je LclipXY_mode2_entry
                cmp al,0x3
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x4
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
                cmp al,0x5
        _emit 0x0f
        _emit 0x84
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
    LclipXY_mode0_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode0_run:
                add edi,dword ptr [esi]
                mov eax,dword ptr [esi]
                shr eax,1
                add dword ptr d2_ShpX,eax
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode0_nextRun
    LclipXY_mode0_pixel:
                mov eax,d2_ClipLeft
                cmp dword ptr d2_ShpX,eax
                jle LclipXY_mode0_pixelDone
                mov eax,d2_ClipRight
                cmp dword ptr d2_ShpX,eax
                jge LclipXY_mode0_pixelDone
                mov ax,word ptr [esi]
                mov word ptr [edi],ax
    LclipXY_mode0_pixelDone:
                add edi,0x2
                add esi,0x2
                add dword ptr d2_ShpX,0x1
                dec ecx
                jne LclipXY_mode0_pixel
    LclipXY_mode0_nextRun:
                dec edx
                jne LclipXY_mode0_run
                add edi,ebp
                mov eax,d2_ShpRowX
                mov d2_ShpX,eax
                dec ebx
                jne LclipXY_mode0_row
                jmp LclipXY_done
                xor ebp,ebp
                mov eax,dword ptr d2_ShpLightTable
    LclipXY_mode3_entry:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode3_row:
                add edi,dword ptr [esi]
                mov ebp,dword ptr [esi]
                shr ebp,1
                add dword ptr d2_ShpX,ebp
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode3_nextRun
    LclipXY_mode3_pixel:
                mov ebp,dword ptr d2_ClipLeft
                cmp dword ptr d2_ShpX,ebp
                jle LclipXY_mode3_pixelDone
                mov ebp,dword ptr d2_ClipRight
                cmp dword ptr d2_ShpX,ebp
                jge LclipXY_mode3_pixelDone
                mov bp,word ptr [edi]
                mov bp,word ptr [eax+ebp*2]
                mov word ptr [edi],bp
    LclipXY_mode3_pixelDone:
                add dword ptr d2_ShpX,0x1
                add edi,0x2
                dec ecx
                jne LclipXY_mode3_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    LclipXY_mode3_nextRun:
                dec edx
                jne LclipXY_mode3_row
                add edi,dword ptr d2_ShpRowGap
                mov ebp,dword ptr d2_ShpRowX
                mov dword ptr d2_ShpX,ebp
                dec ebx
                jne LclipXY_mode3_entry
                jmp LclipXY_done
                xor ebp,ebp
    LclipXY_mode1_entry:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode1_row:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode1_nextRun
                bt ecx,0x0
                jae LclipXY_mode1_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipXY_mode1_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipXY_mode1_nextRun
    LclipXY_mode1_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipXY_mode1_pair
    LclipXY_mode1_nextRun:
                dec edx
                jne LclipXY_mode1_row
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipXY_mode1_entry
                jmp LclipXY_done
                xor ebp,ebp
    LclipXY_mode4_entry:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode4_row:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode4_nextRun
                bt ecx,0x0
                jae LclipXY_mode4_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,0x3def
                add bp,word ptr [esi]
                mov word ptr [edi],bp
                add esi,0x2
                add edi,0x2
    LclipXY_mode4_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipXY_mode4_nextRun
    LclipXY_mode4_pair:
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,0x3defbdef
                add ebp,dword ptr [esi]
                mov dword ptr [edi],ebp
                add edi,0x4
                add esi,0x4
                dec ecx
                jne LclipXY_mode4_pair
    LclipXY_mode4_nextRun:
                dec edx
                jne LclipXY_mode4_row
                add edi,dword ptr d2_ShpRowGap
                dec ebx
                jne LclipXY_mode4_entry
                jmp LclipXY_done
    LclipXY_mode2_entry:
                xor ebp,ebp
    LclipXY_mode2_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode2_run:
                add edi,dword ptr [esi]
                mov ecx,dword ptr [esi+0x4]
                mov ebp,dword ptr [esi]
                shr ebp,1
                add dword ptr d2_ShpX,ebp
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode2_nextRun
                bt ecx,0x0
                jae LclipXY_mode2_oddDone
                mov bp,word ptr [edi]
                shr bp,1
                and bp,word ptr d2_HighBitMask
                mov word ptr [edi],bp
                add edi,0x2
    LclipXY_mode2_oddDone:
                shr ecx,1
                cmp ecx,0x0
                je LclipXY_mode2_nextRun
    LclipXY_mode2_pair:
                mov ebp,dword ptr d2_ClipLeft
                cmp dword ptr d2_ShpX,ebp
                jle LclipXY_mode2_pairDone
                mov ebp,dword ptr d2_ClipRight
                cmp dword ptr d2_ShpX,ebp
                jge LclipXY_mode2_pairDone
                mov ebp,dword ptr [edi]
                shr ebp,1
                and ebp,dword ptr d2_HighBitMask32
                mov dword ptr [edi],ebp
    LclipXY_mode2_pairDone:
                add edi,0x4
                add dword ptr d2_ShpX,0x1
                dec ecx
                jne LclipXY_mode2_pair
    LclipXY_mode2_nextRun:
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
                dec edx
                jne LclipXY_mode2_run
                add edi,dword ptr d2_ShpRowGap
                mov ebp,dword ptr d2_ShpRowX
                mov dword ptr d2_ShpX,ebp
                dec ebx
                jne LclipXY_mode2_row
                jmp LclipXY_done
                xor ebp,ebp
                mov eax,d2_Mode5Colour
    LclipXY_mode5_row:
                mov edx,dword ptr [esi]
                add esi,0x4
    LclipXY_mode5_run:
                add edi,dword ptr [esi]
                mov ebp,dword ptr [esi]
                shr ebp,1
                add dword ptr d2_ShpX,ebp
                mov ecx,dword ptr [esi+0x4]
                add esi,0x8
                cmp ecx,0x0
                je LclipXY_mode5_nextRun
    LclipXY_mode5_pixel:
                mov ebp,dword ptr d2_ClipLeft
                cmp dword ptr d2_ShpX,ebp
                jle LclipXY_mode5_pixelDone
                mov ebp,dword ptr d2_ClipRight
                cmp dword ptr d2_ShpX,ebp
                jge LclipXY_mode5_pixelDone
                mov word ptr [edi],ax
    LclipXY_mode5_pixelDone:
                add dword ptr d2_ShpX,0x1
                add edi,0x2
                dec ecx
                jne LclipXY_mode5_pixel
                mov cx,word ptr [esi-0x4]
                shl ecx,1
                add esi,ecx
    LclipXY_mode5_nextRun:
                dec edx
                jne LclipXY_mode5_run
                add edi,dword ptr d2_ShpRowGap
                mov ebp,dword ptr d2_ShpRowX
                mov dword ptr d2_ShpX,ebp
                dec ebx
                jne LclipXY_mode5_row
    LclipXY_done:
                pop ebp
                pop ecx
                pop edx
                pop ebx
                pop eax
                pop esi
                pop edi
    }
}
#pragma optimize("", on)

unsigned short *d2_ShpLightTable = 0;  /* 0x6f0460, colour table (own object: d2_ClipTop is a 4-byte int) */
unsigned int d2_LastAnimTick = 0;  /* 0x6f0464, tick of the last animation step */
int d2_ShpX = 0;  /* 0xf8a108 */
int d2_ShpY = 0;  /* 0xf8a10c */
int d2_ShpRowX = 0;  /* 0xf8a110 */
int d2_ShpRowGap = 0;  /* 0xf8a114 */
unsigned char d2_Palette[256][4] = {0};  /* 0xf8a120, palette */
D2ANIM d2_Anims[16] = {0};  /* 0xf8a520 */
int d2_LightTableExternal = 0;  /* 0xf8a630 */
unsigned int d2_Mode5Colour = 0;  /* 0xf8a638, colour twice, 16 bit */
unsigned int d2_BlitH = 0;  /* 0xf8a63c */
unsigned int d2_HighBitMask32 = 0;  /* 0xf8a640 */
unsigned char d2_BlitN = 0;  /* 0xf8a648 */
D2KEY d2_EndMarkerColour = { 0, 0xff00 };  /* 0x5fed74, blue (0, 0, 255) */
D2KEY d2_StartMarkerColour = { 0xff, 0 };  /* 0x5fed78, red (255, 0, 0) */
int d2_SurfPitch = 800;  /* 0x5fed84 */
