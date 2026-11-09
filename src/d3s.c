/* d3s.c -- original file name unknown: object shadows ("d3s:" allocation tags, texture "Schatten").
   0x459d80..0x45ce60: the integer edge set-up and span functions and a software triangle rasterizer for
   shadow textures, the shadow texture cache, the shadow system set-up / shutdown, the terrain shadow mesh, and
   the object shadows. The start at 0x459d80 follows the mesh code of d3_vtx.c (d3_GetBoxVerts ends at
   0x459d80); the end is proven by d3_sky.c, whose first D3DCHECK is at line 38 of that file.
   Functions are kept in their original order. Names are ours.
   Open: d3s.h's D3STERR / D3STBLOCK / D3SMESH are local views of D3TERRAIN / D3SMBLOCK (d3_sm.h) and D3MESH
   (d3_engine.h), which lack bs / mip and the float at 0x954; a union in D3MESH moves d3_Unk4550e0 by 8 bytes.
   unit of rea 8cef7d5b (src/game/d3s.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_d3d.h"
#include "d3_sm.h"
#include "d3s.h"
#include "data_views.h"
#include "d3_light.h"
#include "d3_engine.h"
#include "d3_vec.h"
#include "dd_vesa.h"
#include "mem.h"
#include "ts_texture.h"
#include "d3_pose.h"
extern unsigned char d3_MipSkip;

struct TSTEXTURE; struct TSTEXTURE *tex_Create(char *name, int size, char hicol, D3TEXFLAGS flags, unsigned char f0, unsigned char f6a,
                 int npal, unsigned char *pal, char mips);                              /* 0x461230 */

extern int d3s_EdgeX[4];
extern int d3s_EdgeY[4];
extern int d3s_EdgeC[3];
extern int d3s_SlopeL;
extern int d3s_SlopeR;
extern int d3s_SlopeC;
extern int d3s_XL;
extern int d3s_XR;
extern int d3s_CL;
extern int d3s_Area;
extern unsigned char * d3s_Dst;
extern D3SPANPIX * d3s_SpanRow;
extern int d3s_ShadowRadius;
extern char d3s_Byte;
extern unsigned short * d3s_Dst16;
extern int d3s_SpanLen;
extern int d3s_Fill;
extern int dd_PitchPixels;                      /* 0x14661a0 */
extern int dd_Pitch;                       /* 0x1463588 */
extern void *dd_Surface;                 /* 0x1463590 */
D3OBJECT * d3s_Object = 0;  /* 0x633448, object the shadow polygons go to */
unsigned int d3s_CacheCount = 0;  /* 0x633db8 */
unsigned int d3s_CacheAge = 0;  /* 0x633dbc */
extern D3SCENT * d3s_Cache;
unsigned int d3s_Count = 0;  /* 0x633dc0 */
unsigned int d3s_MaxPolys = 0;  /* 0x633dc4 */
int d3s_MaxPolys2 = 0;  /* 0x633dc8 */
extern int d3s_ShadowMapSize;              /* 0x633dcc, cells per side of the object shadow map */
extern const unsigned char d3s_Palette[];
extern float * d3s_Verts;
extern float * d3s_UVs;
extern char d3s_Enabled;
D3SCENT * d3s_Cache = 0;  /* 0x633dd0 */
char d3s_LodTexSize = 0;  /* 0xfb91d8, 1: the shadow texture size follows the mesh LOD */
extern unsigned char d3s_ShadowMips;
extern unsigned int d3s_TexSize;
unsigned int d3s_MaxTexSize = 0;  /* 0xfb91dc */
D3OBJECT *d3s_Lights[4] = {0};                    /* 0xfb91e0 */
char d3s_Enabled = 0;  /* 0xfb91f0 */
float d3s_QuadUV[6] = {0};  /* 0xfb9200 */
float * d3s_UVs = 0;  /* 0xfb9218 */
unsigned char d3s_ShadowMips = 0;  /* 0xfb921c, mip flag of the cached shadow textures */
int d3s_NumVerts = 0;  /* 0xfb9220 */
float d3s_QuadUV2[6] = {0};  /* 0xfb9240 */
extern D3OBJECT *d3s_Lights[4];             /* 0xfb91e0 */
extern unsigned int d3s_NumLights;          /* 0xfb9264 */
void d3sm_Destroy(struct D3SM *h);          /* 0x45ef60 */

/* float to int with fistp (current rounding mode) */
__inline void d3s_FtoI(int *dst, float f)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}

/* 64-bit fixed point helpers (inline asm): (a * b) >> 14, (a * b) >> 16 and (a << 16) / b */
__inline int d3s_MulShr14(int a, int b)
{
    __asm mov eax, a
    __asm mov edx, b
    __asm imul edx
    __asm shrd eax, edx, 14
}

__inline int d3s_MulShr16(int a, int b)
{
    __asm mov eax, a
    __asm mov edx, b
    __asm imul edx
    __asm shrd eax, edx, 16
}

__inline int d3s_ShlDiv16(int a, int b)
{
    __asm mov eax, a
    __asm mov ebx, b
    __asm mov edx, eax
    __asm sar edx, 16
    __asm shl eax, 16
    __asm idiv ebx
}
/* 0x459d80, x slope (d3s_SlopeR) and the x at the first pixel row of the edge from vertex a to vertex b */
void d3s_RightEdge(int a, int b)
{
    int dy = d3s_EdgeY[b] - d3s_EdgeY[a];

    if (dy < 0x10000)
        d3s_SlopeR = d3s_MulShr14(d3s_EdgeX[b] - d3s_EdgeX[a], 0x40000000 / dy);
    else
        d3s_SlopeR = d3s_ShlDiv16(d3s_EdgeX[b] - d3s_EdgeX[a], dy);
    d3s_XR = d3s_EdgeX[a]
        + d3s_MulShr16(((d3s_EdgeY[a] + 0xffff) & 0xffff0000) - d3s_EdgeY[a], d3s_SlopeR);
}
/* 0x459e50, as d3s_RightEdge with a second interpolated value (d3s_EdgeC, slope d3s_SlopeC, start d3s_CL) */

void d3s_LeftEdgeColor(int a, int b)
{
    int dy = d3s_EdgeY[b] - d3s_EdgeY[a];
    int q, d;

    if (dy < 0x10000) {
        q = 0x40000000 / dy;
        d3s_SlopeL = d3s_MulShr14(d3s_EdgeX[b] - d3s_EdgeX[a], q);
        d3s_SlopeC = d3s_MulShr14(d3s_EdgeC[b] - d3s_EdgeC[a], q);
    } else {
        d3s_SlopeL = d3s_ShlDiv16(d3s_EdgeX[b] - d3s_EdgeX[a], dy);
        d3s_SlopeC = d3s_ShlDiv16(d3s_EdgeC[b] - d3s_EdgeC[a], dy);
    }
    d = ((d3s_EdgeY[a] + 0xffff) & 0xffff0000) - d3s_EdgeY[a];
    d3s_XL = d3s_EdgeX[a] + d3s_MulShr16(d, d3s_SlopeL);
    d3s_CL = d3s_EdgeC[a] + d3s_MulShr16(d, d3s_SlopeC);
}
/* 0x459f90, writes v into the first dword of n consecutive 0x1c byte entries */

void d3s_MarkSpan(int v, int n, int *p)
{
    for (; n > 0; n--) {
        *p = v;
        p += 7;
    }
}

/* the span loop of d3s_DrawSpans: one byte per pixel from the 16.16 value u, which grows by step; the carry of the
   low half is carried into the next add (adc), the byte is the middle of the value (ror 16) */
__inline void d3s_Span(unsigned int u, int n, unsigned char *dst, int step)
{
    __asm mov eax, u
    __asm mov edx, n
    __asm mov ebx, dst
    __asm mov ecx, step
    __asm ror eax, 16
    __asm ror ecx, 16
    __asm clc
    __asm d3s_span_loop: mov [ebx], al
    __asm adc eax, ecx
    __asm inc ebx
    __asm dec edx
    __asm jne d3s_span_loop
}

/* 0x459fb0, steps down the rows of a triangle (rows of w pixels) along the edges d3s_XL / d3s_XR (left /
   right x with slopes d3s_SlopeL / d3s_SlopeR). Fills the byte buffer with the interpolated value (flags & 2) and
   marks the span (flags & 5: value 0xb for the border when flags & 1 is clear), widening it by d3s_ShadowRadius rows
   and pixels around.
   @deep B code hunks 91: the same operations in other registers / stack slots (span pointer in ebx vs memory, the hoisted neg of w from j * w, the [ebp-0x14..-0x24] homes swapped); no missing statement, constant or call - the 24 parameter orders and 16 loop-step orders were tried */
