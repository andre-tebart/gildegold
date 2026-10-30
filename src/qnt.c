/* qnt.c -- original file name unknown.
   Octree color quantizer: counts the image's colors in a 6-level octree (5 bits per channel), merges
   the leaves with the fewest pixels into their parents until ncolors are left (min-heap), builds the
   palette and maps the image, optionally with error diffusion.
   Extent in GildeGold.exe (inferred, not proven): 0x4206d0..0x4214a0.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/qnt.c); boundaries in decl/text.dcl, notes in notes/units/qnt.md */
#include <stdlib.h>
#include <string.h>
#include "qnt.h"

#define QTOTAL(h) qnt_Level[(h).level][(h).node].total

/* 0x4206d0, quantizes the w x h RGB image to ncolors (octree, optional error diffusion): writes the
   palette indices to out and the 256-entry palette (3 x 256 bytes, planar) to pal; 0 on out of memory */
int qnt_Quantize(unsigned char *rgb, unsigned char *out, int w, int h, int ncolors, int dither, unsigned char *pal)
{
    if (!qnt_Inited) {
        qnt_Inited = 1;
        qnt_InitTables();
    }
    if (!qnt_Alloc()) {
        qnt_Free();
        return 0;
    }
    if (!qnt_Build(rgb, w * h)) {
        qnt_Free();
        return 0;
    }
    qnt_Reduce(ncolors);
    qnt_MakePalette(0, 0);
    if (!qnt_Map(rgb, out, w, h, dither)) {
        qnt_Free();
        return 0;
    }
    qnt_Free();
    qnt_GetPalette(pal);
    return 1;
}
/* 0x420780, copies the palette the last quantization built, planar R / G / B, 256 bytes each */

void qnt_GetPalette(unsigned char *pal)
{
    int i;

    for (i = 0; i < 256; i++) {
        pal[i] = qnt_PalR[i];
        pal[i + 256] = qnt_PalG[i];
        pal[i + 512] = qnt_PalB[i];
    }
}
/* 0x4207c0, qnt_R / G / B: the 8 bits of a channel spread to their bit positions in the octree node
   index (the three tables are or-ed into the finest-level index)
   @deep behaviour: register/width spellings only - grade.py code 72, fill 1; unitdiff 92 B / 35 asm lines (2026-10-26) (the original narrows the 0x20 / 0x40 terms to mov dl,al / mov cl,al and keeps a in esi, ours mov edx,eax / and edx,0x20; same instruction multiset, no meaning error); readable met; 125 casts + 1024 u variants tried (notes/units/qnt.md) */

void qnt_InitTables(void)
{
    int i, a, a2;

    for (i = 0; i < 256; i++) {
        /* the casts give the original's byte moves (mov cl,al / mov dl,al); two different types, since the same
           cast twice is shared as one value (and edx,0xff) */
        a = (((unsigned char)i & 0x80) << 2 | (char)i & 0x40) << 2 | i & 0x20;
        a2 = a << 2; /* shared by the r and g table, as in the original */
        qnt_R[i] = (a2 | i & 0x10) << 1 | i >> 1 & 4;
        qnt_G[i] = i >> 2 & 2 | a2 | i & 0x10;
        qnt_B[i] = i >> 3 & 1 | a << 1 | i >> 1 & 8;
    }
    for (i = -255; i <= 255; i++)
        qnt_Sq[i] = i * i;
    qnt_SqPtr = qnt_Sq;
}
/* 0x420870, allocates the six octree levels (8^n nodes of 0x18 bytes); 0 on failure */

int qnt_Alloc(void)
{
    int i;

    qnt_Level[0] = calloc(sizeof(QNODE), 1);
    qnt_Level[1] = calloc(sizeof(QNODE), 8);
    qnt_Level[2] = calloc(sizeof(QNODE), 64);
    qnt_Level[3] = calloc(sizeof(QNODE), 512);
    qnt_Level[4] = calloc(sizeof(QNODE), 4096);
    qnt_Level[5] = calloc(sizeof(QNODE), 32768);
    for (i = 0; i <= 5; i++)
        if (!qnt_Level[i])
            return 0;
    qnt_Colors = 0;
    return 1;
}
/* 0x420900, frees the octree levels and the heap @deep Q2: qnt_Unk632364 (0x632364, void * next to qnt_Heap) is never written in the binary - an exe scan finds the single reference to its address at 0x42097f, the free() in this function - so its role has no evidence; every other rule holds */