void d3s_DrawSpans(int rows, int w, unsigned char flags, char top, int y0)
{
    int x0, row, i, j, e, t, lim, span, yy, k;
    int fill2, fill1, lastx;
    D3SPANPIX *base;
    char drawn, marked;

    fill2 = 0;
    fill1 = 0;
    drawn = 0;
    marked = (flags & 5) != 0;
    if (marked && !(flags & 1))
        fill2 = fill1 = 0xb;
    lastx = -1;
    row = 0;
    if (rows > 0) {
        do {
            x0 = d3s_XL + 0xffff >> 16;
            if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x0) > 0) {
                base = d3s_SpanRow;
                lastx = x0;
                span = d3s_SpanLen;
                if (flags & 2)
                    d3s_Span(d3s_CL + d3s_MulShr16((x0 << 16) - d3s_XL, d3s_Area), d3s_SpanLen, d3s_Dst + x0, d3s_Area);
                if (marked) {
                    if ((k = d3s_ShadowRadius) > 0) {
                        d3s_MarkSpan(fill1, min(d3s_SpanLen + k * 2, lim = w - x0),
                                      (int *)(d3s_SpanRow + max(x0 - k, 0)));
                        if (top && !drawn) {
                            j = d3s_ShadowRadius;
                            if (j > 0) {
                                i = j * w;
                                e = row - j + y0;
                                do {
                                    if (e > 0) {
                                        t = d3s_ShadowRadius - j;
                                        d3s_MarkSpan(fill1, min(d3s_SpanLen + t * 2, lim),
                                                      (int *)(d3s_SpanRow + (max(x0 - t, 0) - i)));
                                    }
                                    j--;
                                    e++;
                                    i -= w;
                                } while (j > 0);
                            }
                            drawn = 1;
                        }
                    }
                    d3s_MarkSpan(fill2, d3s_SpanLen, (int *)(d3s_SpanRow + x0));
                }
            }
            d3s_XR += d3s_SlopeR;
            d3s_XL += d3s_SlopeL;
            d3s_CL += d3s_SlopeC;
            d3s_Dst += w;
            d3s_SpanRow += w;
            row++;
        } while (row < rows);
        if (lastx >= 0 && marked) {
            j = d3s_ShadowRadius;
            if (j > 0) {
                yy = j + rows + y0;
                do {
                    if (yy <= w) {
                        t = d3s_ShadowRadius - j;
                        d3s_MarkSpan(fill1, min(span + t * 2, w - lastx), (int *)(base + (max(lastx - t, 0) + j * w)));
                    }
                    yy--;
                    j--;
                } while (j > 0);
            }
        }
    }
}
/* 0x45a2e0, a shadow triangle: edges from the three points p (x, y pairs), colours c, radius pixels / rows around
   @deep Read in full against 0x45a2e0..0x45a66a: the orientation test, both edge loops (reversed with the
   p+4 / p+5 / c+2 walking pointers, forward indexed), the flag masks, the top-vertex edge choice (e / b /
   d / l over d3_TriEdge), the colour gradient d3s_Area (65536 / area * (dc0 * (p5 - p3) - dc2 * (p1 -
   p3)), inline FtoI), both span passes with their left / right edges; every statement, call and argument
   agrees (ours 0x169 ins vs the original's, size 906 vs 905). Left: allocation - in the edge choice the
   original keeps d in [esp+0x1c], b in ebx, e in esi and l in ecx ('+ mov ebx,[eax*8+TriEdge+4] / mov
   [esp+0x1c],esi / mov esi,[esi*8+TriEdge] / mov ecx,eax'), ours keeps b in the slot and d in esi, which
   renames every later EdgeY index and the d3s_DrawSpans / d3s_RightEdge pushes (+0x15b..+0x37e); the
   orientation pointers in ebx vs ours edx; reordering the declarations does not move it. */

void d3s_ShadowTriangle(float *p, unsigned char *c, unsigned char *dst, int pitch, unsigned char flags, char *base, int radius)
{
    int i, t, b, d, e, y, n, l;
    int minY;
    int left;
    float area;

    minY = pitch << 16;
    i = 0;
    t = -1;
    if (p[0] * p[5] + ((p[2] * p[1] + (p[3] * p[4] - p[2] * p[5])) - p[0] * p[3]) - p[4] * p[1] > 0.0) {
        do {
            d3s_EdgeX[i] = (int)(p[4 - i * 2] * 65536.0f);
            d3s_EdgeY[i] = (int)(p[5 - i * 2] * 65536.0f);
            d3s_EdgeC[i] = c[2 - i] << 16;
            if (d3s_EdgeY[i] <= minY) {
                minY = d3s_EdgeY[i];
                t = i;
            }
            i++;
        } while (i < 3);
    } else {
        do {
            d3s_EdgeX[i] = (int)(p[i * 2] * 65536.0f);
            d3s_EdgeY[i] = (int)(p[i * 2 + 1] * 65536.0f);
            d3s_EdgeC[i] = c[i] << 16;
            if (d3s_EdgeY[i] <= minY) {
                minY = d3s_EdgeY[i];
                t = i;
            }
            i++;
        } while (i < 3);
    }
    if (t != -1) {
        if (base == 0)
            flags &= 2;
        if (dst == 0)
            flags &= 5;
        if (flags != 0) {
            e = d3_TriEdge[t][0];
            if (d3s_EdgeY[e] == minY) {
                b = d3_TriEdge[t][1];
                d = e;
                e = d3_TriEdge[e][0];
                l = t;
            } else {
                l = d3_TriEdge[t][1];
                if (d3s_EdgeY[l] == minY) {
                    b = d3_TriEdge[l][1];
                    d = t;
                } else {
                    b = l;
                    d = t;
                    l = t;
                }
            }
            area = (p[0] - p[2]) * (p[5] - p[3]) - (p[4] - p[2]) * (p[1] - p[3]);
            if (area == 0.0f)
                d3s_Area = 0;
            else
                d3s_FtoI(&d3s_Area, 65536.0f / area * ((float)(c[0] - c[1]) * (p[5] - p[3]) - (float)(c[2] - c[1]) * (p[1] - p[3])));
            if (d3s_EdgeY[b] - d3s_EdgeY[l] > 0) {
                d3s_LeftEdgeColor(l, b);
                if (d3s_EdgeY[e] - d3s_EdgeY[d] > 0) {
                    d3s_RightEdge(d, e);
                    y = minY + 0xffff >> 16;
                    d3s_Dst = dst + y * pitch;
                    d3s_SpanRow = (D3SPANPIX *)(base + y * pitch * 0x1c);
                    d3s_ShadowRadius = radius;
                    left = d3s_EdgeY[b] < d3s_EdgeY[e];
                    n = ((left ? d3s_EdgeY[b] : d3s_EdgeY[e]) + 0xffff >> 16) - y;
                    d3s_DrawSpans(n, pitch, flags, 1, y);
                    if (d3s_EdgeY[b] != d3s_EdgeY[e]) {
                        if (left) {
                            d3s_LeftEdgeColor(b, e);
                            i = (d3s_EdgeY[e] + 0xffff >> 16) - (d3s_EdgeY[b] + 0xffff >> 16);
                        } else {
                            d3s_RightEdge(e, b);
                            i = (d3s_EdgeY[b] + 0xffff >> 16) - (d3s_EdgeY[e] + 0xffff >> 16);
                        }
                        d3s_DrawSpans(i, pitch, flags, 0, y + n);
                    }
                }
            }
        }
    }
}
/* 0x45a670, the left edge slope and start from vertex a to b (16.16) */

void d3s_LeftEdge(int a, int b)
{
    int dy = d3s_EdgeY[b] - d3s_EdgeY[a];

    if (dy < 0x10000)
        d3s_SlopeL = d3s_MulShr14(d3s_EdgeX[b] - d3s_EdgeX[a], 0x40000000 / dy);
    else
        d3s_SlopeL = d3s_ShlDiv16(d3s_EdgeX[b] - d3s_EdgeX[a], dy);
    d3s_XL = d3s_EdgeX[a] + d3s_MulShr16((d3s_EdgeY[a] + 0xffff & 0xffff0000) - d3s_EdgeY[a], d3s_SlopeL);
}
/* 0x45a740, fills n scanlines between d3s_XL and d3s_XR with d3s_Fill */

void d3s_FillRows(int n)
{
    int x, i;
    char *p;

    if (d3s_Byte) {
        for (i = 0; i < n; i++) {
            x = d3s_XL + 0xffff >> 16;
            if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x) > 0) {
                p = (char *)(d3s_Dst + x);
                __asm mov edi, p
                __asm mov ecx, d3s_SpanLen
                __asm mov eax, d3s_Fill
                __asm rep stosb
            }
            d3s_XL += d3s_SlopeL;
            d3s_XR += d3s_SlopeR;
            d3s_Dst += dd_Pitch;
        }
    } else {
        for (i = 0; i < n; i++) {
            x = d3s_XL + 0xffff >> 16;
            if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x) > 0) {
                p = (char *)(d3s_Dst16 + x);
                __asm mov edi, p
                __asm mov ecx, d3s_SpanLen
                __asm mov eax, d3s_Fill
                __asm rep stosw
            }
            d3s_XL += d3s_SlopeL;
            d3s_XR += d3s_SlopeR;
            d3s_Dst16 += dd_PitchPixels;
        }
    }
}
/* 0x45a880, draws one polygon of the shadow mesh into the shadow texture (bpp bits per pixel, its low byte)
   @deep Read whole against the original; fixed bpp<=8 (setbe), the ternary with +0xffff in both arms, bpp
   as int param (whole slot is the first top index; d3s_Project calls via @original char cast macro).
   Prover: every store/call/branch equal, only eax at the void return differs. Left: allocation - original
   keeps lim in [esp+0x10] and the min y in ebp with lim-1 hoisted to edx before the bounds loop (ours lim
   in ebp, lea edx,[ebp-1] in the loop, reload of [esp+0x20] at +0x13e); shl of size scheduled first; imul
   eax,[dd_Pitch] memory-operand forms at +0x1dc/+0x1fe; the two FillRows tails pop add esp,8 then add
   esp,4 where ours merges add esp,0xc. */