void qnt_Free(void)
{
    if (qnt_Level[0])
        free(qnt_Level[0]);
    if (qnt_Level[1])
        free(qnt_Level[1]);
    if (qnt_Level[2])
        free(qnt_Level[2]);
    if (qnt_Level[3])
        free(qnt_Level[3]);
    if (qnt_Level[4])
        free(qnt_Level[4]);
    if (qnt_Level[5])
        free(qnt_Level[5]);
    if (qnt_Heap)
        free(qnt_Heap);
    if (qnt_Unk632364)
        free(qnt_Unk632364);
}
/* 0x420990, counts the n pixels into the finest level, sums their colors up to the root and puts every
   leaf into the min-heap on its pixel total
   @deep behaviour: spellings only - grade.py sib 8, code 6; unitdiff 7 B / 10 asm lines (2026-10-26) - base/index swapped in the qnt_Level[lvl][idx] addressing ([eax+ecx+0x10] vs [ecx+eax+0x10]), mov esi,eax before vs after pop ebp, test esi,esi vs test eax,eax; no meaning error; 800 access spellings tried, and a for / while+dec / do shape, a fresh block variable k and idx / lvl declared inside the if block all compile identically (2026-10-26), so the entry test register and the pop ebp position are allocator-level (notes/units/qnt.md) */

int qnt_Build(unsigned char *rgb, unsigned int n)
{
    unsigned int i, j, c, idx;
    int lvl;

    qnt_Heap = malloc(0x20004);
    if (!qnt_Heap)
        return 0;
    for (; n > 0; n--) {
        qnt_Level[5][qnt_R[rgb[0]] + qnt_G[rgb[1]] + qnt_B[rgb[2]]].count++;
        rgb += 3;
    }
    qnt_HeapCount = 0;
    for (i = 0; i < 0x8000; i++) {
        c = qnt_Level[5][i].count;
        if (c) {
            qnt_HeapCount++;
            qnt_Heap[qnt_HeapCount].level = 5;
            qnt_Heap[qnt_HeapCount].node = i;
            qnt_Level[5][i].total = c;
            j = i >> 2;
            qnt_Level[5][i].r = (((((j & 0x1000 | i & 0x800) >> 2 | i & 0x100) >> 2 | i & 0x20) >> 1 | (i & 4) << 1) + 4) * c;
            qnt_Level[5][i].g = ((((j & 0x800 | i & 0x400) >> 2 | i & 0x80) >> 2 | (i & 2) << 2 | i & 0x10) + 4) * c;
            qnt_Level[5][i].b = ((((j & 0x400 | i & 0x200) >> 2 | i & 0x40) >> 1 | ((i & 1) << 2 | i & 8) << 1) + 4) * c;
            idx = i;
            for (lvl = 4; lvl >= 0; lvl--) {
                j = idx & 7;
                idx >>= 3;
                qnt_Level[lvl][idx].total += c;
                qnt_Level[lvl][idx].mask |= 1 << j;
            }
        }
    }
    for (i = qnt_HeapCount; i > 0; i--)
        qnt_Sift(i);
    return 1;
}
/* 0x420b90, sift-down of heap entry k (heap on the nodes' total, 1-based) */

void qnt_Sift(unsigned int k)
{
    unsigned char level;
    unsigned int node;
    unsigned int half, v, j;

    level = qnt_Heap[k].level;
    node = qnt_Heap[k].node;
    half = qnt_HeapCount >> 1;
    v = qnt_Level[level][node].total;
    while (k <= half) {
        j = k * 2;
        if (j < qnt_HeapCount && QTOTAL(qnt_Heap[j]) > QTOTAL(qnt_Heap[j + 1]))
            j++;
        if (v <= QTOTAL(qnt_Heap[j]))
            break;
        qnt_Heap[k] = qnt_Heap[j];
        k = j;
    }
    qnt_Heap[k].level = level;
    qnt_Heap[k].node = node;
}
/* 0x420c90, merges the leaf with the fewest pixels into its parent until only ncolors leaves are left */

void qnt_Reduce(unsigned int ncolors)
{
    unsigned char lvl, plvl;
    unsigned int idx;
    unsigned int pidx;

    while (qnt_HeapCount > ncolors) {
        lvl = qnt_Heap[1].level;
        idx = qnt_Heap[1].node;
        plvl = lvl - 1;
        pidx = idx >> 3;
        if (qnt_Level[plvl][pidx].count) {
            qnt_Heap[1] = qnt_Heap[qnt_HeapCount];
            qnt_HeapCount--;
        } else {
            qnt_Heap[1].level = plvl;
            qnt_Heap[1].node = pidx;
        }
        qnt_Level[plvl][pidx].count += qnt_Level[lvl][idx].count;
        qnt_Level[plvl][pidx].r += qnt_Level[lvl][idx].r;
        qnt_Level[plvl][pidx].g += qnt_Level[lvl][idx].g;
        qnt_Level[plvl][pidx].b += qnt_Level[lvl][idx].b;
        qnt_Level[plvl][pidx].mask &= ~(1 << (idx & 7));
        qnt_Sift(1);
    }
}
/* 0x420db0, walks the reduced octree (children first) and gives every node that kept pixels the next
   palette index with its mean color */