void d3s_ShadowPoly(D3SPOLY *poly, int bpp, int size)
{
    D3SVERT *v0, *v1, *v2;
    int i, top, lim;
    int a, b, c, y, n;
    int left;

    lim = size << 16;
    v0 = poly->v[0];
    v1 = poly->v[1];
    v2 = poly->v[2];
    size = lim;
    if ((v0->tu - v1->tu) * (v0->tv - v2->tv) < (v0->tu - v2->tu) * (v0->tv - v1->tv)) {
        if (!(poly->flags & 4))
            return;
        top = bpp;
        for (i = 0; i < 3; i++) {
            d3s_EdgeX[i] = (int)(poly->v[2 - i]->tu * 65536.0f);
            d3s_EdgeY[i] = (int)(poly->v[2 - i]->tv * 65536.0f);
            if (d3s_EdgeY[i] <= size) {
                size = d3s_EdgeY[i];
                top = i;
            }
        }
    } else {
        top = bpp;
        for (i = 0; i < 3; i++) {
            d3s_EdgeX[i] = (int)(poly->v[i]->tu * 65536.0f);
            d3s_EdgeY[i] = (int)(poly->v[i]->tv * 65536.0f);
            if (d3s_EdgeY[i] <= size) {
                size = d3s_EdgeY[i];
                top = i;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (d3s_EdgeX[i] < 0 || d3s_EdgeX[i] > lim - 1 || d3s_EdgeY[i] < 0 || d3s_EdgeY[i] > lim - 1)
            return;
    }
    a = d3_TriEdge[top][0];
    if (d3s_EdgeY[a] == size) {
        b = d3_TriEdge[top][1];
        c = top;
        top = a;
        a = d3_TriEdge[top][0];
    } else {
        c = d3_TriEdge[top][1];
        if (d3s_EdgeY[c] == size) {
            b = d3_TriEdge[c][1];
        } else {
            b = c;
            c = top;
        }
    }
    if (d3s_EdgeY[b] - d3s_EdgeY[c] > 0) {
        d3s_LeftEdge(c, b);
        if (d3s_EdgeY[a] - d3s_EdgeY[top] > 0) {
            d3s_RightEdge(top, a);
            d3s_Byte = (unsigned char)bpp <= 8;
            y = size + 0xffff >> 16;
            if (d3s_Byte) {
                d3s_Dst = (unsigned char *)dd_Surface + y * dd_Pitch;
                d3s_Fill = 1;
            } else {
                d3s_Fill = 0xffff;
                d3s_Dst16 = (unsigned short *)dd_Surface + y * dd_PitchPixels;
            }
            left = d3s_EdgeY[b] < d3s_EdgeY[a];
            n = (left ? d3s_EdgeY[b] + 0xffff : d3s_EdgeY[a] + 0xffff) >> 16;
            d3s_FillRows(n - y);
            if (d3s_EdgeY[b] != d3s_EdgeY[a]) {
                if (left) {
                    d3s_LeftEdge(b, a);
                    d3s_FillRows((d3s_EdgeY[a] + 0xffff >> 16) - (d3s_EdgeY[b] + 0xffff >> 16));
                } else {
                    d3s_RightEdge(a, b);
                    d3s_FillRows((d3s_EdgeY[b] + 0xffff >> 16) - (d3s_EdgeY[a] + 0xffff >> 16));
                }
            }
        }
    }
}
/* 0x45ab60, allocates n shadow texture cache entries */

void d3s_CacheInit(int n, int age)
{
    d3s_CacheCount = n;
    d3s_CacheAge = age;
    d3s_Cache = mem_Alloc(n * 0x14, "d3s:Cache");
}
/* 0x45ab90, frees the shadow texture cache */

void d3s_CacheFree(void)
{
    d3s_CacheAge = 0;
    d3s_CacheCount = 0;
    mem_Free(d3s_Cache);
    d3s_Cache = 0;
}
/* 0x45abc0, gives the cache entry c to slot, (re)creating its texture if size or flag differ */

D3TEXHANDLE *d3s_CacheBind(D3SCENT *c, D3SSLOT *slot, char flag, int size, unsigned int time)
{
    D3TEXFLAGS f;

    c->time = time;
    slot->cache = c;
    if (c->size > 0 && c->owner)
        *c->owner = 0;
    c->owner = (void **)slot;
    if (c->size != size || c->flag != flag) {
        if (c->tex)
            tex_Release(c->tex);
        c->size = size;
        c->flag = flag;
        memset(&f, 0, sizeof(f));
        f.b[0] = 0x7f;
        f.b[2] |= 2;
        c->tex = (D3TEXHANDLE *)tex_Create("*shadow", size, 1, f, 0, 0, 2, d3s_Palette, flag);
    }
    return c->tex;
}
/* 0x45ac60, a texture of size for slot: a free entry, the oldest of the same kind, or any other of the flag */

D3TEXHANDLE *d3s_CacheTexture(D3SSLOT *slot, char flag, int size, unsigned int time, D3SCENT **unused)
{
    D3SCENT *c, *best;
    unsigned int i, oldest;

    best = 0;
    oldest = time;
    for (i = 0, c = d3s_Cache; i < d3s_CacheCount; i++, c++) {
        if (c->size == size && c->flag == flag && c->time < oldest) {
            oldest = c->time;
            best = c;
        }
        if (c->size == 0)
            return d3s_CacheBind(c, slot, flag, size, time);
    }
    if (best && time - oldest > d3s_CacheAge)
        return d3s_CacheBind(best, slot, flag, size, time);
    best = 0;
    oldest = time;
    for (i = 0, c = d3s_Cache; i < d3s_CacheCount; i++, c++) {
        if (c->size > 0 && c->flag == flag && c->time < oldest) {
            oldest = c->time;
            best = c;
        }
    }
    if (best && time - oldest > 1)
        return d3s_CacheBind(best, slot, flag, size, time);
    return 0;
}
/* 0x45ad80, sets up the shadow system (texture sizes, poly buffers, cache, lights) */

void d3_InitShadows(unsigned int size, unsigned int maxsize, int ncache, int age, char mips, int polys, int mapsize)
{
    if (maxsize >= d3d_UsedCard.info.maxw)
        maxsize = d3d_UsedCard.info.maxw;
    d3s_MaxTexSize = maxsize;
    d3s_TexSize = size;
    if (size >= maxsize)
        d3s_TexSize = maxsize;
    d3s_MaxPolys2 = polys;
    d3s_MaxPolys = polys * polys;
    d3s_ShadowMapSize = mapsize;
    d3s_UVs = mem_Alloc(d3s_MaxPolys * 0x30, "d3s:BuildingPolyBuffer(Shadow)");
    d3s_NumUVs = 0;
    d3s_Verts = mem_Alloc((d3s_MaxPolys >> 4) * 600, "d3s:BuildingPointBuffer(Shadow)");
    d3s_Enabled = 1;
    d3s_LodTexSize = 1;
    d3s_NumVerts = 0;
    d3s_QuadUV[0] = 0.0f;
    d3s_QuadUV[1] = 1.0f;
    d3s_QuadUV[2] = 1.0f;
    d3s_QuadUV[3] = 1.0f;
    d3s_QuadUV[4] = 0.0f;
    d3s_QuadUV[5] = 0.0f;
    d3s_QuadUV2[0] = 1.0f;
    d3s_QuadUV2[1] = 1.0f;
    d3s_QuadUV2[2] = 1.0f;
    d3s_QuadUV2[3] = 0.0f;
    d3s_QuadUV2[4] = 0.0f;
    d3s_QuadUV2[5] = 0.0f;
    d3s_ShadowMips = mips;
    d3s_CacheInit(ncache, age);
    d3s_NumLights = 0;
    memset(d3s_Lights, 0, 16);
}
/* 0x45aee0, shuts the shadow system down */

void d3_ExitShadows(void)
{
    d3s_MaxTexSize = 0;
    d3s_TexSize = 0;
    d3s_MaxPolys2 = 0;
    d3s_MaxPolys = 0;
    d3s_NumUVs = 0;
    mem_Free(d3s_UVs);
    d3s_UVs = 0;
    d3s_NumVerts = 0;
    mem_Free(d3s_Verts);
    d3s_Verts = 0;
    d3s_CacheFree();
    if (d3sm_Shadow) {
        d3sm_Destroy(d3sm_Shadow);
        d3sm_Shadow = 0;
    }
}
/* 0x45af50, the shadow mesh over the part rect of a terrain with 8x8 blocks, each block sampled at its own lod @deep B code hunks 738 with the frame 0xa4 here vs 0xa0: the prologue and loops read out as the same loads / FP maths on other stack slots (every local of the original's 0xa0 slot map in another home, the hoisted fstep*uv / fstep*cx / fstep*cz products and fx0 / oy+1.0 in other slots); no missing statement or call found in the hunks read; the original's slot map (rect in ebp, buf in ebx, uv reloaded from memory, the hoisted products in float slots) is what a deep round needs */

char d3s_TerrainShadowMesh(D3STERR *g, int *rect, float *uv, D3SBUF *buf, D3TEXHANDLE *tex, unsigned long color)
{
    D3VECTOR4 o, p;
    float d[4];
    float cx, cy, cz;
    unsigned char *heights, *mip, *mp;
    unsigned int stride, step, rowskip;
    int x, z, zend, xend, xi, zi, k, r, nrows, ncx;
    int bz, bz1, bx, bx0, bx1, nz;
    float vcur, ucur, fstep, cxs, czs, fxc, vtop, vbot, ul, ur;
    float *pos, *uvp;
    D3SVERT *vert, *vp, *tr, *bl, *br, *trow, *brow;
    D3SPOLY *poly;
    char drew;
    unsigned char lod;

    o.x = g->origin.x;
    o.y = g->origin.y;
    o.z = g->origin.z;
    cx = g->axisx.x;
    cy = g->axisy.y;
    heights = g->heights;
    cz = g->axisz.z;
    stride = g->size;
    bz1 = rect[3] / (int)g->bs;
    drew = 0;
    bx1 = rect[1] / (int)g->bs;
    bx0 = rect[0] / (int)g->bs;
    vcur = uv[2];
    z = rect[2];
    for (bz = z / (int)g->bs, nz = bz + 1; bz <= bz1; bz++, nz++) {
        if (bz == bz1)
            zend = rect[3];
        else
            zend = nz * g->bs;
        x = rect[0];
        ucur = uv[0];
        for (bx = bx0; bx <= bx1; ) {
            if (bz >= 0 && bz < 8 && bx >= 0 && bx < 8 && (lod = g->blocks[bz][bx].lod) > 0) {
                step = lod;
                rowskip = stride / step;
                fstep = (float)step;
                drew = 1;
                mip = g->mip[step >> 1];
                d[1] = fstep * uv[1];
                d[3] = fstep * uv[3];
                cxs = cx * fstep;
                czs = fstep * cz;
                if (bx == bx1)
                    xend = rect[1];
                else
                    xend = (bx + 1) * g->bs;
                p.x = (float)x * cx + o.x;
                poly = buf->polys + buf->npolys;
                pos = d3s_Verts + d3s_NumVerts * 6;
                p.y = o.y + 1.0f;
                vert = buf->verts + buf->nverts;
                uvp = d3s_UVs + d3s_NumUVs * 6;
                p.z = (float)z * cz + o.z;
                vp = vert;
                for (zi = z; zi <= zend; zi += step) {
                    for (xi = x, fxc = p.x; xi <= xend; xi += step) {
                        pos[0] = fxc;
                        pos[1] = (float)heights[zi * stride + xi] * cy + p.y;
                        pos[2] = p.z;
                        vp->c1 = color;
                        vp->pos = pos;
                        pos += 6;
                        vp++;
                        d3s_NumVerts++;
                        buf->nverts++;
                        fxc += cxs;
                    }
                    p.z += czs;
                }
                nrows = (zend - z) / step;
                vtop = vcur;
                ncx = (xend - x) / step;
                mp = mip + x / step + z / step * rowskip;
                for (r = 0, trow = vert + 1, brow = vert + (ncx + 1); r < nrows; r++) {
                    ul = ucur;
                    tr = trow;
                    bl = brow;
                    for (k = 0; k < ncx; k++) {
                        vbot = d[3] + vtop;
                        br = bl + 1;
                        poly->v[2] = bl;
                        if (mp[k] & 0x80) {
                            poly->v[1] = tr;
                            uvp[3] = vtop;
                            ur = ul + d[1];
                            poly[1].v[1] = br;
                            poly[1].v[0] = tr;
                            poly[1].v[2] = bl;
                            uvp[10] = ul;
                            uvp[6] = ur;
                            uvp[9] = vbot;
                        } else {
                            poly->v[1] = br;
                            uvp[3] = vbot;
                            poly[1].v[0] = tr - 1;
                            poly[1].v[1] = tr;
                            poly[1].v[2] = br;
                            uvp[9] = vtop;
                            ur = ul + d[1];
                            uvp[6] = ul;
                            uvp[10] = ur;
                        }
                        poly[1].flags &= 0xfd;
                        poly->flags &= 0xfd;
                        poly[1].uv = uvp + 6;
                        poly[1].z = -1.0f;
                        poly->z = -1.0f;
                        poly->v[0] = tr - 1;
                        poly->uv = uvp;
                        poly[1].tex = tex;
                        poly->tex = tex;
                        uvp[1] = vtop;
                        poly += 2;
                        uvp[2] = ur;
                        uvp[0] = ul;
                        uvp[4] = ul;
                        uvp[5] = vbot;
                        bl++;
                        uvp[7] = vtop;
                        tr++;
                        uvp[8] = ur;
                        uvp[11] = vbot;
                        d3s_NumUVs += 2;
                        uvp += 12;
                        buf->npolys += 2;
                        ul = ur;
                    }
                    mp += rowskip;
                    vtop += d[3];
                    brow += ncx + 1;
                    trow += ncx + 1;
                }
            }
            bx++;
            ucur = (float)(bx * (int)g->bs - x) * uv[1] + ucur;
            x = (x + (int)g->bs) / (int)g->bs * g->bs;
        }
        vcur = (float)(nz * (int)g->bs - z) * uv[3] + vcur;
        z = (z + (int)g->bs) / (int)g->bs * g->bs;
    }
    return drew;
}

typedef struct TSTEXSIZE {          /* the parts of a texture slot (ts_texture.h: TSTEXTURE) used here */
    char pad00[0x74];
    unsigned int size;              /* 0x74 */
    char pad78[0x4];
    unsigned char bpp;              /* 0x7c */
} TSTEXSIZE;

#define d3s_ShadowPolyB ((void (*)(D3SPOLY *, unsigned char, int))d3s_ShadowPoly)  /* @original d3s_Project pushes the byte bpp unextended; d3s_ShadowPoly reads the whole slot as its first top index */

void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);                          /* 0x458810 */
unsigned char d3_ClipBox(void *box, float *zmin, float *zmax);                                           /* 0x439650 */
char dd_FillSurface(LPDIRECTDRAWSURFACE4 surf, RECT *rect, DWORD color);                         /* 0x41e0a0 */
void * dd_LockSurfaceFlags(LPDIRECTDRAWSURFACE4 surf, DWORD flags);                               /* 0x41de30 */
void dd_UnlockSurface(LPDIRECTDRAWSURFACE4 surf);                                       /* 0x41e740 */
void d3d_BuildMipmaps(LPDIRECTDRAWSURFACE4 tex, DDSURFACEDESC2 *ddsd);                                /* 0x4371d0 */
struct TSTEXTURE; void ts_CacheFetch(struct TSTEXTURE *tex, int npal, unsigned char *pal);                                     /* 0x45f8d0 */

const float d3s_Dist[2] = { 60.0f, 200.0f };  /* 0x5ebbd8 */
const unsigned char d3s_Palette[] = { 0, 0, 0, 0xff, 0xff, 0xff, 0, 0 };  /* 0x5ebbe0, palette */
extern int d3_FrameCount;          /* 0x63343c */
extern unsigned char d3_Hardware;                    /* 0x633450 */
extern const D3VECTOR d3_ZAxis;             /* 0x5ebb74 */

float * d3s_Verts = 0;  /* 0xfb9258, 6 floats each */
int d3s_NumUVs = 0;  /* 0xfb925c */
unsigned int d3s_TexSize = 0;  /* 0xfb9260 */
unsigned int d3s_NumLights = 0;                 /* 0xfb9264 */
char d3s_Byte = 0;  /* 0xfb9268 */
int d3s_ShadowRadius = 0;  /* 0xfb926c, rows / pixels the marked span widens around the shadow */
D3SPANPIX * d3s_SpanRow = 0;  /* 0xfb9270, current row of the span buffer */
unsigned char * d3s_Dst = 0;  /* 0xfc2940 */
int d3s_EdgeC[3] = {0};  /* 0xfc29b0 */
int d3s_SlopeC = 0;  /* 0xfc2dd0, slope of the interpolated value along the left edge */
int d3s_SlopeL = 0;  /* 0xfc2de4 */
int d3s_CL = 0;  /* 0xfc2de8, interpolated value at the left edge */
int d3s_SlopeR = 0;  /* 0xfc2df8 */
int d3s_EdgeX[4] = {0};  /* 0xfc50d0 */
int d3s_EdgeY[4] = {0};  /* 0xfc50e0 */
int d3s_XL = 0;  /* 0xfc5334 */
int d3s_XR = 0;  /* 0xfc533c */
int d3s_SpanLen = 0;  /* 0xfe73a0 */
unsigned short * d3s_Dst16 = 0;  /* 0xfe73b0 */
int d3s_Area = 0;  /* 0xfe73b8 */
int d3s_Fill = 0;  /* 0xfe75ac */

/* 0x45b510, a shadow mesh over the map cells in rect @deep whole function compared against the original and reconciled: corrected here were the height reads of the second loop (row[i + nx + 1].pos[1], through the vertex's pos pointer, instead of the never-set .y field: 4 reads), the hoisted loop invariants xs = x0 * cell[0] + origin[0] and ys = add + origin[1] (stored to [esp+0x24] / [esp+0x28] in the original, reloaded per row), and steep as an int (xor eax,eax / mov eax,1 instead of the byte forms); instruction count 289 vs 297. Remaining: allocation only - the original keeps the polygon pointer in esi and the uv pointer in ecx (ours the reverse), x0 / z0 / nx / nz in slots [0x10] [0x18] [0x1c] [0x20] against ours [0x14] [0x1c] [0x18] [0x10] (slots hunks 0x45b519..0x45b86e), and a few reloads of rect / cell / origin through the stack (the 66 / 61 / 169 / 87 / 48 'code' hunks are the same statements with these registers) */
char d3s_MeshFromMap(int *rect, int stride, unsigned char *map, float *origin, float *cell, float *uv,
                     D3SBUF *buf, D3TEXHANDLE *tex, unsigned long color)
{
    D3SVERT *v, *row;
    D3SPOLY *p;
    float *pos, *t;
    float add, xs, ys, x, z, u, w, a, b, c;
    int nx, nz, i, k, n, x0, z0;
    int steep;

    x0 = rect[0];
    nx = rect[1] - x0;
    z0 = rect[2];
    nz = rect[3] - z0;
    row = &buf->verts[buf->nverts];
    pos = &d3s_Verts[d3s_NumVerts * 6];
    t = &d3s_UVs[d3s_NumUVs * 6];
    add = d3d_ZEnabled ? 2.0f : 1.0f;
    xs = x0 * cell[0] + origin[0];
    ys = add + origin[1];
    z = z0 * cell[2] + origin[2];
    v = row;
    for (k = z0, map += z0 * stride; k <= rect[3]; k++, map += stride) {
        x = xs;
        for (i = rect[0]; i <= rect[1]; i++, v++) {
            pos[1] = (map[i] + add) * cell[1] + ys;
            pos[0] = x;
            pos[2] = z;
            v->c1 = color;
            v->pos = pos;
            pos += 6;
            d3s_NumVerts++;
            buf->nverts++;
            x += cell[0];
        }
        z += cell[2];
    }
    w = uv[2];
    p = &buf->polys[buf->npolys];
    for (k = nz; k > 0; k--, row += nx + 1) {
        u = uv[0];
        for (i = 0; i < nx; i++) {
            p->v[0] = &row[i];
            a = row[i + nx + 1].pos[1];
            p->v[1] = &row[i + 1];
            b = row[i + 1].pos[1];
            p->v[2] = &row[i + nx + 1];
            c = row[i + nx + 1].pos[1];
            p->uv = t;
            p->tex = tex;
            p->z = -1.0f;
            steep = fabs(a - b) > 25.0f || fabs(a - c) > 25.0f ? 1 : 0;
            p->flags = (steep << 1 ^ p->flags) & 2 ^ p->flags;
            t[0] = u;
            t[1] = w;
            t[2] = u + uv[1];
            t[3] = w;
            t[4] = u;
            t[5] = w + uv[3];
            d3s_NumUVs++;
            buf->npolys++;
            p++;
            p->v[0] = &row[i + 1];
            p->v[1] = &row[i + nx + 2];
            p->v[2] = &row[i + nx + 1];
            p->uv = t + 6;
            p->tex = tex;
            p->z = -1.0f;
            a = row[i + nx + 2].pos[1];
            steep = fabs(a - b) > 25.0f || fabs(a - c) > 25.0f ? 1 : 0;
            p->flags = (steep << 1 ^ p->flags) & 2 ^ p->flags;
            t[6] = u + uv[1];
            t[7] = w;
            t[8] = u + uv[1];
            t[9] = w + uv[3];
            t[10] = u;
            t[11] = w + uv[3];
            d3s_NumUVs++;
            buf->npolys++;
            p++;
            t += 12;
            u += uv[1];
        }
        w += uv[3];
    }
    return 1;
}
/* 0x45b8d0, adds the shadow of slot to the shadow object @deep match-base 2026-10-31: read the whole function against the original and rebuilt it (296 -> 327 vs 327 instructions): one if (d3s_Enabled && map && size && !simple) / else quad, range check on the slot fields first and the stores after, v0 = 0.0f first (kept on the FPU), du / dv / u0 / v0 divided by (float)(x1 - x0) / (z1 - z0) (no reciprocal CSE), nested size ifs falling into the final return ok (ok is a memory char), uv[4] / rect[4] copies, grid->bs signed division, both mesh calls store ok, the quad walks p++ / v++ / pos += 6 with pos computed before the poly stores, light colour / power through D3LOBJ. Left: register rotation ecx/edi and ebx/edi over the whole mesh part (regs / slots hunks; the h*2*w product starts from the other register), the size-fail return ok block (fstp st(0)) is placed at the end instead of after the call epilogues, ok = 1 is scheduled early, and the quad copies the slot floats with mov where the original uses fld/fstp (no free integer register in the original). Tried + 0.0f / * 1.0f / (float) on the copies (folded, no change), early return ok instead of nested ifs (ok folds to xor al,al, worse), fw/fh float locals (reciprocal CSE, worse). */

char d3s_AddShadow(D3SSLOT *slot, D3STERR *grid)
{
    D3SBUF *buf;
    D3SPOLY *p;
    D3SVERT *v;
    float *pos;
    float u0, v0, du, dv, fw, fh;
    float uv[4];
    int rect[4];
    int x0, x1, z0, z1, w, h, n;
    union { unsigned long d; D3COLOR c; } col;
    char ok = 0;
    D3TEXHANDLE *tex;

    buf = (D3SBUF *)&D3_MESH(d3s_Object)->surf[0].info;
    tex = slot->tex;
    col.c.r = (unsigned char)D3_L(slot->light)->color.x;
    col.c.g = (unsigned char)D3_L(slot->light)->color.y;
    col.c.b = (unsigned char)D3_L(slot->light)->color.z;
    col.c.a = (unsigned char)(D3_L(slot->light)->power * 0.5f);
    if (d3s_Enabled && slot->map && slot->size && !slot->simple) {
        if (slot->ix0 >= slot->size || slot->ix1 < 0 || slot->iz0 >= slot->size || slot->iz1 < 0
            || slot->ix1 - slot->ix0 <= 0 || slot->iz1 - slot->iz0 <= 0)
            return 0;
        x0 = slot->ix0;
        x1 = slot->ix1;
        z0 = slot->iz0;
        z1 = slot->iz1;
        v0 = 0.0f;
        du = 1.0f / (x1 - x0);
        u0 = 0.0f;
        dv = 1.0f / (z1 - z0);
        if (x0 < 0) {
            u0 = -x0 / (float)(x1 - x0);
            x0 = 0;
        }
        if (x1 >= slot->size)
            x1 = slot->size - 1;
        if (z0 < 0) {
            v0 = -z0 / (float)(z1 - z0);
            z0 = 0;
        }
        if (z1 >= slot->size)
            z1 = slot->size - 1;
        h = z1 - z0;
        w = x1 - x0;
        if (h * 2 * w + buf->npolys < d3s_MaxPolys * 2) {
            n = (h + 1) * (w + 1);
            if (buf->nverts + n < (d3s_MaxPolys >> 4) * 25) {
                uv[0] = u0;
                uv[1] = du;
                uv[2] = v0;
                uv[3] = dv;
                rect[0] = x0;
                rect[1] = x1;
                rect[2] = z0;
                rect[3] = z1;
                if (grid) {
                    if (buf->nverts + (w / (int)grid->bs + 1) * h + (h / (int)grid->bs + 1) * w + n
                        < (d3s_MaxPolys >> 4) * 25)
                        ok = d3s_TerrainShadowMesh(grid, rect, uv, buf, tex, col.d);
                } else
                    ok = d3s_MeshFromMap(rect, slot->size, slot->map, &slot->origin.x, &slot->cell.x, uv, buf,
                                         tex, col.d);
            }
        }
    } else if (buf->npolys < d3s_MaxPolys * 2 - 2) {
        p = &buf->polys[buf->npolys];
        v = &buf->verts[buf->nverts];
        pos = &d3s_Verts[d3s_NumVerts * 6];
        p->v[0] = v;
        p->uv = d3s_QuadUV;
        p->v[2] = v + 2;
        p->v[1] = v + 1;
        p->tex = tex;
        p++;
        p->v[1] = v + 3;
        p->v[2] = v + 2;
        p->v[0] = v + 1;
        p->uv = d3s_QuadUV2;
        p->tex = tex;
        buf->npolys += 2;
        v->pos = pos;
        pos[0] = slot->x0;
        pos[1] = slot->y;
        pos[2] = slot->z1;
        v->c0 = col.d;
        v->c1 = col.d;
        v++;
        pos += 6;
        v->pos = pos;
        pos[0] = slot->x1;
        pos[1] = slot->y;
        pos[2] = slot->z1;
        v->c0 = col.d;
        v->c1 = col.d;
        v++;
        pos += 6;
        v->pos = pos;
        pos[0] = slot->x0;
        pos[1] = slot->y;
        pos[2] = slot->z0;
        v->c0 = col.d;
        v->c1 = col.d;
        v++;
        pos += 6;
        v->pos = pos;
        pos[0] = slot->x1;
        pos[1] = slot->y;
        pos[2] = slot->z0;
        v->c0 = col.d;
        v->c1 = col.d;
        buf->nverts += 4;
        ok = 1;
        d3s_NumVerts += 4;
    }
    return ok;
}
/* 0x45bcd0, height of the ground below obj (a shadow of a parent, or the bottom of its box) */

float d3s_GetGround(D3OBJECT *obj, D3OBJINFO *info, D3OBJECT *light)
{
    float y;
    D3VECTOR4 pos;
    D3OBJECT *p;
    D3BOXVERT *box;
    int i;
    char found;

    y = 0.0f;
    found = 0;
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    if (obj->parent) {
        p = obj->parent;
        while (p) {
            if ((p->objectFlags & 4) && p->material) {
                for (i = 0; i < 4 && !found; i++) {
                    if (D3S_SLOTS(p->material)[i].tex
                        && D3S_SLOTS(p->material)[i].dir.x * D3S_SLOTS(p->material)[i].dir.x
                           + D3S_SLOTS(p->material)[i].dir.y * D3S_SLOTS(p->material)[i].dir.y
                           + D3S_SLOTS(p->material)[i].dir.z * D3S_SLOTS(p->material)[i].dir.z > 0.0f) {
                        y = D3S_SLOTS(p->material)[i].y;
                        found = 1;
                    }
                }
            }
            p = p->parent;
        }
    }
    if (!found && obj->material) {
        if (obj->objectFlags & 0x20)
            return pos.y - info->mesh->radius + D3S_MESH(obj->material)->groundOffset;
        box = ((D3MESHOPS *)info->mesh)->GetBox(obj, info, D3_MESH(obj)->worldMatrix);
        y = 1e10f;
        for (i = 8; i; i--, box++)
            y = min(y, box->y);
        y += D3S_MESH(obj->material)->groundOffset;
    }
    return y;
}
/* 0x45be10, projects the mesh along the light onto the ground and renders the shadow texture
   @deep Read whole against the original and rebuilt: branch order (simple first, directional light first,
   terrain before d3sm_Shadow), 1/-ldir->y hoisted via inv = -ldir->y, one D3VECTOR for the position and
   the light vector (stored like the original), clamped cells into locals before the divisions by the new
   D3TERRAIN.bs, iz0 from z1 and iz1 from z0 as the original does, sx/sz scale hoisted,
   D3_OPS(mesh)->Transform, typed d3sm_Shadow (D3SM) and D3SVERT tu/tv. ours=609 original=609 instructions.
   Left: allocation - the original keeps obj in ebp, mesh in ebx, ldir in edi and 0 in ebp for the compares
   (cmp edi, ebp at +0x27c), y at [esp+0x10] in its own frame (frame 0x1c vs ours 0x18, ours puts it in a
   param slot), so the five slot->x0..y stores at +0x6d3 load in another order; mh in ebp at +0x3f6 where
   ours keeps [esp+0x4c]; the head schedule (d3s_Count++ before the dir copy). */

void d3s_Project(D3OBJECT *obj, D3OBJINFO *info, D3OBJINFO *mesh, D3TERRAIN *t, D3VECTOR *ldir,
                 D3VECTOR *lpos, D3OBJECT *light, D3SSLOT *slot)
{
    D3TEXHANDLE *tex = slot->tex;
    D3VECTOR pos;                   /* the object's position, then the light-to-vertex vector */
    D3SVERT *v;
    D3SPOLY *p;
    float y, f, inv, x0, x1, z0, z1, sx, sz;
    int i, k, size, mh, m;
    int cx0, cx1, cz0, cz1;

    d3s_Count++;
    slot->dir = *ldir;
    y = d3s_GetGround(obj, info, light);
    z0 = 1e10f;
    x0 = 1e10f;
    z1 = -1e10f;
    x1 = -1e10f;
    if (slot->simple) {
        d3_PointToWorld(obj, &obj->pos, &pos);
        y += 3.0f;
        x0 = pos.x - 16.0f;
        x1 = pos.x + 16.0f;
        z0 = pos.z - 16.0f;
        z1 = pos.z + 16.0f;
    } else {
        D3_OPS(mesh)->Transform(obj, mesh, obj->material->meshData);
        obj->state |= 0x10;
        v = (D3SVERT *)mesh->keys;
        if (light->objectFlags & 0x10) {
            inv = -ldir->y;
            for (i = 0; i < mesh->nkeys; i++, v++) {
                f = (v->y - y) * (1.0f / inv);
                v->x = f * ldir->x + v->x;
                v->y = f * ldir->y + v->y;
                v->z = f * ldir->z + v->z;
                z0 = min(z0, v->z);
                z1 = max(z1, v->z);
                x0 = min(x0, v->x);
                x1 = max(x1, v->x);
            }
        } else {
            for (i = 0; i < mesh->nkeys; i++, v++) {
                pos.x = lpos->x - v->x;
                pos.y = lpos->y - v->y;
                pos.z = lpos->z - v->z;
                f = -((lpos->y - y) / pos.y);
                v->x = pos.x * f + lpos->x;
                v->y = pos.y * f + lpos->y;
                v->z = f * pos.z + lpos->z;
                z0 = min(z0, v->z);
                z1 = max(z1, v->z);
                x0 = min(x0, v->x);
                x1 = max(x1, v->x);
            }
        }
    }
    if (fabs(x1) > 16384.0f || fabs(x0) > 16384.0f || fabs(z1) > 16384.0f || fabs(z0) > 16384.0f
        || fabs(x1 - x0) > 16384.0f || fabs(z1 - z0) > 16384.0f)
        return;
    slot->bz0 = -1;
    slot->map = 0;
    slot->size = 0;
    if (d3s_Enabled) {
        if (t && !slot->simple) {
            slot->ix0 = (int)((x0 - t->origin.x) / t->axisx.x);
            slot->ix1 = (int)((x1 - t->origin.x) / t->axisx.x + 1.0);
            slot->iz0 = (int)((z1 - t->origin.z) / t->axisz.z);
            slot->iz1 = (int)((z0 - t->origin.z) / t->axisz.z + 1.0);
            size = t->size;
            if (slot->ix0 < size && slot->ix1 > 0 && slot->iz0 < size && slot->iz1 > 0) {
                mh = 0;
                cx0 = max(0, slot->ix0);
                cx1 = min(size - 1, slot->ix1);
                cz0 = max(0, slot->iz0);
                cz1 = min(size - 1, slot->iz1);
                slot->bx0 = cx0 / t->bs;
                slot->bx1 = cx1 / t->bs;
                slot->bz0 = cz0 / t->bs;
                slot->bz1 = cz1 / t->bs;
                for (k = slot->bz0; k <= slot->bz1; k++)
                    for (i = slot->bx0; i <= slot->bx1; i++)
                        mh = max(mh, t->blocks[k][i].lod);
                slot->maxh = mh;
                if (mh > 1) {
                    m = ~(mh - 1);
                    slot->ix0 &= m;
                    if (slot->ix1 & mh - 1)
                        slot->ix1 = slot->ix1 + mh & m;
                    slot->iz0 &= m;
                    if (slot->iz1 & mh - 1)
                        slot->iz1 = slot->iz1 + mh & m;
                }
                slot->map = t->heights;
                slot->size = t->size;
                slot->cell.x = t->axisx.x;
                slot->cell.y = t->axisy.y;
                slot->cell.z = t->axisz.z;
                slot->origin.x = t->origin.x;
                slot->origin.y = t->origin.y;
                slot->origin.z = t->origin.z;
                x0 = slot->ix0 * t->axisx.x + t->origin.x;
                x1 = slot->ix1 * t->axisx.x + t->origin.x;
                z0 = slot->iz0 * t->axisz.z + t->origin.z;
                z1 = slot->iz1 * t->axisz.z + t->origin.z;
            }
        } else if (d3sm_Shadow && !slot->simple) {
            slot->ix0 = (int)((x0 - d3sm_Shadow->origin.x) / d3sm_Shadow->cell.x);
            slot->ix1 = (int)((x1 - d3sm_Shadow->origin.x) / d3sm_Shadow->cell.x + 1.0);
            slot->iz0 = (int)((z1 - d3sm_Shadow->origin.z) / d3sm_Shadow->cell.z);
            slot->iz1 = (int)((z0 - d3sm_Shadow->origin.z) / d3sm_Shadow->cell.z + 1.0);
            if (slot->ix0 < d3sm_Shadow->size && slot->ix1 > 0 && slot->iz0 < d3sm_Shadow->size && slot->iz1 > 0) {
                slot->map = d3sm_Shadow->data;
                slot->size = d3sm_Shadow->size;
                slot->cell.x = d3sm_Shadow->cell.x;
                slot->cell.y = d3sm_Shadow->cell.y;
                slot->cell.z = d3sm_Shadow->cell.z;
                slot->origin.x = d3sm_Shadow->origin.x;
                slot->origin.y = d3sm_Shadow->origin.y;
                slot->origin.z = d3sm_Shadow->origin.z;
                x0 = slot->ix0 * d3sm_Shadow->cell.x + d3sm_Shadow->origin.x;
                x1 = slot->ix1 * d3sm_Shadow->cell.x + d3sm_Shadow->origin.x;
                z0 = slot->iz0 * d3sm_Shadow->cell.z + d3sm_Shadow->origin.z;
                z1 = slot->iz1 * d3sm_Shadow->cell.z + d3sm_Shadow->origin.z;
            }
        }
    }
    slot->z0 = z0;
    slot->x0 = x0;
    slot->x1 = x1;
    slot->z1 = z1;
    slot->y = y;
    if (!slot->simple) {
        sx = (float)((TSTEXSIZE *)tex)->size / (x1 - x0);
        sz = (float)((TSTEXSIZE *)tex)->size / (z1 - z0);
        v = (D3SVERT *)mesh->keys;
        for (i = 0; i < mesh->nkeys; i++, v++) {
            v->tu = (v->x - x0) * sx;
            v->tv = (v->z - z0) * sz;
        }
        if (!tex->surf && d3_Hardware)
            ts_CacheFetch(tex, 2, (int)d3s_Palette);
        if (tex->surf) {
            dd_FillSurface(tex->surf, 0, 0);
            dd_Surface = dd_LockSurfaceFlags(tex->surf, 0x20);
            for (i = 0, p = (D3SPOLY *)mesh->tris; i < mesh->ntris; i++, p++)
                d3s_ShadowPolyB(p, ((TSTEXSIZE *)tex)->bpp, ((TSTEXSIZE *)tex)->size);
            dd_UnlockSurface(tex->surf);
            dd_Surface = 0;
            if (slot->cache->flag)
                d3d_BuildMipmaps(tex->surf, 0);
        }
    }
}
/* 0x45c620, updates the shadow of obj cast by light */

char d3s_Update(D3OBJECT *obj, D3OBJECT *light, D3TERRAIN *t)
{
    D3SSLOT *slot = 0;
    D3OBJINFO *info, *mesh;
    D3VECTOR ldir, lpos, opos;
    D3TEXFLAGS f;
    unsigned int size, n;
    int i, j, k;
    float eps, dist;
    char redo;
    unsigned char mh;

    if (!d3s_TexSize || obj->type >= 5 || !(obj->objectFlags & 4) || !obj->material || !(light->objectFlags & 4)
        || D3_MESH(d3s_Object)->surf[0].info.ntris >= d3s_MaxPolys * 2 - 2
        || light->lightParams.y == 0.0)
        return 0;
    for (k = 0; k < 4; k++) {
        if (D3S_SLOTS(obj->material)[k].tex && D3S_SLOTS(obj->material)[k].light == light) {
            slot = &D3S_SLOTS(obj->material)[k];
            break;
        }
    }
    if (k == 4) {
        for (k = 0; k < 4; k++)
            if (!D3S_SLOTS(obj->material)[k].tex)
                goto found;
        return 0;
found:
        slot = &D3S_SLOTS(obj->material)[k];
        slot->light = light;
        slot->map = 0;
        slot->size = 0;
        slot->maxh = 0;
        slot->bz0 = -1;
        slot->simple = obj->objectFlags >> 3 & 1;
        slot->dir.x = 0;
        slot->dir.y = 0;
        slot->dir.z = 0;
        if (slot->simple) {
            f.all = 0;
            f.b[0] = 0x7f;
            f.b[2] |= 2;
            slot->tex = (D3TEXHANDLE *)tex_Load("Schatten", f, 0, 0);
            if (slot->tex && !slot->tex->surf)
                tex_Reload(slot->tex, 0);
        } else {
            size = d3s_TexSize;
            if (obj->info->mesh && d3s_LodTexSize) {
                dist = D3_OPS(obj->info)->diagonal;
                for (n = 0; n < 2 && !(dist < d3s_Dist[n]); n++)
                    size *= 2;
                size = min(size, d3s_MaxTexSize);
            }
            slot->tex = d3s_CacheTexture(slot, d3s_ShadowMips, size, d3_FrameCount, &slot->cache);
        }
    }
    if (!slot || !slot->tex)
        return 0;
    if (slot->cache)
        slot->cache->time = d3_FrameCount;
    info = obj->info;
    mesh = info;
    if (D3_MESH(obj)->surf[3].info.ntris > 0 && D3_MESH(obj)->surf[3].info.nkeys > 0)
        mesh = &D3_MESH(obj)->surf[3].info;
    d3_PointToWorld(light, &light->pos, &lpos);
    if (light->objectFlags & 0x10) {
        eps = 0.01f;
        d3_DirToWorld(light, &d3_ZAxis, &ldir);
    } else {
        eps = 1.0f;
        d3_PointToWorld(obj, &obj->pos, &opos);
        ldir.x = opos.x - lpos.x;
        ldir.y = opos.y - lpos.y;
        ldir.z = opos.z - lpos.z;
    }
    redo = !d3_VecEqual(&ldir, &slot->dir, eps) || ((D3SURF *)info)->alwaysDraw ? 1 : 0;
    if (t) {
        if (!redo) {
            j = slot->bz0;
            if (j != -1 && !slot->simple) {
                mh = 0;
                for (; j <= slot->bz1; j++)
                    for (i = slot->bx0; i <= slot->bx1; i++)
                        mh = max(mh, t->blocks[j][i].lod);
                if (!mh)
                    return 0;
                if (mh != slot->maxh)
                    redo = 1;
            }
        }
    }
    if (redo)
        d3s_Project(obj, info, mesh, t, &ldir, &lpos, light, slot);
    if (d3s_AddShadow(slot, d3_Terrain))
        d3s_Object->state |= 4;
    return 0;
}
/* 0x45c9c0, d3_ForEachObject callback: collects up to 4 shadow casting lights */

char d3s_LightCB(D3OBJECT *obj, void *ctx)
{
    if (obj->objectFlags & 4)
        d3s_Lights[d3s_NumLights++] = obj;
    return d3s_NumLights < 4;
}
/* 0x45c9f0, collects the shadow casting lights into d3s_Lights */

void d3s_CollectLights(void)
{
    d3s_NumLights = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x10, d3s_LightCB, 0);
}
/* 0x45ca20, updates obj's shadow slots against every collected light (d3s_Lights), when obj's
   objectFlags bit 2 (shadow mode) asks for a shadow */

void d3s_UpdateObject(D3OBJECT *obj)
{
    unsigned int i;

    if (obj->objectFlags & 4) {
        for (i = 0; i < d3s_NumLights; i++)
            d3s_Update(obj, d3s_Lights[i], (D3TERRAIN *)d3_Terrain);
    }
}
/* 0x45ca60 */

void d3s_UpdateObjectAt(D3OBJECT *obj)
{
    D3OBJECT *light;
    D3OBJINFO *info;
    D3BOXVERT *box;
    D3VECTOR lpos, ldir;
    float y, f, x, z, w;
    unsigned int i;
    int k;

    obj->flags &= ~2;
    if (!(obj->stateFlags & 0x80))
        d3_UpdateWorldMatrix(obj, obj->state & 0x80 ? 0 : d3_Camera, 0);
    for (i = 0; i < d3s_NumLights; i++) {
        info = obj->info;
        light = d3s_Lights[i];
        if (!info)
            return;
        d3_PointToWorld(light, &light->pos, &lpos);
        d3_DirToWorld(light, &d3_ZAxis, &ldir);
        y = d3s_GetGround(obj, info, light);
        box = ((D3MESHOPS *)info->mesh)->GetBox(obj, info, (char *)obj->material + 8);
        for (k = 8; k; k--, box++) {
            if (!(light->objectFlags & 0x10)) {
                f = -((lpos.y - y) / (lpos.y - box->y));
                x = (lpos.x - box->x) * f + lpos.x;
                w = (lpos.y - box->y) * f + lpos.y;
                z = (lpos.z - box->z) * f + lpos.z;
            } else {
                f = -((box->y - y) / ldir.y);
                x = ldir.x * f + box->x;
                w = ldir.y * f + box->y;
                z = ldir.z * f + box->z;
            }
            x -= d3_Camera->pos.x;
            w -= d3_Camera->pos.y;
            z -= d3_Camera->pos.z;
            box->x = x * d3_Camera->matrix._11 + w * d3_Camera->matrix._21 + z * d3_Camera->matrix._31 + d3_Camera->matrix._41;
            box->y = x * d3_Camera->matrix._12 + w * d3_Camera->matrix._22 + z * d3_Camera->matrix._32 + d3_Camera->matrix._42;
            box->z = x * d3_Camera->matrix._13 + w * d3_Camera->matrix._23 + z * d3_Camera->matrix._33 + d3_Camera->matrix._43;
        }
        if (!(d3_ClipBox(box - 8, 0, 0) & 0x40)) {
            obj->flags |= 2;
            d3s_Update(obj, light, (D3TERRAIN *)d3_Terrain);
        }
    }
}
/* 0x45cc90, frees the texture of slot and clears the cache entry that owned it */