void qnt_MakePalette(int idx, int lvl)
{
    int i;
    unsigned int c, g, b;

    if (qnt_Level[lvl][idx].mask) {
        for (i = 7; i >= 0; i--)
            if (qnt_Level[lvl][idx].mask & (1 << i))
                qnt_MakePalette(i + idx * 8, lvl + 1);
    }
    if (qnt_Level[lvl][idx].count) {
        qnt_Level[lvl][idx].index = qnt_Colors;
        c = qnt_Level[lvl][idx].count;
        g = qnt_Level[lvl][idx].g;
        b = qnt_Level[lvl][idx].b;
        qnt_PalR[qnt_Colors] = (qnt_Level[lvl][idx].r + (c >> 1)) / c;
        qnt_PalG[qnt_Colors] = ((c >> 1) + g) / c;
        qnt_PalB[qnt_Colors++] = ((c >> 1) + b) / c;
    }
}
/* 0x420e70, maps the image to palette indices: without dither through a lookup table over the 5-bit
   colors (qnt_Nearest per entry), with dither by error diffusion (err0 / err1 rows, cache)
   @deep behaviour: register / addressing / reload spellings only - grade.py code 147, regs 14; unitdiff 318 B / 87 asm lines (2026-10-26) (the clamp loop's three stores run on ebp/esi/edi with the 0x200 folded into one base instead of three, [eax+ecx+0xc] vs [ecx+eax+0xc], the ecx/edi/eax assignments of the palette loops and the cur / prev / lim slot reloads); same stores, same values, no meaning error; the clamp / palette spelling space (216+ variants) is in notes/units/qnt.md */