void d3s_FreeSlot(D3SSLOT *slot)
{
    D3SCENT *c;

    if (slot->tex) {
        if (c = slot->cache) {
            c->owner = 0;
            c->tex = 0;
            c->size = 0;
        }
        tex_Release(slot->tex);
        slot->tex = 0;
        slot->cache = 0;
    }
}
/* 0x45ccd0, frees the four shadow slots of obj and forgets their lights and directions */

void d3s_FreeShadows(D3OBJECT *obj)
{
    int i;

    if ((obj->objectFlags & 4) && obj->material) {
        for (i = 0; i < 4; i++) {
            if (D3S_SLOTS(obj->material)[i].tex) {
                d3s_FreeSlot(&D3S_SLOTS(obj->material)[i]);
                D3S_SLOTS(obj->material)[i].light = 0;
                D3S_SLOTS(obj->material)[i].dir.x = 0;
                D3S_SLOTS(obj->material)[i].dir.y = 0;
                D3S_SLOTS(obj->material)[i].dir.z = 0;
            }
        }
    }
}
/* 0x45cd60, d3_ForEachObject callback: frees the shadows of obj cast by light */

char d3s_FreeLightCB(D3OBJECT *obj, D3OBJECT *light)
{
    int i;

    if ((obj->objectFlags & 4) && obj->material) {
        for (i = 0; i < 4; i++) {
            if (D3S_SLOTS(obj->material)[i].tex && D3S_SLOTS(obj->material)[i].light == light) {
                d3s_FreeSlot(&D3S_SLOTS(obj->material)[i]);
                D3S_SLOTS(obj->material)[i].light = 0;
                D3S_SLOTS(obj->material)[i].dir.x = 0;
                D3S_SLOTS(obj->material)[i].dir.y = 0;
                D3S_SLOTS(obj->material)[i].dir.z = 0;
            }
        }
    }
    return 1;
}
/* 0x45ce00, drops the light direction of every shadow slot of obj (its mesh's four D3SSLOTs), so no
   shadow is drawn until d3s_Update refills them; only for an object that casts shadows (objectFlags bit 2) */

void d3s_ResetShadows(D3OBJECT *obj)
{
    int i;

    if ((obj->objectFlags & 4) && obj->material) {
        for (i = 0; i < 4; i++) {
            if (D3S_SLOTS(obj->material)[i].tex) {
                D3S_SLOTS(obj->material)[i].dir.x = 0;
                D3S_SLOTS(obj->material)[i].dir.y = 0;
                D3S_SLOTS(obj->material)[i].dir.z = 0;
            }
        }
    }
}