int qnt_Map(unsigned char *rgb, unsigned char *out, int w, int h, int dither)
{
    int e2;
    int cnt;
    unsigned char *lut;
    unsigned int i, n, off;
    unsigned char *clamp, *cl;
    short *cache, *err0, *err1, *cur, *prev;
    signed char *lim;
    int flip, y, x, dir, k, r, g, b, er, eg, eb, c;

    if (!dither) {
        n = w * h;
        lut = malloc(0x8000);
        if (lut) {
            for (i = 0, off = 0; off < 0x8000 * sizeof(QNODE); off += sizeof(QNODE), i++)
                if (((QNODE *)(off + (char *)qnt_Level[5]))->count)
                    lut[i] = qnt_Nearest((unsigned char)(i >> 7) & 0x80 | (unsigned char)((i & 4) << 1)
                                             | (unsigned char)(i >> 1) & 0x10 | (unsigned char)(i >> 3) & 0x20
                                             | (unsigned char)(i >> 5) & 0x40,
                                         (unsigned char)(i >> 6) & 0x80 | (unsigned char)(i >> 4) & 0x40
                                             | (unsigned char)(i >> 2) & 0x20 | (unsigned char)((i & 2) << 2)
                                             | (unsigned char)i & 0x10,
                                         (unsigned char)(((i & 1) << 2 | i & 8) << 1) | (unsigned char)(i >> 1) & 0x20
                                             | (unsigned char)(i >> 3) & 0x40 | (unsigned char)(i >> 5) & 0x80);
            for (i = 0; i < n; i++) {
                out[i] = lut[qnt_R[rgb[0]] + qnt_B[rgb[2]] + qnt_G[rgb[1]]];
                rgb += 3;
            }
            free(lut);
            return 1;
        }
    } else {
        flip = 0;
        cnt = w * 3 + 6;
        clamp = malloc(0x300);
        cl = clamp + 0x100;
        cache = malloc(0x10000);
        err0 = malloc(cnt * sizeof(short));
        err1 = malloc(cnt * sizeof(short));
        lim = malloc(0x200);
        if (clamp) {
            if (cache && err0 && err1 && lim) {
                for (k = 0; k < cnt; k++)
                    err0[k] = 0;
                memset(cache, 0xff, 0x10000);
                /* cl[k - 256] is clamp[k]: this spelling and order of the three stores give VC6's code */
                for (k = 0; k < 256; k++) {
                    cl[k] = k;
                    clamp[k + 512] = 255;
                    cl[k - 256] = 0;
                }
                for (k = 0; k < 256; k++) {
                    lim[k] = -20;
                    lim[k + 256] = 20;
                }
                for (k = -20; k <= 20; k++)
                    lim[k + 256] = k;
                for (y = 0; y < h; y++) {
                    if (flip) {
                        dir = -1;
                        rgb += w * 3 - 3;
                        out += w - 1;
                        cur = err0 + w * 3;
                        prev = err1 + 3;
                    } else {
                        dir = 1;
                        cur = err1 + w * 3;
                        prev = err0 + 3;
                    }
                    cur[2] = 0;
                    cur[1] = 0;
                    cur[0] = 0;
                    for (x = w; x > 0; x--) {
                        r = cl[rgb[0] + (prev[0] + 8 >> 4)];
                        g = cl[rgb[1] + (prev[1] + 8 >> 4)];
                        b = cl[rgb[2] + (prev[2] + 8 >> 4)];
                        k = ((r & 0xf8) << 5 | g & 0xf8) << 2 | b >> 3;
                        if (cache[k] < 0)
                            cache[k] = qnt_Nearest(r, g, b);
                        prev += 3;
                        cur -= 3;
                        *out = (unsigned char)cache[k];
                        c = cache[k];
                        er = lim[r - qnt_PalR[c] + 256];
                        eg = lim[g - qnt_PalG[c] + 256];
                        eb = lim[b - qnt_PalB[c] + 256];
                        cur[0] = er;
                        /* error diffusion: the error goes to the neighbours with weights 3, 5, 7 (of 16) through the
                           running sums er + 2er, + 2er, + 2er (VC6 builds the multiples as this add chain; the
                           products er * 3 / 5 / 7 compile differently) */
                        e2 = er + er;
                        er += e2;
                        cur[6] += er;
                        er += e2;
                        cur[3] += er;
                        e2 += er;
                        prev[0] += e2;
                        cur[1] = eg;
                        e2 = eg + eg;
                        eg += e2;
                        cur[7] += eg;
                        eg += e2;
                        cur[4] += eg;
                        e2 += eg;
                        prev[1] += e2;
                        cur[2] = eb;
                        e2 = eb + eb;
                        eb += e2;
                        cur[8] += eb;
                        eb += e2;
                        cur[5] += eb;
                        e2 += eb;
                        prev[2] += e2;
                        rgb += dir * 3;
                        out += dir;
                    }
                    if (y % 2 == 1) {
                        rgb += (w + 1) * 3;
                        out += w + 1;
                    }
                    flip = !flip;
                }
                free(clamp);
                free(cache);
                free(err0);
                free(err1);
                free(lim);
                return 1;
            }
            free(clamp);
        }
        if (cache)
            free(cache);
        if (err0)
            free(err0);
        if (err1)
            free(err1);
        if (lim)
            free(lim);
    }
    return 0;
}
/* 0x421410, palette index with the smallest squared RGB distance to the 5-bit color (r, g, b) @deep byte work only (behaviour + readable met): 2 B / 4 asm lines, the g / b registers swapped in the two sub ebx of the SqPtr terms (the original subtracts in the PalR, PalB, PalG load order); grade.py diffclass verdict order 12; all 6 term orders, 24 statement perms, per-term temps and parenthesizations tried (notes/units/qnt.md) */

int qnt_Nearest(unsigned int r, unsigned int g, unsigned int b)
{
    unsigned int i, d, best;
    int bi;

    r = (r & 0xf8) + 4;
    g = (g & 0xf8) + 4;
    b = (b & 0xf8) + 4;
    best = 200000;
    for (i = 0; i < qnt_HeapCount; i++) {
        d = qnt_SqPtr[qnt_PalR[i] - r] + qnt_SqPtr[qnt_PalB[i] - b] + qnt_SqPtr[qnt_PalG[i] - g];
        if (d < best) {
            best = d;
            bi = i;
        }
    }
    return bi;
}

unsigned short qnt_G[256] = {0};  /* 0x631228 */
unsigned char qnt_PalR[256] = {0};  /* 0x631428 */
unsigned char qnt_PalG[256] = {0};  /* 0x631528 */
unsigned char qnt_PalB[256] = {0};  /* 0x631628 */
int qnt_Colors = 0;  /* 0x631728 */
int qnt_Sq[256] = {0};  /* 0x631b34, i * i for i = -255..255 (indexed around 0) */
unsigned short qnt_B[256] = {0};  /* 0x631f38 */
int * qnt_SqPtr = 0;  /* 0x632138 */
unsigned short qnt_R[256] = {0};  /* 0x632140, r bits spread to octree index positions */
QNODE * qnt_Level[6] = {0};  /* 0x632340 */
unsigned int qnt_HeapCount = 0;  /* 0x632358 */
int qnt_Inited = 0;  /* 0x63235c */
QHEAP * qnt_Heap = 0;  /* 0x632360, 1-based min-heap on total */
void * qnt_Unk632364 = 0;  /* 0x632364 */
