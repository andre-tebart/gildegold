/* d3_fl.c -- terrain ("floor") texture cache: builds 16-bit textures for groups of map tiles by
   blending the four corner tile textures with per-pixel weights. Allocation tag "d3_fl:TextureCache", so
   probably d3_fl.c (no path string). Extent in GildeGold.exe: 0x43eab0 .. map_Region 0x446910, between the
   leaked d3_engine.c and d3_interface.c; d3_engine.c's .data ends before 0x5fb970, which holds d3fl_Blend =
   0x43eab0, followed by this file's strings up to "D:\develop\Units\d3\d3_interface.c". Functions are kept in
   their original order. Names are ours.
   Open: d3fl_* are still declared outside d3_fl.h (d3.h, d3_engine.h, cm.h, f3init.c, ls.c, lc.c).
   unit of rea 8cef7d5b (src/game/d3_fl.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_light.h"
#include "d3_d3d.h"
#include "data_views.h"
#include "bio.h"
#include "bmp.h"
#include "d3_vec.h"
#include "dd_vesa.h"
#include "f3gm.h"
#include "mem.h"
#include "ts_texture.h"
#include "d3_pose.h"
#include "vfs.h"
#include "ch.h"
#include "cm.h"
#define tex_GetFrameF ((D3FLTEX *(*)(int, unsigned char))tex_GetFrame)  /* 0x460b00 (TSTEXTURE *): this unit's D3FLTEX view of the texture */
#define tex_FindF ((D3FLTEX *(*)(char *))tex_Find)  /* 0x45f980 (TSTEXTURE *): this unit's D3FLTEX view of the texture */
#define d3_DirToWorldD ((void (*)(D3OBJECT *, D3VECTOR *, D3VECTOR *))d3_DirToWorld)  /* 0x458810 (d3_pose.h: XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
extern const char d3_ScrollPool[];

/* 0xfe79fc / 0xfe7a00 / 0xfe7a04 are d3d_UsedTexFormat.pf2 (+0x2c) + 0x10 / 0x14 / 0x18. */
#define d3d_TexRMask (d3d_UsedTexFormat.pf2.dwRBitMask)
#define d3d_TexGMask (d3d_UsedTexFormat.pf2.dwGBitMask)
#define d3d_TexBMask (d3d_UsedTexFormat.pf2.dwBBitMask)

/* 0x43eab0, blends n x n pixels of the four corner textures src[0..3] with the weights w into dst @deep read the whole function against the original 0x43eab0..0x43ed68 (the uniform-texture loop is equal; the blend loop compared statement by statement): the weight word goes through a byte union (the original stores it to [esp+0x38] and loads the bytes [esp+0x39..0x3b] with and 0xff), the channel indices are (unsigned char)(sum >> 8) (the original's sar 8 / and 0xff, not mov dl,ah). ours 214 vs 214 instructions, same statements and order of the channels (2, 1, 0). Left: allocation and scheduling in the blend loop: the original keeps a in [esp+0x10], b esi, c ecx/[esp+0x1c], e eax, ww in ecx/[esp+0x30], the dst pointer in [esp+0x20], ours a in [esp+0x10] but b/c/e in esi/ecx/eax differently and ww in ebx ('- mov eax,[ebx] / - add ebx,edx' vs '+ mov edx,[ecx] / + add ecx,eax'); the channel-0 products are summed in another order (the original s0*a, s3*e, s2*c, s1*b by its register choice; '- imul ebx,esi / - imul esi,ecx' vs '+ imul ebx,eax / + imul eax,ecx'), and the original spills the zeroed edx through [esp+0x34] around s3. */
void d3fl_BlendC(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n)
{
    unsigned int i, j;
    unsigned short *d;
    unsigned char *s, *s0, *s1, *s2, *s3;
    unsigned long *ww;
    union {
        unsigned long l;
        unsigned char b[4];             /* the weights of the four corners */
    } wv;
    int a, b, c, e;

    if (src[0] == src[1] && src[1] == src[2] && src[2] == src[3]) {
        for (j = 0; j < n; j++) {
            s = src[0];
            d = dst;
            for (i = 0; i < n; i++) {
                *d++ = (unsigned short)d3fl_ColorTab[2][s[2]] + (unsigned short)d3fl_ColorTab[1][s[1]] + (unsigned short)d3fl_ColorTab[0][s[0]];
                s += d3fl_SrcStep;
            }
            dst = (unsigned short *)((char *)dst + d3fl_DstPitch);
            src[0] += d3fl_SrcPitch;
        }
    } else {
        for (j = 0; j < n; j++) {
            s0 = src[0];
            s1 = src[1];
            s2 = src[2];
            s3 = src[3];
            d = dst;
            ww = w;
            for (i = 0; i < n; i++) {
                wv.l = *ww;
                a = wv.b[0];
                b = wv.b[1];
                c = wv.b[2];
                e = wv.b[3];
                ww = (unsigned long *)((char *)ww + d3fl_SrcStep);
                *d++ = (unsigned short)d3fl_ColorTab[2][(unsigned char)((s0[2] * a + s1[2] * b + s2[2] * c + s3[2] * e) >> 8)]
                     + (unsigned short)d3fl_ColorTab[1][(unsigned char)((s0[1] * a + s1[1] * b + s2[1] * c + s3[1] * e) >> 8)]
                     + (unsigned short)d3fl_ColorTab[0][(unsigned char)((s0[0] * a + s1[0] * b + s2[0] * c + s3[0] * e) >> 8)];
                s0 += d3fl_SrcStep;
                s1 += d3fl_SrcStep;
                s2 += d3fl_SrcStep;
                s3 += d3fl_SrcStep;
            }
            dst = (unsigned short *)((char *)dst + d3fl_DstPitch);
            src[0] += d3fl_SrcPitch;
            w = (unsigned long *)((char *)w + d3fl_WPitch);
            src[1] += d3fl_SrcPitch;
            src[2] += d3fl_SrcPitch;
            src[3] += d3fl_SrcPitch;
        }
    }
}
/* 0x43ed70, the same with MMX (all inline asm) */

void d3fl_BlendMMX(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n)
{
    __asm {
        mov eax, dst
        mov edx, w
        mov ebx, src
        mov ecx, n
        push ebp
        push esi
        push edi
        mov edi, eax
        mov ebp, edx
        mov esi, ebx
        mov edx, [esi]
        cmp [esi+4], edx
        jne blend
        cmp [esi+8], edx
        jne blend
        cmp [esi+12], edx
        jne blend
        mov esi, edx
        mov edx, ecx
        xor ebx, ebx
    row1:
        push edi
        push esi
        mov ebp, d3fl_TileN
        align 16
    pix1:
        mov eax, [esi]
        mov ebx, eax
        and ebx, 0xff
        mov ecx, d3fl_ColorTab[ebx*4]
        shr eax, 8
        mov ebx, eax
        and ebx, 0xff
        add ecx, d3fl_ColorTab[ebx*4+0x400]
        shr eax, 8
        add esi, d3fl_SrcStep
        add ecx, d3fl_ColorTab[eax*4+0x800]
        mov [edi], cx
        add edi, 2
        dec ebp
        jne pix1
        pop esi
        add esi, d3fl_SrcPitch
        pop edi
        add edi, d3fl_DstPitch
        dec edx
        jne row1
        pop edi
        pop esi
        pop ebp
        jmp done
    blend:
        xor edx, edx
        pcmpeqb mm7, mm7
        psrlw mm7, 8
    row2:
        push ecx
        push edi
        push edx
        push ebp
        mov ecx, d3fl_TileN
        align 16
    pix2:
        mov eax, [esi]
        movd mm0, [eax+edx]
        mov eax, [esi+4]
        movd mm1, [eax+edx]
        mov eax, [esi+8]
        movd mm2, [eax+edx]
        mov eax, [esi+12]
        movd mm3, [eax+edx]
        punpcklwd mm0, mm1
        punpcklwd mm2, mm3
        movq mm1, mm0
        movq mm3, mm2
        punpckldq mm0, mm2
        punpckhdq mm1, mm3
        movq mm2, mm0
        pand mm2, mm7
        psrlw mm0, 8
        punpcklbw mm6, [ebp]
        add edx, d3fl_SrcStep
        add ebp, d3fl_SrcStep
        psrlw mm6, 8
        pmaddwd mm2, mm6
        movq mm3, mm2
        psrlq mm2, 32
        paddd mm3, mm2
        psrld mm3, 8
        movd ebx, mm3
        mov eax, d3fl_ColorTab[ebx*4]
        pmaddwd mm0, mm6
        movq mm4, mm0
        psrlq mm0, 32
        paddd mm4, mm0
        psrld mm4, 8
        movd ebx, mm4
        add eax, d3fl_ColorTab[ebx*4+0x400]
        pmaddwd mm1, mm6
        movq mm5, mm1
        psrlq mm1, 32
        paddd mm5, mm1
        psrld mm5, 8
        movd ebx, mm5
        add eax, d3fl_ColorTab[ebx*4+0x800]
        mov [edi], ax
        add edi, 2
        dec ecx
        jne pix2
        pop ebp
        add ebp, d3fl_WPitch
        pop edx
        add edx, d3fl_SrcPitch
        pop edi
        add edi, d3fl_DstPitch
        pop ecx
        dec ecx
        jne row2
        pop edi
        pop esi
        pop ebp
    done:
    }
}
/* 0x43eef0, draws the tiles of r into the 16-bit texture pixels dst */

void d3fl_Render(unsigned short *dst, D3FLTILES *r)
{
    unsigned char *s[4];
    unsigned long *w;
    unsigned int m, i, j;
    unsigned char **base;
    int off;

    s[0] = (unsigned char *)d3fl_W16;
    s[1] = (unsigned char *)d3fl_W32;
    s[2] = (unsigned char *)d3fl_W64;
    d3fl_TileN = r->texsize / r->n;
    m = min(64, max(16, d3fl_TileN));
    w = (unsigned long *)s[m >> 5];
    base = r->src[2 - (m >> 5)];
    d3fl_SrcStep = r->n * m * 4 / r->texsize;
    d3fl_WPitch = d3fl_SrcStep * m;
    d3fl_SrcPitch = d3fl_SrcStep << 6;
    d3fl_DstPitch = r->texsize * 2;
    for (j = 0; j < r->n; j++)
        for (i = 0; i < r->n; i++) {
            off = ((((r->y + j) * m) & 0x3f) * 64 + (((r->x + i) * m) & 0x3f)) * 4;
            s[0] = base[r->tiles[j][i]] + off;
            s[1] = base[r->tiles[j][i + 1]] + off;
            s[2] = base[r->tiles[j + 1][i]] + off;
            s[3] = base[r->tiles[j + 1][i + 1]] + off;
            d3fl_Blend(dst + (r->texsize * j + i) * d3fl_TileN, w, s, d3fl_TileN);
        }
    if (dd_HasMMX)
        _m_empty();
}
/* 0x43f080, bilinear weights of the four corners for an n x n tile, 4 bytes per pixel */

void d3fl_MakeWeights(unsigned char *p, unsigned int n)
{
    int step = 0x100 / n;
    unsigned int x, y;
    int w[4], d[4];

    for (y = 0; y < n; y++, p += n * 4) {
        w[2] = (y * step) << 8;
        w[0] = 0xffff - w[2];
        w[3] = 0;
        w[1] = 0;
        d[0] = -(d[1] = (w[0] >> 8) * step);
        d[2] = -(d[3] = (w[2] >> 8) * step);
        for (x = 0; x < n; x++) {
            p[x * 4] = w[0] >> 8;
            p[x * 4 + 1] = w[1] >> 8;
            p[x * 4 + 2] = w[2] >> 8;
            p[x * 4 + 3] = w[3] >> 8;
            w[0] += d[0];
            w[1] += d[1];
            w[2] += d[2];
            w[3] += d[3];
        }
    }
}
/* 0x43f180, creates the texture cache of n empty entries, builds the three blend weight tables and selects
   the MMX blend routine (the C one without MMX) */

void d3fl_InitCache(unsigned int n)
{
    unsigned int i;

    d3fl_Count = n;
    d3fl_Cache = mem_Alloc(n * sizeof(D3FLCACHE), "d3_fl:TextureCache");
    for (i = 0; i < d3fl_Count; i++) {
        d3fl_Cache[i].tex = 0;
        d3fl_Cache[i].flag = 0;
        d3fl_Cache[i].stamp = 0;
        d3fl_Cache[i].size = 0;
        D3_ZERO(d3fl_Cache[i].t.tiles, sizeof(d3fl_Cache[i].t.tiles));
    }
    d3fl_MakeWeights((unsigned char *)d3fl_W64, 64);
    d3fl_MakeWeights((unsigned char *)d3fl_W32, 32);
    d3fl_MakeWeights((unsigned char *)d3fl_W16, 16);
    d3fl_Blend = d3fl_BlendMMX;
    if (!dd_HasMMX)
        d3fl_Blend = d3fl_BlendC;
}
/* 0x43f270, releases the cached textures, frees the cache and selects the C blend routine */

void d3fl_FreeCache(void)
{
    unsigned int i;

    for (i = 0; i < d3fl_Count; i++) {
        if (d3fl_Cache[i].tex) {
            if (d3fl_Cache[i].tex->name[0] == '*')
                while (d3fl_Cache[i].tex->refs > 0)
                    tex_Release((D3TEXHANDLE *)d3fl_Cache[i].tex);
            d3fl_Cache[i].tex = 0;
            d3fl_Cache[i].flag = 0;
        }
    }
    d3fl_Count = 0;
    mem_Free(d3fl_Cache);
    d3fl_Cache = 0;
    d3fl_Blend = d3fl_BlendC;
}
/* 0x43f300, 256 texture triangles: 4 fixed ones covering the inner square (f..1-f), then 63 x 4 randomly
   placed, scaled (0.9..1.23) and rotated copies of the same four triangles @deep read the whole function against the original 0x43f300..0x43f895: the loop now turns each corner (local vectors c01 / c10 / c11) by m into r and writes r * s + o (the original's x87 order: both components summed, then both scaled; the zero terms m * 0.0 kept, not factored as literal 0.0f made VC6 do), rot.z before rot.x / rot.y (the original stores the zeros after the 4th rand). The four loop blocks now match the original's sequence instruction for instruction but for registers and slots. Left: (1) the head (t[0..11]): the original keeps g = 1 - f - f in four stack slots ([esp+0x14], [esp+0x80], [esp+0x30], [esp+0x2c] via mov) and adds f to a different copy per corner (A+f for P1.x, B+f for P3.y, C+f / D+f for P2), storing t[1] before t[0]; ours computes g + f once and keeps it on the x87 stack (52 vs 70 instructions: '- fld st(1) / - fstp [eax] / - fadd st(1)' vs '+ fst [esp+0x14] / + fst [esp+0x80] / + fstp [esp+0x30] / + mov ecx,[esp+0x14]'); tried g + f / f + g, the corner form needs four variables that nothing in the bytes names; (2) the loop pointer: the original biases esi = out + 0x80 (t + 16 entries, '[esi-0x20]'), ours out + 0x60; (3) slots: ox/oy/s at [esp+0x10]/[0xc]/[8] vs ours, frame 0x84 vs 0x78. (size: 336 vs 359 instructions, explained: the 23 missing instructions are the head's: the original spills g to four stack copies and reloads each (fst/fld/mov pairs, 18 of them) where ours keeps g + f on the x87 stack; the loop blocks differ by 1..2 each (the esi bias and slot reloads)) */

void d3fl_MakeJitter(float *out)
{
    D3FLUV *t = (D3FLUV *)out;
    float f, g, ox, oy, s;
    D3VECTOR4 rot;
    D3MATRIX m;
    D3VECTOR r;                         /* a corner turned by m */
    D3VECTOR c01 = {0.0f, 1.0f, 0.0f}, c10 = {1.0f, 0.0f, 0.0f}, c11 = {1.0f, 1.0f, 0.0f};
    int i;

    f = 1.0f / d3fl_DetailScale;
    g = 1.0f - f - f;
    t[0].u = f;
    t[0].v = g + f;
    t[1].u = f;
    t[1].v = f;
    t[2].u = g + f;
    t[2].v = f;
    t[3].u = g + f;
    t[3].v = f;
    t[4].u = g + f;
    t[4].v = g + f;
    t[5].u = f;
    t[5].v = g + f;
    t[6].u = f;
    t[6].v = f;
    t[7].u = g + f;
    t[7].v = g + f;
    t[8].u = f;
    t[8].v = g + f;
    t[9].u = f;
    t[9].v = f;
    t[10].u = g + f;
    t[10].v = f;
    t[11].u = g + f;
    t[11].v = g + f;
    for (i = 1, t += 12; i < 64; i++, t += 12) {
        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        rot.x = 0.0f;
        rot.y = 0.0f;
        d3_MatFromAngles(&rot, &m);
        r.x = c01.x * m._11 + c01.y * m._21 + c01.z * m._31;
        r.y = c01.x * m._12 + c01.y * m._22 + c01.z * m._32;
        t[0].u = r.x * s + ox;
        t[0].v = r.y * s + oy;
        t[1].u = ox;
        t[1].v = oy;
        r.x = c10.x * m._11 + c10.y * m._21 + c10.z * m._31;
        r.y = c10.x * m._12 + c10.y * m._22 + c10.z * m._32;
        t[2].u = r.x * s + ox;
        t[2].v = r.y * s + oy;

        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        rot.x = 0.0f;
        rot.y = 0.0f;
        d3_MatFromAngles(&rot, &m);
        r.x = c10.x * m._11 + c10.y * m._21 + c10.z * m._31;
        r.y = c10.x * m._12 + c10.y * m._22 + c10.z * m._32;
        t[3].u = r.x * s + ox;
        t[3].v = r.y * s + oy;
        r.x = c11.x * m._11 + c11.y * m._21 + c11.z * m._31;
        r.y = c11.x * m._12 + c11.y * m._22 + c11.z * m._32;
        t[4].u = r.x * s + ox;
        t[4].v = r.y * s + oy;
        r.x = c01.x * m._11 + c01.y * m._21 + c01.z * m._31;
        r.y = c01.x * m._12 + c01.y * m._22 + c01.z * m._32;
        t[5].u = r.x * s + ox;
        t[5].v = r.y * s + oy;

        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        rot.x = 0.0f;
        rot.y = 0.0f;
        d3_MatFromAngles(&rot, &m);
        t[6].u = ox;
        t[6].v = oy;
        r.x = c11.x * m._11 + c11.y * m._21 + c11.z * m._31;
        r.y = c11.x * m._12 + c11.y * m._22 + c11.z * m._32;
        t[7].u = r.x * s + ox;
        t[7].v = r.y * s + oy;
        r.x = c01.x * m._11 + c01.y * m._21 + c01.z * m._31;
        r.y = c01.x * m._12 + c01.y * m._22 + c01.z * m._32;
        t[8].u = r.x * s + ox;
        t[8].v = r.y * s + oy;

        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        rot.x = 0.0f;
        rot.y = 0.0f;
        d3_MatFromAngles(&rot, &m);
        t[9].u = ox;
        t[9].v = oy;
        r.x = c10.x * m._11 + c10.y * m._21 + c10.z * m._31;
        r.y = c10.x * m._12 + c10.y * m._22 + c10.z * m._32;
        t[10].u = r.x * s + ox;
        t[10].v = r.y * s + oy;
        r.x = c11.x * m._11 + c11.y * m._21 + c11.z * m._31;
        r.y = c11.x * m._12 + c11.y * m._22 + c11.z * m._32;
        t[11].u = r.x * s + ox;
        t[11].v = r.y * s + oy;
    }
}
/* 0x43f8a0, sets the terrain detail level n (4 .. 0x100): clamps n to 0x40, the detail scale is the power of two
   of that lowest bit (1 << s), the detail shift 6 - s and the size shift n >> 7, then rebuilds the jitter and
   flushes the floor cache */

void d3fl_SetDetail(unsigned int n)
{
    if (n <= 0x100 && n >= 4) {
        unsigned int m = n > 0x40 ? 0x40 : n;
        unsigned char d;
        int s = 0;

        while (!(m & 1)) {
            m >>= 1;
            s++;
        }
        d = (unsigned char)(n >> 7);
        d3fl_DetailSizeShift = 2;
        d3fl_DetailScale = 1 << s;
        d3fl_DetailShift = 6 - s;
        if (d <= 2)
            d3fl_DetailSizeShift = d;
        d3fl_MakeJitter(d3fl_Jitter);
        d3fl_Flush();
    }
}
/* 0x43f910, reloads the textures of a map (does nothing for a scene without one) */

void d3fl_ReloadMap(void *p)
{
    if (p)
        d3fl_ReloadTextures(p);
}
/* 0x43f930, switches mipmapping and reloads the textures of the terrain and of every scene's map */

void d3fl_SetMips(char on)
{
    int i;

    if (on != d3fl_Mips) {
        d3fl_Flush();
        d3fl_Mips = on;
        if (d3_Terrain)
            d3fl_ReloadMap(d3_Terrain);
        for (i = 0; i < D3_MAX_SCENES; i++)
            if (d3_Scenes[i].map && d3_Scenes[i].map != d3_Terrain)
                d3fl_ReloadMap(d3_Scenes[i].map);
    }
}
/* 0x43f990, empties the cache */

void d3fl_Flush(void)
{
    unsigned int i;

    for (i = 0; i < d3fl_Count; i++) {
        if (d3fl_Cache[i].tex) {
            if (d3fl_Cache[i].tex->name[0] == '*')
                while (d3fl_Cache[i].tex->refs > 0)
                    tex_Release((D3TEXHANDLE *)d3fl_Cache[i].tex);
            d3fl_Cache[i].tex = 0;
        }
        d3fl_Cache[i].flag = 0;
        d3fl_Cache[i].stamp = 0;
        d3fl_Cache[i].size = 0;
        d3fl_Cache[i].hash = 0;
        D3_ZERO(d3fl_Cache[i].t.tiles, sizeof(d3fl_Cache[i].t.tiles));
    }
    if (d3_Terrain)
        d3fl_Invalidate(d3_Terrain);
    for (i = 0; i < D3_MAX_SCENES; i++)
        if (d3_Scenes[i].map && d3_Scenes[i].map != d3_Terrain)
            d3fl_Invalidate(d3_Scenes[i].map);
}
/* 0x43faa0, a free cache entry, or the least recently used one */

D3FLCACHE *d3fl_Alloc(void)
{
    D3FLCACHE *c = d3fl_Cache, *best = 0;
    unsigned int i, oldest = d3_FrameCount;

    for (i = 0; i < d3fl_Count; i++, c++) {
        if (!c->tex) {
            c->stamp = d3_FrameCount;
            return c;
        }
        if (c->stamp < oldest) {
            oldest = c->stamp;
            best = c;
        }
    }
    if (best) {
        best->stamp = d3_FrameCount;
        if (best->tex)
            best->tex->reserved = 0;
    }
    return best;
}
/* 0x43fb00, the cache entry for the (size + 1)^2 tiles at x, y */

D3FLCACHE *d3fl_Find(unsigned char *map, int mapsize, int x, int y, unsigned int size, int *phash)
{
    unsigned char tiles[5][5];
    unsigned int i, j;
    int hash;
    D3FLCACHE *c;
    int mask = mapsize - 1;
    unsigned int n = size + 1;

    D3_ZERO(tiles, sizeof(tiles));
    for (j = 0; j < n; j++)
        for (i = 0; i < n; i++)
            tiles[j][i] = map[((x + i) & mask) + ((j + y) & mask) * mapsize];
    hash = util_Hash(tiles, 25) + d3_CurrentScene;
    if (phash)
        *phash = hash;
    for (i = 0, c = d3fl_Cache; i < d3fl_Count; i++, c++)
        if (c->tex && c->size == size && c->hash == hash) {
            d3fl_Cache[i].stamp = d3_FrameCount;
            return &d3fl_Cache[i];
        }
    return 0;
}
/* 0x43fc10, the cache texture for the tiles at x, y (created if needed) */

D3FLTEX *d3fl_GetTexture(unsigned char *map, int mapsize, int x, int y, unsigned int size, D3FLSRC *src)
{
    D3FLCACHE *c;
    int hash;
    unsigned int i, j;
    union {
        int i;                      /* the flags as the dword the original zeroes at once */
        D3TEXFLAGS f;
    } u;

    if (c = d3fl_Find(map, mapsize, x, y, size, &hash))
        return c->tex;
    if (c = d3fl_Alloc()) {
        char names[5][7] = { "*", "*fltx1", "*fltx2", "*", "*fltx4" };

        int mask;

        c->size = size;
        mask = mapsize - 1;
        for (j = 0; j <= size; j++)
            for (i = 0; i <= size; i++)
                c->t.tiles[j][i] = map[((x + i) & mask) + ((j + y) & mask) * mapsize];
        c->hash = hash;
        c->t.n = size;
        c->t.x = x;
        c->t.y = y;
        c->t.src = src;
        if (!c->tex) {
            c->t.texsize = min((0x40 >> d3fl_DetailShift) * size, d3fl_DetailScale << d3fl_DetailSizeShift);
            u.i = 0;
            u.f.b[0] = 0xff;
            c->tex = (D3FLTEX *)tex_CreateHi(names[size], c->t.texsize, u.f, 0, 0, d3fl_Mips);
            c->tex->reserved = 0;
        } else
            strcpy(c->tex->name, names[size]);
        c->tex->cache = c;
        c->flag = 0;
        return c->tex;
    }
    return 0;
}
/* 0x43fe00, starts the terrain texture system: the colour tables, the cache of n entries, the detail level,
   mipmapping, the map shift and the jitter / uv tables */

void d3fl_Init(unsigned int n, unsigned int detail, char mips, char b)
{
    dd_MakeColorTables(d3d_TexRMask, d3d_TexGMask, d3d_TexBMask, d3fl_ColorTab[0]);
    d3fl_InitCache(n);
    d3fl_SetDetail(detail);
    d3fl_Mips = mips;
    d3fl_MapShift = b;
    d3fl_UV[0][0] = 0.0f;
    d3fl_UV[0][1] = 0.0f;
    d3fl_UV[0][2] = 1.0f;
    d3fl_UV[0][3] = 1.0f;
    d3fl_UV[0][4] = 0.0f;
    d3fl_UV[0][5] = 1.0f;
    d3fl_UV[1][0] = 0.0f;
    d3fl_UV[1][1] = 0.0f;
    d3fl_UV[1][2] = 1.0f;
    d3fl_UV[1][3] = 0.0f;
    d3fl_UV[1][4] = 1.0f;
    d3fl_UV[1][5] = 1.0f;
    d3fl_MakeJitter(d3fl_Jitter);
}
/* 0x43fed0, frees the texture cache and resets the detail scale */

void d3fl_Exit(void)
{
    d3fl_FreeCache();
    d3fl_DetailScale = 0;
}
/* 0x43fee0, detail level (1, 2 or 4) of a tile, by its distance */

char d3fl_TileLod(D3FLMAP *map, D3FLTILE *t, float unused, int detail)
{
    char lod;
    char old;
    float d, k;

    if (!(lod = map->lod)) {
        if (!map->divide[0])
            return 0;
        d = t->centerDist / map->scale;
        k = (40.0f - detail) * 0.4f;
        if (d > (k + map->lodDist4) + t->lodDistBias)
            lod = 4;
        else if (d > (k + map->lodDist2) + t->lodDistBias)
            lod = 2;
        else
            lod = 1;
        old = t->lod;
        if (old && old != -1 && lod != old) {
            t->nextLod = lod;
            if (++t->lodDelay <= 8)
                return old;
        }
        t->lodDelay = 0;
    }
    return lod;
}
/* 0x43ff90, frees a floor map and everything it owns: the source images, every tile's geometry (and its
   water data), the height / type / water / light arrays, the regions' textures and the divide arrays, then
   the map itself */

void d3fl_FreeMap(D3FLMAP *map)
{
    int i, j;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 3; j++)
            if (map->src[j][i]) {
                mem_Free(map->src[j][i]);
                map->src[j][i] = 0;
            }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            mem_Free(map->tiles[j][i].points);
            map->tiles[j][i].points = 0;
            mem_Free(map->tiles[j][i].polys);
            map->tiles[j][i].polys = 0;
            mem_Free(map->tiles[j][i].split);
            map->tiles[j][i].split = 0;
            mem_Free(map->tiles[j][i].bp);
            map->tiles[j][i].bp = 0;
            if (map->tiles[j][i].water) {
                mem_Free(map->tiles[j][i].wregions);
                map->tiles[j][i].wregions = 0;
                mem_Free(map->tiles[j][i].wpoints);
                map->tiles[j][i].wpoints = 0;
                mem_Free(map->tiles[j][i].wpolys);
                map->tiles[j][i].wpolys = 0;
            }
        }
    if (map->height) {
        mem_Free(map->height);
        map->height = 0;
    }
    if (map->types) {
        mem_Free(map->types);
        map->types = 0;
    }
    if (map->water) {
        mem_Free(map->water);
        map->water = 0;
    }
    if (map->light) {
        mem_Free(map->light);
        map->light = 0;
    }
    if (map->lightofs) {
        mem_Free(map->lightofs);
        map->lightofs = 0;
    }
    if (map->regions) {
        for (i = 0; i < map->nregions; i++)
            tex_Release((D3TEXHANDLE *)map->regions[i].tex);
        mem_Free(map->regions);
        map->regions = 0;
    }
    for (i = 0; i < 3; i++)
        if (map->divide[i]) {
            mem_Free(map->divide[i]);
            map->divide[i] = 0;
        }
    d3fl_Flush();
    mem_Free(map);
}
/* 0x440140, marks the map dirty and every tile's lod invalid (-1), so the next use recomputes them */

void d3fl_Invalidate(D3FLMAP *map)
{
    int i, j;

    map->dirty = 1;
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            map->tiles[j][i].lod = -1;
}
/* 0x440170, flood fill */

void d3fl_Fill(char *map, int size, int x, int y, char from, char to)
{
    map[y * size + x] = to;
    if (x + 1 < size && map[y * size + x + 1] == from)
        d3fl_Fill(map, size, x + 1, y, from, to);
    if (x - 1 >= 0 && map[y * size + x - 1] == from)
        d3fl_Fill(map, size, x - 1, y, from, to);
    if (y + 1 < size && map[(y + 1) * size + x] == from)
        d3fl_Fill(map, size, x, y + 1, from, to);
    if (y - 1 >= 0 && map[(y - 1) * size + x] == from)
        d3fl_Fill(map, size, x, y - 1, from, to);
}
/* 0x440280, the water point of a tile at x, y */

D3FLPOINT *d3fl_WaterPoint(D3FLTILE *t, int x, int y, char flag)
{
    D3FLPOINT *p = 0;
    D3FLWREG *r;

    if (t && t->wregions) {
        for (r = t->wregions; r->y >= 0; r++)
            if (y == r->y && x >= r->x0 && x <= r->x1) {
                if (r->flag == -1)
                    r->flag = flag;
                p = &t->wpoints[r->idx - r->x0 + x];
                break;
            }
    }
    return p;
}
/* 0x4402f0, linear ramp from v0 at p[a] to v1 at p[b] */

void d3fl_Ramp(unsigned char *p, int a, int b, unsigned char v0, unsigned char v1)
{
    float step = (float)(v1 - v0) / (b - a);
    float f = v0;
    int i;

    for (i = a; i <= b; i++) {
        p[i] = (unsigned char)(f + 0.5f);
        f += step;
    }
}
/* 0x440360, finds the water tile type ("WASSER"), builds the water height map, its regions and the water
   strips, points and polygons of the tiles @deep read the whole function against the original 0x440360..0x441253 (water type search, mask and its 8 neighbours, the shore ramps, the diagonal switch with its tables, regions, region records, strips/points/polys, the removed-region compaction, the frees) and fixed: if (found) {...} else watertype = 0xff (the original lays the found branch first, 0x441149 after the cmask free), the strips test if (!in) {...} else if (!v) (the original tests in first, 0x440ce7), the corner bits built in the unsigned char q with byte adds (add bl,bl / or bl,dl, the last bit and 0xff), the flag words cleared through D3TEXFLAGS.all. ours 1331 vs 1326 instructions, switch tables equal. Left: allocation and scheduling: the 8 neighbour writes of the mask keep x+1 / x-1 in [ebp-0x28] / [ebp-4] and y-1 / y+1 in other registers ('- mov ebx,[esi+0xc] / - mov edx,[esi]' vs '+ mov ebx,ecx / + mov edx,[esi+0xc]', a 29/30-line regs/slots run at 0x4404f4..0x4405bb), the region init loop rotated with i in [ebp-0x30] and a jmp into the body ('+ jmp 0x440b29 / + mov edx,[ebp-0x30]', 0x440aee), the strip loop counters swap [ebp+8] / [ebp-0x14] / [ebp-0x1c] (0x440f51, 0x44106d), and the slots of found / k / tx (e.g. [ebp-1] vs [ebp-0x15]). */

void d3fl_PrepareWater(D3FLMAP *map)
{
    char msg[256];
    char buf[64];
    unsigned char *mask, *cmask, *c, *w, *w2;
    D3FLREGION *regs;
    D3FLTEX *tex;
    D3FLTILE *t;
    D3FLWREG *r;
    D3FLPOLY *p;
    D3TEXFLAGS f;
    int oldn, i, j, x, y, k, n, x0, x1, y1, np, npoly, first, left, off;
    unsigned char found, in, v, h0, h1, q;
    int tx, ty;

    oldn = map->nregions;
    map->nregions = 0;
    if (map->watertype == 0xff)
        for (i = 0; i < 8; i++) {
            strcpy(buf, map->typenames[i]);
            _strupr(buf);
            if (strstr(buf, "WASSER")) {
                map->watertype = i;
                break;
            }
        }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            map->tiles[j][i].water = 0;
    if (map->watertype < 0xff) {
        found = 0;
        mask = mem_Alloc(map->size * map->size, "d3_fl:Water(water_mask)");
        for (y = 0; y < map->size - 4; y++)
            for (x = 0; x < map->size - 4; x++) {
                k = map->size * y + x;
                if (map->types[k] == map->watertype) {
                    found = 1;
                    ty = y / map->n;
                    tx = x / map->n;
                    map->tiles[ty][tx].water = 1;
                    if (y % map->n == 0 && y > 0)
                        map->tiles[ty - 1][tx].water = 1;
                    if (x % map->n == 0 && x > 0)
                        map->tiles[ty][tx - 1].water = 1;
                    if (x % map->n == 0 && x > 0 && y % map->n == 0 && y > 0)
                        map->tiles[ty - 1][tx - 1].water = 1;
                    mask[k] = 0xff;
                    mask[map->size * y + ((x - 1) & map->mask)] = 0xff;
                    mask[map->size * y + ((x + 1) & map->mask)] = 0xff;
                    mask[((y - 1) & map->mask) * map->size + ((x - 1) & map->mask)] = 0xff;
                    mask[((y - 1) & map->mask) * map->size + x] = 0xff;
                    mask[((y - 1) & map->mask) * map->size + ((x + 1) & map->mask)] = 0xff;
                    mask[((y + 1) & map->mask) * map->size + ((x - 1) & map->mask)] = 0xff;
                    mask[((y + 1) & map->mask) * map->size + x] = 0xff;
                    mask[((y + 1) & map->mask) * map->size + ((x + 1) & map->mask)] = 0xff;
                }
            }
        if (found) {
            if (map->water) {
                for (i = 0; i < map->size * map->size; i++) {
                    map->water[i] &= mask[i];
                    if (mask[i] == 0xff && !map->water[i])
                        map->water[i] = 0xff;
                }
            } else {
                map->water = mem_Alloc(map->size * map->size, "d3_fl:Water(height)");
                D3_COPY(map->water, mask, map->size * map->size);
            }
            /* ramps from the shore height into the water, row by row */
            for (y = 0; y < map->size; y++) {
                in = 0;
                for (x = 0; x < map->size; x++) {
                    k = map->size * y + x;
                    v = map->water[k];
                    if (v == 0xff && !in) {
                        in = 1;
                        x0 = x;
                        if (x > 0 && map->water[k - 1] > 0)
                            h0 = max(1, map->water[k - 1]);
                        else
                            h0 = max(1, map->height[k] - 2);
                    } else if (v != 0xff && in) {
                        in = 0;
                        if (v > 0)
                            h1 = max(1, v);
                        else
                            h1 = max(1, map->height[k - 1] - 2);
                        d3fl_Ramp(&map->water[map->size * y], x0, x - 1, h0, h1);
                    }
                }
                if (in) {
                    h1 = max(1, map->height[(y + 1) * map->size - 1] - 2);
                    d3fl_Ramp(&map->water[map->size * y], x0, map->size - 1, h0, h1);
                }
            }
            /* diagonals of the quads at the shore */
            for (y = 0; y < map->size; y++)
                for (x = 0; x < map->size; x++) {
                    /* the water corners of the quad as bits: (x, y) 8, (x, y + 1) 4, (x + 1, y) 2, (x + 1, y + 1) 1 */
                    q = map->types[map->size * y + x] == map->watertype;
                    q = q * 2 | map->types[((y + 1) & map->mask) * map->size + x] == map->watertype;
                    q = q * 2 | map->types[map->size * y + ((x + 1) & map->mask)] == map->watertype;
                    q *= 2;
                    switch (q | (unsigned char)(map->types[((y + 1) & map->mask) * map->size + ((x + 1) & map->mask)] == map->watertype)) {
                    case 2:
                    case 4:
                    case 6:
                    case 11:
                    case 13:
                        map->divide[0][map->size * y + x] &= 0x7f;
                        break;
                    case 1:
                    case 7:
                    case 8:
                    case 9:
                    case 14:
                        map->divide[0][map->size * y + x] |= 0x80;
                        break;
                    }
                }
            /* regions: connected quads with water at all four corners */
            cmask = mem_Alloc(map->size * map->size, "d3_fl:Water(CalcRegions)");
            D3FL_FILL(cmask, 0xff, map->size * map->size);
            for (y = 0; y < map->size - 1; y++)
                for (x = 0; x < map->size - 1; x++) {
                    c = &cmask[map->size * y + x];
                    if (*c == 0xff) {
                        w = &map->water[map->size * y + x];
                        w2 = &map->water[(y + 1) * map->size + x];
                        if (*w > 0 && *w2 > 0 && w[1] > 0 && w2[1] > 0)
                            *c = 0xfe;
                    }
                }
            for (y = 0; y < map->size - 1; y++)
                for (x = 0; x < map->size - 1; x++)
                    if (cmask[map->size * y + x] == 0xfe)
                        d3fl_Fill((char *)cmask, map->size, x, y, 0xfe, map->nregions++);
            if (map->nregions != oldn) {
                regs = mem_Alloc(map->nregions * sizeof(D3FLREGION), "d3_fl:Water(Regions)");
                if (oldn > map->nregions)
                    D3FL_COPY_TO(regs, map->regions, map->nregions * sizeof(D3FLREGION))
                else {
                    first = 0;
                    if (oldn > 0) {
                        D3FL_COPY_TO(regs, map->regions, oldn * sizeof(D3FLREGION));
                        first = oldn;
                    }
                    if (!(tex = tex_FindF("EF_WASS_06A_2T_W_AN0"))) {
                        f.all = 0;
                        f.b[0] = 0xac;
                        if (tex = (D3FLTEX *)tex_Load("EF_WASS_06A_2T_W_AN0", f, 0, 0))
                            tex_Reload((D3TEXHANDLE *)tex, 0);
                    }
                    for (i = first; i < map->nregions; i++) {
                        regs[i].tex = regs[i].tex2 = tex;
                        if (tex) {
                            regs[i].flags = tex->flags;
                            if (i < map->nregions - 1)
                                tex_AddRef(tex, 1);
                        } else
                            regs[i].flags.all = 0;
                        regs[i].f14 = 62.0f;
                        regs[i].speed[0] = 0.023f;
                        regs[i].speed[1] = 0.02f;
                        regs[i].speed[2] = 0.027f;
                        regs[i].speed[3] = 0.03f;
                        regs[i].amp[0] = 1.0f;
                        regs[i].amp[1] = 1.0f;
                        regs[i].amp[2] = 1.0f;
                        regs[i].amp[3] = 31.0f;
                        regs[i].scrollSpeedU = 0;
                        regs[i].scrollSpeedV = 0;
                        regs[i].time = d3_Time;
                        regs[i].vOfs = 0;
                        regs[i].uOfs = 0;
                        regs[i].wavePhase[0] = 0;
                        regs[i].wavePhase[1] = 0;
                        regs[i].wavePhase[2] = 0;
                        regs[i].wavePhase[3] = 0;
                    }
                }
                if (map->regions) {
                    mem_Free(map->regions);
                    map->regions = 0;
                }
                map->regions = regs;
            }
            /* water strips, points and polygons of the tiles */
            for (j = 0; j < 8; j++)
                for (i = 0; i < 8; i++)
                    if (map->tiles[j][i].water) {
                        t = &map->tiles[j][i];
                        if (!t->wregions)
                            t->wregions = mem_Alloc(map->n * map->n / 2 * sizeof(D3FLWREG), "d3_fl:Water(strips)");
                        r = t->wregions;
                        np = 0;
                        npoly = 0;
                        y1 = j == 7 ? map->size - 4 : (j + 1) * map->n;
                        for (y = map->n * j; y <= y1; y++) {
                            in = 0;
                            x1 = i == 7 ? map->size - 4 : (i + 1) * map->n;
                            n = map->n;
                            for (x = n * i; x <= x1; x++) {
                                k = (y & map->mask) * map->size + (x & map->mask);
                                v = map->water[k];
                                if (!in) {
                                    if (v > 0 && ((y < (j + 1) * n && x < (i + 1) * n && cmask[k] < 0xff)
                                                  || (y > n * j && cmask[k - map->size] < 0xff)
                                                  || (x > n * i && cmask[k - 1] < 0xff))) {
                                        in = 1;
                                        r->y = y;
                                        r->x0 = x;
                                        r->idx = np;
                                        r->flag = 0xff;
                                    }
                                } else if (!v) {
                                    in = 0;
                                    r->x1 = x - 1;
                                    if (r->x0 < r->x1) {
                                        np += r->x1 - r->x0 + 1;
                                        r++;
                                    }
                                }
                                n = map->n;
                                if (y < (j + 1) * n && x < (i + 1) * n && cmask[k] < 0xff)
                                    npoly += 2;
                            }
                            if (in) {
                                r->x1 = (i + 1) * map->n;
                                if (r->x0 < r->x1) {
                                    np += r->x1 - r->x0 + 1;
                                    r++;
                                }
                            }
                        }
                        r->y = -1;
                        if (t->wpoints && t->nwpoints != np) {
                            mem_Free(t->wpoints);
                            t->wpoints = 0;
                        }
                        t->nwpoints = np;
                        if (!t->wpoints)
                            t->wpoints = mem_Alloc(np * sizeof(D3FLPOINT), "d3_fl:Water(points)");
                        else
                            D3_ZERO(t->wpoints, np * sizeof(D3FLPOINT));
                        for (k = 0; k < t->nwpoints; k++) {
                            t->wpoints[k].src = 0;
                            t->wpoints[k].flags = 0;
                            t->wpoints[k].specular = 0x7fffffff;
                            t->wpoints[k].color = 0x7fffffff;
                        }
                        if (t->wpolys && t->nwpolys != npoly) {
                            mem_Free(t->wpolys);
                            t->wpolys = 0;
                        }
                        t->nwpolys = npoly;
                        if (!t->wpolys)
                            t->wpolys = mem_Alloc(npoly * sizeof(D3FLPOLY), "d3_fl:Water(polys)");
                        else
                            D3_ZERO(t->wpolys, npoly * sizeof(D3FLPOLY));
                        p = t->wpolys;
                        for (y = map->n * j; y < (j + 1) * map->n; y++)
                            for (x = map->n * i; x < (i + 1) * map->n; x++) {
                                k = (y & map->mask) * map->size + (x & map->mask);
                                if (cmask[k] < 0xff) {
                                    p->region = cmask[k];
                                    p->tex = 0;
                                    p->vOfs = 0;
                                    p->uOfs = 0;
                                    p->flags = 0;
                                    p->zOfs = 16.0f;
                                    p->v[0] = d3fl_WaterPoint(t, x, y, p->region);
                                    p->v[1] = d3fl_WaterPoint(t, x + 1, y + 1, p->region);
                                    p->v[2] = d3fl_WaterPoint(t, x, y + 1, p->region);
                                    p->uv = d3fl_UV[0];
                                    p++;
                                    p->tex = 0;
                                    p->vOfs = 0;
                                    p->uOfs = 0;
                                    p->flags = 0;
                                    p->region = cmask[k];
                                    p->zOfs = 16.0f;
                                    p->v[0] = d3fl_WaterPoint(t, x, y, p->region);
                                    p->v[1] = d3fl_WaterPoint(t, x + 1, y, p->region);
                                    p->v[2] = d3fl_WaterPoint(t, x + 1, y + 1, p->region);
                                    p->uv = d3fl_UV[1];
                                    p++;
                                }
                            }
                        r = t->wregions;
                        left = map->n * map->n / 2 * sizeof(D3FLWREG);
                        off = sizeof(D3FLWREG);
                        while (r->y != -1)
                            if (r->flag == -1) {
                                sprintf(msg, "PrepareFloorWater: removed region @ %i:[%i - %i] idx %i (hsize = %i)",
                                        r->y, r->x0, r->x1, r->idx, map->size);
                                memmove(r, r + 1, left - off);
                                left -= sizeof(D3FLWREG);
                            } else {
                                r++;
                                off += sizeof(D3FLWREG);
                            }
                    }
            mem_Free(cmask);
        } else {
            map->watertype = 0xff;
        }
        mem_Free(mask);
    }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            if (!map->tiles[j][i].water) {
                if (map->tiles[j][i].wregions) {
                    mem_Free(map->tiles[j][i].wregions);
                    map->tiles[j][i].wregions = 0;
                }
                if (map->tiles[j][i].wpoints) {
                    mem_Free(map->tiles[j][i].wpoints);
                    map->tiles[j][i].wpoints = 0;
                }
                if (map->tiles[j][i].wpolys) {
                    mem_Free(map->tiles[j][i].wpolys);
                    map->tiles[j][i].wpolys = 0;
                }
            }
    if (map->watertype == 0xff) {
        if (map->regions) {
            for (i = 0; i < map->nregions; i++)
                tex_Release((D3TEXHANDLE *)map->regions[i].tex);
            mem_Free(map->regions);
            map->regions = 0;
        }
        map->nregions = 0;
        if (map->water) {
            mem_Free(map->water);
            map->water = 0;
        }
    }
    map->dirty = 1;
}
/* 0x441270, 1 if all map points of the (n + 1)^2 square at x, y have the same tile type */

char d3fl_SameType(D3FLMAP *map, int x, int y, int n)
{
    int i, j;
    unsigned char t = 0xff;

    for (j = y; j <= y + n; j++)
        for (i = x; i <= x + n; i++) {
            int k = (i & map->mask) + (j & map->mask) * map->size;

            if (t == 0xff)
                t = map->types[k];
            else if (t != map->types[k])
                return 0;
        }
    return 1;
}
/* 0x441300, the divide maps: per map quad, which diagonal splits it (bit 7) and whether all its
   corners have the same tile type (bit 6); then the height range of each tile
   @deep B: code 497 / regs 2 / fill 1 - semantics verified in round 4 (level loops 0/1/2, d3fl_PrepareWater, tile height range; readable): the remaining code hunks are register allocation (map in ebp in the original, edi in ours), the stack slots, and the (abs <= abs) * 2 | SameType << 6 expression, where the original builds 2*x in a byte (add al,al) and shifts once after the OR while ours distributes the shift (shl al,7 / shl,6) - the same value. Tried byte/int temp, d |= ...; d <<= 6, * 64, +, ? 2 : 0, (x << 1 | f) << 6, casts (round-4 note): all same operation other spelling, needs vargen */

void d3fl_MakeDivide(D3FLMAP *map)
{
    unsigned char step[3] = { 1, 2, 4 };
    unsigned int mask = map->mask;
    int size = map->size;
    int x, y, l, s, n, i, j;
    unsigned int x1, y1;
    unsigned char *h;
    unsigned char mn, mx, v;
    int k, d; unsigned char c;

    for (y = 0; y < size; y++) {
        y1 = (y + 1) & mask;
        for (x = 0; x < size; x++) {
            h = map->height;
            x1 = (x + 1) & mask;
            map->divide[0][y * size + x] = ((abs(h[y * size + x] - h[y1 * size + x1]) <= abs(h[y * size + x1] - h[y1 * size + x])) * 2
                                            | d3fl_SameType(map, x, y, 1)) << 6;
        }
    }
    for (l = 1; l < 3; l++) {
        s = step[l];
        n = map->size / s;
        for (y = 0; y < n; y++) {
            y1 = (y * s + s) & mask;
            for (x = 0; x < n; x++) {
                size = map->size;
                x1 = (x * s + s) & mask;
                h = map->height;
                map->divide[l][y * n + x] = ((abs(h[(y * size + x) * s] - h[y1 * size + x1]) <= abs(h[y * s * size + x1] - h[y1 * size + x * s])) * 2
                                             | d3fl_SameType(map, x * s, y * s, s)) << 6;
            }
        }
    }
    d3fl_PrepareWater(map);
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            mx = 0;
            mn = 0xff;
            for (y = 0; y <= map->n; y++)
                for (x = 0; x <= map->n; x++) {
                    k = ((map->n * i + x) & mask) + ((map->n * j + y) & mask) * map->size;
                    v = map->height[k];
                    mx = max(mx, v);
                    mn = min(mn, v);
                    if (map->water && (v = map->water[k]) > 0) {
                        mx = max(mx, v);
                        mn = min(mn, v);
                    }
                }
            map->tiles[j][i].hmax = mx;
            map->tiles[j][i].hmin = mn;
            map->tiles[j][i].lod = -1;
        }
    map->dirty = 1;
}

typedef struct D3FLLIGHTCTX {
    D3FLMAP *map;
    D3OBJECT *light;
} D3FLLIGHTCTX;

/* 0x441660, d3_ForEachObject callback of d3fl_MakeLight: finds the sun (type 7) and marks the map
   points lit by the other lights (bit 7 of the light map) */
char d3fl_LightCB(D3OBJECT *obj, void *ctx)
{
    D3FLMAP *map;
    D3VECTOR4 pos;
    float h, r, d, t, q;
    int x, y, n, n2, xx, yy, x0, x1, y0, y1, k;

    if (obj->type == 7) {
        if ((obj->objectFlags & 0x10) && !((D3FLLIGHTCTX *)ctx)->light) {
            ((D3FLLIGHTCTX *)ctx)->light = obj;
            return 1;
        }
    } else if (D3_L(obj)->power > 0.0 && D3_L(obj)->range > 0.0) {
        map = ((D3FLLIGHTCTX *)ctx)->map;
        r = D3_L(obj)->range;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
        if (map_Pick(map, (D3VECTOR *)&pos, 0, &x, &y, &h)) {
            d = pos.y - h;
            t = r * r - d * d;
            if (t > 0.0) {
                q = (float)sqrt(t) / (d3fl_LightMargin + map->scale);
                k = map->size * y + x;
                if (q > 1.0) {
                    n = (int)q;
                    n2 = (int)(q * q + 0.5f);
                    y1 = min(map->size - 1, y + n);
                    y0 = max(0, y - n);
                    for (yy = y0; yy <= y1; yy++) {
                        x1 = min(map->size - 1, x + n);
                        x0 = max(0, x - n);
                        for (xx = x0; xx <= x1; xx++)
                            if ((xx - x) * (xx - x) + (yy - y) * (yy - y) < n2)
                                map->light[map->size * yy + xx] |= 0x80;
                    }
                } else
                    map->light[k] |= 0x80;
            }
        }
    }
    return 1;
}
/* 0x441860, the light map: bit 7 = lit by a point light (d3fl_LightCB), bits 0..6 = sun shade by the
   terrain normal; then bit 7 of the divide maps follows the light edges
   @deep B: code 357 / fpu 26 / slots 20 - the function was read (same size,frame 0x60 vs 0x68, the light part only differs in slot offsets / FPU operand order (the original loads map->negScale * hscale in the other order), the inlined d3fl_FtoI scheduling, and the divide-edge decision, where 288 + 144 group/order permutations were tried and none was closer than the kept five-group form (set {0111,1000}, clear {1101,1011,0100,0010,1001}, set {0001,1110,0110}, merged tails); no missing statement, wrong constant or field - needs vargen / listing */

void d3fl_MakeLight(D3FLMAP *map)
{
    D3FLLIGHTCTX ctx;
    D3VECTOR4 dir;
    D3VECTOR n;
    int size, x, y, s, dx, dz, iv, row;
    unsigned int l;
    unsigned char *h, *lp, *up, *down, *p, *drow;
    float kx, ky, kz, ky2, half, fx, fz, inv, d;
    int step[3];
    unsigned char a, b, c, e;

    if (!map->nolight && map->light) {
        d3fl_Invalidate(map);
        size = map->size;
        D3_ZERO(map->light, size * size);
        ctx.map = map;
        ctx.light = 0;
        d3_ForEachObject(d3_DefaultScene, 0, 0xe, d3fl_LightCB, &ctx);
        if (ctx.light) {
            half = D3_L(ctx.light)->power * 0.5f;
            map->ambient[0] = D3_L(ctx.light)->color.x * (1.0f / 255.0f);
            map->ambient[1] = D3_L(ctx.light)->color.y * (1.0f / 255.0f);
            map->ambient[2] = D3_L(ctx.light)->color.z * (1.0f / 255.0f);
            d3_DirToWorldD(ctx.light, &d3_ZAxis, (D3VECTOR *)&dir);
            lp = map->light;
            h = map->height;
            kx = map->negScale * map->hscale * 2.0f;
            ky = map->negScale * map->scale * -4.0f;
            kz = map->hscale * map->scale * 2.0f;
            for (y = 0, row = 0; y < size; y++, row += size) {
                ky2 = ky * ky;
                up = h + ((y - 1) & (size - 1)) * size;
                down = h + ((y + 1) & (size - 1)) * size;
                for (x = 0; x < size; x++) {
                    dx = h[row + ((x + 1) & (size - 1))] - h[row + ((x - 1) & (size - 1))];
                    fx = dx * kx;
                    dz = down[x] - up[x];
                    fz = dz * kz;
                    inv = 1.0f / (float)sqrt(fx * fx + fz * fz + ky2);
                    n.x = inv * fx;
                    n.y = ky * inv;
                    n.z = inv * fz;
                    d = n.x * dir.x + n.y * dir.y + n.z * dir.z;
                    if (d < 0.0f) {
                        d3fl_FtoI(&iv, half * d3fl_Shade[(int)(d * -1023.0)]);
                        lp[row + x] |= min((unsigned int)iv, 0x7f);
                    }
                }
            }
        } else {
            map->ambient[0] = 255.0f;
            map->ambient[1] = 255.0f;
            map->ambient[2] = 255.0f;
        }
        step[0] = 1;
        step[1] = 2;
        step[2] = 4;
        for (l = 0; l < 3; l++) {
            s = step[l];
            size = map->size / s;
            drow = map->divide[l];
            for (y = 0; y < size - 1; y++, drow += size) {
                p = drow;
                for (x = 0; x < size - 1; x++, p++) {
                    int k = (map->size * y + x) * s;

                    a = map->light[k] >> 7;
                    b = map->light[k + s] >> 7;
                    c = map->light[k + map->size * s] >> 7;
                    e = map->light[k + (map->size + 1) * s] >> 7;
                    if ((!a && b && c && e) || (a && !b && !c && !e))
                        *p |= 0x80;
                    else if ((a && b && !c && e) || (a && !b && c && e) || (!a && b && !c && !e) || (!a && !b && c && !e) || (a && !b && !c && e))
                        *p &= 0x7f;
                    else if ((!a && !b && !c && e) || (a && b && c && !e) || (!a && b && c && !e))
                        *p |= 0x80;
                }
            }
        }
    }
}
/* 0x441c50, allocates and resets the point and polygon buffers of a tile */

void d3fl_InitTile(D3FLTILE *t, unsigned int n, unsigned char shift)
{
    unsigned int i;

    n >>= shift;
    t->lodLevel = 0xff;
    t->lodDelay = 0;
    t->nextLod = 0;
    t->npolys = 0;
    t->npoints = 0;
    if (!t->points)
        t->points = mem_Alloc((n + 2) * (n + 2) * sizeof(D3FLPOINT), "d3_fl:TilePoints");
    if (!t->polys)
        t->polys = mem_Alloc((n + 1) * (n + 1) * 2 * sizeof(D3FLPOLY), "d3_fl:TilePolys");
    if (!t->split)
        t->split = mem_Alloc((n * 3 + 6) * 0x10, "d3_fl:TileSplitUpdate");
    if (!t->bp) {
        t->nbp = n * 8 + 0x10;
        t->bp = mem_Alloc((t->nbp + (n + 1) * (n + 1) * 2) * 0x18, "d3_fl:TileBPBuffer");
    }
    if (!t->wregions) {
        t->water = 0;
        t->wpoints = 0;
        t->wpolys = 0;
        t->nwpolys = 0;
        t->nwpoints = 0;
    }
    for (i = 0; i < (n + 2) * (n + 2); i++) {
        t->points[i].src = 0;
        t->points[i].flags = 0;
        t->points[i].specular = D3FL_WHITE;
        t->points[i].color = D3FL_WHITE;
    }
    for (i = 0; i < (n + 1) * (n + 1) * 2; i++) {
        t->polys[i].zOfs = 16.0f;
        t->polys[i].vOfs = 0;
        t->polys[i].uOfs = 0;
        t->polys[i].flags = 0;
        t->polys[i].v[0] = 0;
        t->polys[i].v[1] = 0;
        t->polys[i].v[2] = 0;
        t->polys[i].uv = 0;
        t->polys[i].tex = 0;
    }
}
/* 0x441db0, frees the light, divide and tile buffers of a map */

void d3fl_FreeTiles(D3FLMAP *map)
{
    int i, j;

    if (map->light) {
        mem_Free(map->light);
        map->light = 0;
    }
    for (i = 0; i < 3; i++)
        if (map->divide[i]) {
            mem_Free(map->divide[i]);
            map->divide[i] = 0;
        }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            map->tiles[j][i].lod = 0;
            map->tiles[j][i].lodLevel = 0;
            map->tiles[j][i].npolys = 0;
            map->tiles[j][i].npoints = 0;
            if (map->tiles[j][i].points) {
                mem_Free(map->tiles[j][i].points);
                map->tiles[j][i].points = 0;
            }
            if (map->tiles[j][i].polys) {
                mem_Free(map->tiles[j][i].polys);
                map->tiles[j][i].polys = 0;
            }
            if (map->tiles[j][i].split) {
                mem_Free(map->tiles[j][i].split);
                map->tiles[j][i].split = 0;
            }
            if (map->tiles[j][i].bp) {
                mem_Free(map->tiles[j][i].bp);
                map->tiles[j][i].bp = 0;
            }
            if (map->tiles[j][i].water) {
                mem_Free(map->tiles[j][i].wregions);
                map->tiles[j][i].wregions = 0;
                mem_Free(map->tiles[j][i].wpoints);
                map->tiles[j][i].wpoints = 0;
                mem_Free(map->tiles[j][i].wpolys);
                map->tiles[j][i].wpolys = 0;
            }
        }
}
/* 0x441ea0, allocates the light and divide maps and the tiles, then computes the divide maps */

void d3fl_Inflate(D3FLMAP *map)
{
    int i, j;

    if (!map->light)
        map->light = mem_Alloc(map->size * map->size + 1, "d3fl:Inflate(f->light)");
    for (i = 0; i < 3; i++)
        if (!map->divide[i]) {
            int n = map->size >> i;

            map->divide[i] = mem_Alloc(n * n + 1, "d3fl:Inflate(f->divide)");
        }
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            d3fl_InitTile(&map->tiles[j][i], map->n, map->shift);
    d3fl_MakeDivide(map);
    map->dirty = 1;
}

/* ---- 0x441f50..0x4439e0 (worker map) ---- */

/* 0x441f50, allocates the divide and light maps of a freshly loaded map and resets its tiles */
void d3fl_InitMap(D3FLMAP *map)
{
    int i, j;

    map->watertype = 0xff;
    map->divide[0] = mem_Alloc(map->size * map->size, "d3_fl:Divide0");
    map->divide[1] = mem_Alloc((unsigned int)(map->size * map->size) >> 2, "d3_fl:Divide1");
    map->divide[2] = mem_Alloc((unsigned int)(map->size * map->size) >> 4, "d3_fl:Divide2");
    map->light = mem_Alloc(map->size * map->size, "d3_fl:Light");
    if (!map->lightofs)
        map->lightofs = mem_Alloc(map->size * map->size * 4, "d3_fl:LightOffset");
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++) {
            D3_ZERO(&map->tiles[j][i], sizeof(D3FLTILE));
            d3fl_InitTile(&map->tiles[j][i], map->n, map->shift);
        }
    d3fl_MakeDivide(map);
    d3fl_MakeLight(map);
    map->dirty = 1;
}
/* 0x442080, loads the source textures (3 mip levels) of tile type idx (< 0: the first free one) */

int d3fl_LoadTexture(D3FLMAP *map, char *name, int idx)
{
    char buf[256];
    char *path;
    int i, k;

    if (idx < 0)
        for (k = 0; map->typenames[k][0]; k++)
            ;
    else
        k = idx;
    if (k < 8) {
        map->typenames[k][0] = ch_EmptyName[0];
        for (i = 0; i < 3; i++)
            map->src[i][k] = 0;
        if (tex_FindFile(name)) {
            strncpy(map->typenames[k], name, 0x40);
            for (i = 0; i < 3; i++) {
                strcpy(buf, name);
                strcat(buf, d3fl_Suffix[i]);
                if (path = tex_FindFile(buf)) {
                    map->src[i][k] = mem_Alloc(0x4000, "d3_fl:fltx_SourceData");
                    bmp_Load(path, map->src[i][k], 0, 0x11);
                } else
                    strcat(buf, " not found (FloorTexture)!");
            }
            return k;
        }
        sprintf(buf, "Floor-Texture \"%s\" not found!", name);
    }
    return -1;
}
/* 0x442210, replaces the texture of tile type idx */

int d3fl_ReplaceTexture(D3FLMAP *map, char *name, int idx)
{
    int i;

    map->typenames[idx][0] = ch_EmptyName[0];
    for (i = 0; i < 3; i++)
        if (map->src[i][idx]) {
            mem_Free(map->src[i][idx]);
            map->src[i][idx] = 0;
        }
    i = d3fl_LoadTexture(map, name, idx);
    if (i >= 0) {
        d3fl_Flush();
        d3fl_Invalidate(map);
    }
    return i;
}
/* 0x442280, reloads the source textures of all tile types (d3fl_ReloadTextures in d3.h) */

void d3fl_ReloadTextures(D3FLMAP *map)
{
    char buf[256];
    char *path;
    int i, j;

    if (map) {
        d3fl_Invalidate(map);
        for (i = 0; i < 8; i++)
            for (j = 0; j < 3; j++) {
                strcpy(buf, map->typenames[i]);
                strcat(buf, d3fl_Suffix[j]);
                if ((path = tex_FindFile(buf)) && map->src[j][i])
                    bmp_Load(path, map->src[j][i], 0, 0x11);
            }
    }
}

#define D3FL_CORNER(i, X0, Z0, H)                                    \
    p.x = d3fl_AxisX.x * (X0) + d3fl_Origin.x;                       \
    p.y = d3fl_AxisX.y * (X0) + d3fl_Origin.y;                       \
    p.z = d3fl_AxisX.z * (X0) + d3fl_Origin.z;                       \
    q.x = d3fl_AxisZ.x * (Z0) + p.x;                                 \
    q.y = d3fl_AxisZ.y * (Z0) + p.y;                                 \
    q.z = d3fl_AxisZ.z * (Z0) + p.z;                                 \
    d3fl_Box[i].x = d3fl_AxisY.x * (H) + q.x;                        \
    d3fl_Box[i].y = d3fl_AxisY.y * (H) + q.y;                        \
    d3fl_Box[i].z = d3fl_AxisY.z * (H) + q.z;

/* 0x442ac0, the bounding box of tile (tx, ty) in world space (bottom from t->hmin, top from the map's tile) */
D3BOXVERT *d3fl_TileBox(D3FLMAP *map, D3FLTILE *t, int tx, int ty)
{
    D3VECTOR4 p, q;
    float x0, x1, z0, z1, h;

    x0 = (float)(map->n * tx);
    x1 = (float)((tx + 1) * map->n);
    z0 = (float)(map->n * ty);
    z1 = (float)((ty + 1) * map->n);
    h = t->hmin;
    D3FL_CORNER(0, x0, z0, h)
    D3FL_CORNER(1, x1, z0, h)
    D3FL_CORNER(2, x1, z1, h)
    D3FL_CORNER(3, x0, z1, h)
    h = map->tiles[ty][tx].hmax;
    D3FL_CORNER(4, x0, z0, h)
    D3FL_CORNER(5, x1, z0, h)
    D3FL_CORNER(6, x1, z1, h)
    D3FL_CORNER(7, x0, z1, h)
    return d3fl_Box;
}
/* 0x4437f0, culls the tiles against the view and picks their detail levels; returns 1 if any level changed @deep B: read the whole 112-line asmdiff against 0x4437f0..0x4439e0 and diffclass: what is left is the local layout - the original initialises its locals in another order and keeps map in eax where ours uses edi, computes the first loop's tile addresses as (y*8+x)*25 chains of lea where the original walks a tile pointer (add esi,0x64), and assigns x/y/i/n/sum to other stack slots; the tail is the misaligned disassembly of the shorter function. Meaning fixes applied while reading: the unsigned guards are spelled > 0 (t->lodLevel, the TileLod result, k, y) which reproduces the original's jbe; no other statement/constant/callee differs. Needs vargen/try.py */

char d3fl_UpdateTiles(D3FLMAP *map)
{
    D3FLTILE *t;
    D3BOXVERT *box;
    unsigned int x, y, k;
    int i, n = 0;
    char changed = 0;
    float sum = 0.0f;
    D3VECTOR4 c;
    unsigned char lod;

    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++) {
            box = d3fl_TileBox(map, &map->tiles[y][x], x, y);
            if ((map->tiles[y][x].clip = d3_ClipBox(box, &map->tiles[y][x].zmin, &map->tiles[y][x].zmax)) & 0x40)
                map->tiles[y][x].lodLevel = 0;
            else {
                map->tiles[y][x].lodLevel = 1;
                map->tiles[y][x].zmin += 16.0f;
                map->tiles[y][x].zmax += 16.0f;
                c.x = box[0].x;
                c.y = box[0].y;
                c.z = box[0].z;
                for (i = 1; i < 8; i++) {
                    c.x += box[i].x;
                    c.y += box[i].y;
                    c.z += box[i].z;
                }
                n++;
                c.x *= 0.125f;
                c.y *= 0.125f;
                c.z *= 0.125f;
                map->tiles[y][x].center.x = c.x;
                map->tiles[y][x].center.y = c.y;
                map->tiles[y][x].center.z = c.z;
                sum += map->tiles[y][x].centerDist = (float)sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
            }
        }
    for (y = 0; y < 8; y++)
        for (k = 0; k < 8; k++) {
            t = &map->tiles[y][k];
            if (t->lodLevel > 0 && (t->lodLevel = lod = d3fl_TileLod(map, t, sum, n)) > 0) {
                if (k > 0 && (lod == 4 && t[-1].lodLevel == 1 || lod == 1 && t[-1].lodLevel == 4))
                    t->lodLevel = 2;
                if (y > 0 && (t->lodLevel == 4 && t[-8].lodLevel == 1 || t->lodLevel == 1 && t[-8].lodLevel == 4))
                    t->lodLevel = 2;
                t->lodLevel = max(1 << map->shift, t->lodLevel);
            }
            if (t->lodLevel != (unsigned char)t->lod)
                changed = 1;
        }
    return changed;
}
/* 0x442390, creates a terrain map from a description: height, water and texture maps either from memory or from
   "boden/<name>.bmp", "boden/<name>_w.bmp" and "boden/<texname>.bmp"
   @deep B: 6 code hunks, each the same operations in another spelling, read against the original disassembly (0x442390..0x442ac0), Ghidra output and the source: (1) 0x442582 idxMask/mask: same size*size-1|size-1, the original reuses eax and lea where ours reloads map->size for mask and uses dec; (2) 0x4425c4 water mem_Alloc+D3FL_COPY: same size*size computation and desc->water copy, scheduled on the other side of the push/call (ecx vs edx); (3) 0x442944 transform: same map->n=size/8 division and (float)-size*scale*0.5f, integer division and FPU multiplies interleaved differently, map->size loaded twice vs once+copy; (4) 0x44299b and (5) 0x4429d3: the same scale/hscale field copies, mov vs fld/fstp on the opposite statement each; (6) 0x442a13 typenames loop: mov al vs mov cl, a register rename only; tail = fill nops. No missing statement, constant, callee, condition or field; readability fixed (Q2 idxMask) */

D3FLMAP *d3fl_LoadMap(D3FLDESC *desc)
{
    char path[256];
    char msg[256];
    char dir[256];
    D3FLMAP *map;
    char *p;
    int w, h;
    D3MATRIX *m;
    int i, n, mn, mx, v;

    map = mem_Alloc(sizeof(D3FLMAP), "d3_fl:Floor");
    strcpy(map->name, desc->name);
    strcpy(map->texname, desc->texname);
    map->shift = d3fl_MapShift;
    if (desc->height && (n = desc->size) > 0) {
        map->size = n;
        map->height = mem_Alloc(n * n, "d3_fl:Height");
        D3FL_COPY(map->height, desc->height, map->size * map->size);
    } else {
        vfs_DirPath(d3io_StockDir, vfs_Root, path);
        strcat(path, "boden/");
        strcpy(dir, path);
        strcat(path, desc->name);
        strcat(path, ".bmp");
        if (!bmp_GetInfo(path, &w, &h, 0) || w != h)
            sprintf(msg, "Could not open HeightMap-File %s or HeightMap has wrong size!!!", path);
        else {
            map->size = w;
            map->height = mem_Alloc(w * w, "d3_fl:Height");
            bmp_Load(path, map->height, 0, 5);
        }
    }
    map->idxMask = map->size * map->size - 1 | map->size - 1;
    map->mask = map->size - 1;
    if (desc->nregions > 0 && desc->regions) {
        if (desc->water && desc->size > 0) {
            map->water = mem_Alloc(map->size * map->size, "d3_fl:Water(height)");
            D3FL_COPY(map->water, desc->water, map->size * map->size);
        } else {
            strcpy(path, dir);
            strcat(path, desc->name);
            strcat(path, "_w.bmp");
            if (!bmp_GetInfo(path, &w, &h, 0) || w != h || w != map->size)
                sprintf(msg, "Could not open WaterMap-File %s or WaterMap has wrong size!!!", path);
            else {
                map->water = mem_Alloc(map->size * map->size, "d3_fl:Water(height)");
                bmp_Load(path, map->water, 0, 5);
            }
        }
        map->nregions = desc->nregions;
        map->regions = mem_Alloc(map->nregions * sizeof(D3FLREGION), "d3_fl:Water(regions)");
        D3FL_COPY(map->regions, desc->regions, map->nregions * sizeof(D3FLREGION));
    }
    if (desc->types && desc->size > 0) {
        map->types = mem_Alloc(map->size * map->size, "d3_fl:Texture");
        D3FL_COPY(map->types, desc->types, map->size * map->size);
    } else {
        strcpy(path, dir);
        strcat(path, desc->texname);
        strcat(path, ".bmp");
        if (!bmp_GetInfo(path, &w, &h, 0) || w != h || w != map->size)
            sprintf(msg, "Could not open Texture-Information-File %s or Texture-Information has wrong size!!!", path);
        else {
            map->types = mem_Alloc(map->size * map->size, "d3_fl:Texture");
            bmp_Load(path, map->types, 0, 5);
        }
    }
    if (desc->lightofs && desc->size > 0) {
        map->lightofs = mem_Alloc(map->size * map->size * 4, "d3_fl:LightOffset");
        D3FL_COPY(map->lightofs, desc->lightofs, map->size * map->size * 4);
    }
    if (map->types && map->height) {
        map->n = map->size / 8;
        m = (D3MATRIX *)&map->orgx;
        m->_11 = (float)-map->size * desc->scale * 0.5f;
        m->_12 = desc->hscale * -64.0f;
        m->_13 = (float)map->size * desc->scale * 0.5f;
        m->_21 = desc->scale;
        m->_22 = 0.0f;
        m->_23 = 0.0f;
        m->_31 = 0.0f;
        m->_32 = 0.0f;
        m->_33 = -desc->scale;
        m->_41 = 0.0f;
        m->_42 = desc->hscale;
        m->_43 = 0.0f;
        map->lodDist2 = 20.0f;
        map->lodDist4 = 40.0f;
        map->nolight = 0;
        map->lod = 0;
        for (i = 0; i < 8; i++) {
            map->typenames[i][0] = ch_EmptyName[0];
            p = desc->typenames[i];
            if (p && *p)
                d3fl_LoadTexture(map, p, -1);
        }
        n = map->size * map->size;
        mn = 0xff;
        mx = 0;
        for (i = 0; i < n; i++) {
            v = map->types[i];
            mx = max(mx, v);
            mn = min(mn, v);
        }
        for (i = 0; i < map->size * map->size; i++)
            map->types[i] -= mn;
        d3fl_InitMap(map);
        return map;
    }
    d3fl_FreeMap(map);
    return 0;
}
/* 0x442ed0, animates n water regions: texture frame, texture offsets and the 16 wave vectors */

void d3fl_UpdateWater(unsigned long time, unsigned int n, D3FLREGION *r)
{
    unsigned int k;
    int i, dt;
    float t, *w;
    D3VECTOR4 v;
    double d;

    for (k = 0; k < n; k++, r++) {
        if (r->tex && r->tex->frames > 0 && r->tex->speed > 0)
            r->tex2 = tex_GetFrameF((D3TEXTURE *)r->tex - (D3TEXTURE *)d3_Textures,
                                   time / d3_AnimDiv[r->tex->speed] % r->tex->frames);
        dt = time - r->time;
        if (dt > 0) {
            t = (float)dt;
            r->uOfs = (float)fmod(t * r->scrollSpeedU + r->uOfs, 1.0);
            r->vOfs = (float)fmod(t * r->scrollSpeedV + r->vOfs, 1.0);
            for (i = 0; i < 4; i++)
                r->wavePhase[i] = (float)fmod(t * r->speed[i] + r->wavePhase[i], D3FL_2PI);
            for (i = 0; i < 16; i++) {
                w = r->wave[i >> 2][i & 3];
                v.x = (float)sin(cos(2.7 * i) * D3FL_2PI + r->wavePhase[0] + i);
                v.y = (float)cos(sin(3.1415927f - 2.4 * i) * D3FL_2PI + r->wavePhase[1] + i);
                v.z = (float)sin(i - cos(2.5 * i) * D3FL_2PI + r->wavePhase[2] + 2.2);
                d = cos(i - sin(2.6 * i + 3.1415927f) * D3FL_2PI + r->wavePhase[3] + 4.0);
                w[0] = v.x * r->amp[0];
                w[1] = v.y * r->amp[1];
                w[2] = v.z * r->amp[2];
                w[3] = (float)(d * r->amp[3]);
            }
            r->time = time;
        }
    }
}
/* 0x4430c0, transforms, lights and projects the water points of a tile and queues its water polygons @deep read the whole function against the original 0x4430c0..0x4437e1 (setup, the region/point loop with wave, clamp and lighting, d3_ClipMesh, both projection loops, the counters, the sort queue) and fixed: sun = AxisY * f, then sun += wave as two statement groups (the original stores the products and adds the wave after), the clamp min(255.0f, max(0.0f, h + w)) (the original loads the constant first: fld 0 / fcomp st(1), fld 255 / fcompp), the lit colours L * f + SunCol + Ambient (VC6 swaps the last two addends: the original adds Ambient then SunCol), the poly test flags & 0x80 (test / jns). ours 494 vs 500 instructions, the lighting block now equal but for the iv / temp slots. Left: allocation and loop shape: the original walks the strip records through ebp = r + 8 ([ebp-4] y, [ebp] x0, [ebp+8] flag) with a jmp into the rotated loop ('+ lea ebp,[eax+8] / + jmp 0x443195'), ours r in esi; the wave index as lea [x&3 + ym*4] << 4 per point where ours keeps ym*4 in a slot ('- and ecx,3 / - add ecx,ebx' vs '+ lea ecx,[eax+edx*4]'); map / t in ebx / esi swapped (regs ebx->esi); the counters d3_TriCount / d3_VertCount updated in another order and npolys / npoints in other registers; the sort loop keeps d3_SortPtr in edi; the fog distance products in another x87 stack order. */

void d3fl_DrawWater(D3FLMAP *map, D3FLTILE *t)
{
    D3VECTOR4 sun;
    D3VECTOR4 p;
    D3FLWREG *r;
    D3FLREGION *reg;
    D3PVERT *v;
    D3FLPOLY *poly;
    D3CLIPINFO *clip;
    float *wv;
    float k, fx, h, f;
    unsigned int mask;
    int size, row, ym, x, idx, iv, npoints, npolys, i;
    unsigned char *water, *light;
    unsigned char a, l;

    if (map && t && t->lodLevel && t->water) {
        sun.x = d3fl_AxisY.x;
        sun.y = d3fl_AxisY.y;
        sun.z = d3fl_AxisY.z;
        d3_Normalize(&sun);
        size = map->size;
        water = map->water;
        mask = map->mask;
        npoints = t->nwpoints;
        v = (D3PVERT *)t->wpoints;
        light = map->light;
        npolys = t->nwpolys;
        k = 0.5f - (float)fabs(d3_ZAxis.x * sun.x + d3_ZAxis.y * sun.y + d3_ZAxis.z * sun.z);
        for (r = t->wregions; r->y >= 0; r++) {
            ym = r->y & 3;
            row = (r->y & mask) * size;
            reg = &map->regions[(unsigned char)r->flag];
            p.x = d3fl_AxisZ.x * r->y + d3fl_Origin.x;
            p.y = d3fl_AxisZ.y * r->y + d3fl_Origin.y;
            p.z = d3fl_AxisZ.z * r->y + d3fl_Origin.z;
            fx = (float)r->x0;
            p.x = d3fl_AxisX.x * fx + p.x;
            p.y = d3fl_AxisX.y * fx + p.y;
            p.z = d3fl_AxisX.z * fx + p.z;
            h = k * reg->f14 + reg->flags.b[0];
            for (x = r->x0; x <= r->x1; x++, v++) {
                idx = (x & mask) + row;
                f = water[idx];
                wv = reg->wave[ym][x & 3];
                sun.x = d3fl_AxisY.x * f;
                sun.y = d3fl_AxisY.y * f;
                sun.z = d3fl_AxisY.z * f;
                sun.x += wv[0];
                sun.y += wv[1];
                sun.z += wv[2];
                v->x = p.x + sun.x;
                v->y = p.y + sun.y;
                v->z = p.z + sun.z;
                a = (unsigned char)min(255.0f, max(0.0f, h + wv[3]));
                if (reg->flags.b[2] & 1)
                    v->color = (a << 8 | a) << 8 | a;
                else {
                    l = light[idx];
                    f = (float)(l & 0x7f);
                    f += f;
                    if (l & 0x80) {
                        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_SunCol.x + d3fl_Ambient.x);
                        v->c.r = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_SunCol.y + d3fl_Ambient.y);
                        v->c.g = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_SunCol.z + d3fl_Ambient.z);
                    } else {
                        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_Ambient.x);
                        v->c.r = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_Ambient.y);
                        v->c.g = min(0xff, iv);
                        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_Ambient.z);
                    }
                    v->c.b = min(0xff, iv);
                    v->c.a = a;
                    v->specular = v->color;
                }
                p.x = d3fl_AxisX.x + p.x;
                p.y = d3fl_AxisX.y + p.y;
                p.z = d3fl_AxisX.z + p.z;
            }
        }
        d3_ClipMesh(t->clipCode, (D3PVERT *)t->wpoints, npoints, (D3PTRI *)t->wpolys, npolys);
        clip = (D3CLIPINFO *)d3_ClipInfo[t->clip & 0x3f];
        if (d3_FogOn) {
            for (i = npoints, v = (D3PVERT *)t->wpoints; i > 0; i--, v++)
                if (v->flags & 0x80) {
                    k = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * k + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * k + d3_CenterY;
                    v->rhw = k;
                    d3_Dist2 = v->x * v->x + v->y * v->y + v->z * v->z;
                    v->fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                        ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                }
        } else {
            for (i = npoints, v = (D3PVERT *)t->wpoints; i > 0; i--, v++)
                if (v->flags & 0x80) {
                    k = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * k + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * k + d3_CenterY;
                    v->rhw = k;
                }
        }
        d3_TriCount += npolys;
        d3_DrawnVerts += npoints;
        d3_VertCount += npoints;
        npolys = min(npolys, (int)(d3_SortMax - d3_SortCount));
        for (i = npolys, poly = t->wpolys; i > 0; i--, poly++)
            if (poly->flags & 0x80) {
                reg = &map->regions[poly->region];
                d3_SortPtr->item = poly;
                if (reg->tex2) {
                    d3_SortPtr->key = ((D3TEXTURE *)reg->tex2 - (D3TEXTURE *)d3_Textures) + 1;
                    reg->tex2->stamp = d3_FrameCount;
                } else
                    d3_SortPtr->key = 0;
                poly->tex = reg->tex2;
                poly->clip = clip;
                poly->uOfs = reg->uOfs;
                poly->vOfs = reg->vOfs;
                d3_SortPtr++;
                d3_SortCount++;
            }
    }
}

/* ---- 0x4439e0..0x446990 (worker d3fl3, d3fl4) ---- */

/* light map value l (bit 7: lit by a point light, bits 0..6: sun shade) to the vertex colors of v */
#define D3FL_LIGHT(v, l)                                                        \
    f = (float)((l) & 0x7f);                                                    \
    f += f;                                                                     \
    if ((l) & 0x80) {                                                           \
        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_SunCol.x + d3fl_Ambient.x);   \
        (v)->c.r = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_SunCol.y + d3fl_Ambient.y);   \
        (v)->c.g = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_SunCol.z + d3fl_Ambient.z);   \
    } else {                                                                    \
        d3fl_FtoI(&iv, d3fl_LightCol.x * f + d3fl_Ambient.x);                   \
        (v)->c.r = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.y * f + d3fl_Ambient.y);                   \
        (v)->c.g = min(0xff, iv);                                               \
        d3fl_FtoI(&iv, d3fl_LightCol.z * f + d3fl_Ambient.z);                   \
    }                                                                           \
    (v)->c.b = min(0xff, iv);                                                   \
    (v)->specular = (v)->color;

/* camera-space position of map point (X0, Z0) at height 0, as the tile vertices: the x step (a), plus the
   origin, plus the z step (q) */
#define D3FL_POS(p, X0, Z0)                                                     \
    a.x = d3fl_AxisX.x * (X0);                                                  \
    a.y = d3fl_AxisX.y * (X0);                                                  \
    a.z = d3fl_AxisX.z * (X0);                                                  \
    p.x = d3fl_Origin.x + a.x;                                                  \
    p.y = d3fl_Origin.y + a.y;                                                  \
    p.z = d3fl_Origin.z + a.z;                                                  \
    q.x = d3fl_AxisZ.x * (Z0);                                                  \
    q.y = d3fl_AxisZ.y * (Z0);                                                  \
    q.z = d3fl_AxisZ.z * (Z0);                                                  \
    p.x = q.x + p.x;                                                            \
    p.y = q.y + p.y;                                                            \
    p.z = q.z + p.z;

#define T map->tiles[y][x]

/* 0x4439e0, draws the floor: transforms the map into camera space; if the map is dirty, rebuilds the tile meshes
   (vertices, lit from the light map, and triangles; extra vertices on edges next to finer tiles), projects them and
   culls the triangles; then animates the water (water != 0) and queues the visible triangles of all tiles,
   rendering their cache textures on first use */
void d3fl_Draw(D3FLMAP *map, char water)
{
    D3OBJECT *cam;
    D3FLTILE *t;
    D3PVERT *v, *v0, *v1, *v2;
    D3FLPOLY *poly, *np, *pp;
    D3FLSPLIT *sp;
    D3FLTEX *tex;
    D3FLCACHE *c;
    D3CLIPINFO *clip;
    D3VECTOR4 a, q, r, p, dx, dz;
    float *uv;
    float dx0, dy0, dz0, fx, fy, fs, h, f, u, w, k;
    unsigned int n, size, mask, x, y, i, j, s, half, nx, ny, mx, my, lx, ly, idx, npts, npolys;
    unsigned char *light, *types, *height, *div;
    int iv, jit;
    char changed, d, fl;
    void *surf;

    if (d3_Hardware && d3_Camera) {
        n = map->n;
        size = map->size;
        light = map->light;
        types = map->types;
        height = map->height;
        mask = map->idxMask;
        if (map->nolight) {
            d3fl_Ambient.x = 0.0f;
            d3fl_Ambient.y = 0.0f;
            d3fl_Ambient.z = 0.0f;
            d3fl_SunCol.x = 0.0f;
            d3fl_SunCol.y = 0.0f;
            d3fl_SunCol.z = 0.0f;
            d3fl_LightCol.x = 1.0f;
            d3fl_LightCol.y = 1.0f;
            d3fl_LightCol.z = 1.0f;
        } else {
            d3fl_Ambient.x = d3_AmbientColor.x;
            d3fl_Ambient.y = d3_AmbientColor.y;
            d3fl_Ambient.z = d3_AmbientColor.z;
            d3fl_SunCol.x = d3_SunColor.x;
            d3fl_SunCol.y = d3_SunColor.y;
            d3fl_SunCol.z = d3_SunColor.z;
            d3fl_LightCol.x = map->ambient[0];
            d3fl_LightCol.y = map->ambient[1];
            d3fl_LightCol.z = map->ambient[2];
        }
        cam = d3_Camera;
        dx0 = map->orgx - cam->pos.x;
        dy0 = map->orgy - cam->pos.y;
        dz0 = map->orgz - cam->pos.z;
        d3fl_Origin.x = dx0 * cam->matrix._11 + dy0 * cam->matrix._21 + dz0 * cam->matrix._31;
        d3fl_Origin.y = dx0 * cam->matrix._12 + dy0 * cam->matrix._22 + dz0 * cam->matrix._32;
        d3fl_Origin.z = dx0 * cam->matrix._13 + dy0 * cam->matrix._23 + dz0 * cam->matrix._33;
        d3fl_AxisX.x = map->scale * cam->matrix._11 + map->xAxisY * cam->matrix._21 + map->xAxisZ * cam->matrix._31;
        d3fl_AxisX.y = map->scale * cam->matrix._12 + map->xAxisY * cam->matrix._22 + map->xAxisZ * cam->matrix._32;
        d3fl_AxisX.z = map->scale * cam->matrix._13 + map->xAxisY * cam->matrix._23 + map->xAxisZ * cam->matrix._33;
        d3fl_AxisZ.x = map->zAxisX * cam->matrix._11 + map->zAxisY * cam->matrix._21 + map->negScale * cam->matrix._31;
        d3fl_AxisZ.y = map->zAxisX * cam->matrix._12 + map->zAxisY * cam->matrix._22 + map->negScale * cam->matrix._32;
        d3fl_AxisZ.z = map->zAxisX * cam->matrix._13 + map->zAxisY * cam->matrix._23 + map->negScale * cam->matrix._33;
        d3fl_AxisY.x = map->yAxisX * cam->matrix._11 + map->hscale * cam->matrix._21 + map->yAxisZ * cam->matrix._31;
        d3fl_AxisY.y = map->yAxisX * cam->matrix._12 + map->hscale * cam->matrix._22 + map->yAxisZ * cam->matrix._32;
        d3fl_AxisY.z = map->yAxisX * cam->matrix._13 + map->hscale * cam->matrix._23 + map->yAxisZ * cam->matrix._33;
        if (map->dirty) {
            changed = d3fl_UpdateTiles(map);
            /* vertices and triangles */
            for (y = 0; y < 8; y++)
                for (x = 0; x < 8; x++) {
                    mx = x * n;
                    my = y * n;
                    t = &map->tiles[y][x];
                    if (!(s = t->lodLevel)) {
                        t->npolys = 0;
                        t->npoints = 0;
                    } else {
                        fx = (float)mx;
                        a.x = d3fl_AxisX.x * fx;
                        a.y = d3fl_AxisX.y * fx;
                        a.z = d3fl_AxisX.z * fx;
                        half = s >> 1;
                        fy = (float)my;
                        q.x = d3fl_AxisZ.x * fy;
                        q.y = d3fl_AxisZ.y * fy;
                        q.z = d3fl_AxisZ.z * fy;
                        a.x = q.x + a.x;
                        a.y = q.y + a.y;
                        a.z = q.z + a.z;
                        r.x = d3fl_Origin.x + a.x;
                        r.y = d3fl_Origin.y + a.y;
                        r.z = d3fl_Origin.z + a.z;
                        nx = ny = n / s + 1;
                        if (x == 7)
                            nx -= 4 / s;
                        if (y == 7)
                            ny -= 4 / s;
                        if (t->lodLevel != (unsigned char)t->lod || changed)
                            t->npoints = ny * nx;
                        fs = (float)s;
                        dx.x = d3fl_AxisX.x * fs;
                        dx.y = d3fl_AxisX.y * fs;
                        dx.z = d3fl_AxisX.z * fs;
                        dz.x = d3fl_AxisZ.x * fs;
                        dz.y = d3fl_AxisZ.y * fs;
                        dz.z = d3fl_AxisZ.z * fs;
                        v = (D3PVERT *)t->points;
                        lx = (my * size + mx) & mask;
                        for (j = 0; j < ny; j++) {
                            p.x = r.x;
                            p.y = r.y;
                            p.z = r.z;
                            idx = lx;
                            for (i = 0; i < nx; i++) {
                                h = height[idx];
                                a.x = d3fl_AxisY.x * h;
                                a.y = d3fl_AxisY.y * h;
                                a.z = d3fl_AxisY.z * h;
                                v->x = p.x + a.x;
                                v->y = p.y + a.y;
                                v->z = p.z + a.z;
                                D3FL_LIGHT(v, light[idx])
                                p.x += dx.x;
                                p.y += dx.y;
                                p.z += dx.z;
                                idx = (idx + s) & mask;
                                v++;
                            }
                            r.x += dz.x;
                            r.y += dz.y;
                            r.z += dz.z;
                            lx = (lx + s * size) & mask;
                        }
                        if (s > 0 && changed) {
                            v = (D3PVERT *)t->points;
                            poly = t->polys;
                            t->npolys = 0;
                            idx = (my / s * map->size / s + mx / s) & map->idxMask;
                            div = map->divide[half];
                            for (j = 0, ly = my; j < ny - 1; j++, ly += s) {
                                v0 = v;
                                v1 = v + nx;
                                for (i = 0, lx = mx; i < nx - 1; i++, lx += s) {
                                    d = div[idx + i];
                                    if (d & 0x40)
                                        jit = d3_Noise[(j + my) & 0xff][(i + mx) & 0xff] & 0x3f;
                                    else
                                        jit = 0;
                                    poly[0].tex = poly[1].tex = d3fl_GetTexture(types, size, lx, ly, s, map->src);
                                    if (d & 0x80) {
                                        poly[0].flip &= ~1;
                                        poly[0].v[2] = poly[1].v[0] = (D3FLPOINT *)(v0 + 1);
                                        poly[0].v[0] = (D3FLPOINT *)v1;
                                        poly[0].v[1] = (D3FLPOINT *)v0;
                                        poly[1].v[1] = (D3FLPOINT *)(v1 + 1);
                                        poly[1].v[2] = (D3FLPOINT *)v1;
                                        poly[0].uv = &d3fl_Jitter[jit * 24];
                                        poly[1].uv = &d3fl_Jitter[jit * 24 + 6];
                                    } else {
                                        poly[0].flip |= 1;
                                        poly[0].v[1] = poly[1].v[2] = (D3FLPOINT *)(v1 + 1);
                                        poly[1].v[1] = (D3FLPOINT *)(v0 + 1);
                                        poly[0].v[0] = poly[1].v[0] = (D3FLPOINT *)v0;
                                        poly[0].v[2] = (D3FLPOINT *)v1;
                                        poly[0].uv = &d3fl_Jitter[jit * 24 + 12];
                                        poly[1].uv = &d3fl_Jitter[jit * 24 + 18];
                                    }
                                    poly += 2;
                                    v0++;
                                    v1++;
                                    t->npolys += 2;
                                }
                                idx += map->size / s;
                                v += nx;
                            }
                        }
                    }
                }
            /* extra vertices on the edges next to finer tiles; their triangles are split in two */
            for (y = 0; y < 8; y++)
                for (x = 0; x < 8; x++) {
                    if (T.lodLevel && changed) {
                        s = T.lodLevel;
                        half = s >> 1;
                        ny = n / s + 1;
                        if (x == 7)
                            nx = ny - 4 / s;
                        else
                            nx = ny;
                        if (y == 7)
                            ny -= 4 / s;
                        mx = x * n;
                        my = y * n;
                        T.nbpUsed = 0;
                        T.nsplit = 0;
                        sp = T.split;
                        if (x < 7 && map->tiles[y][x + 1].lodLevel > 0 && map->tiles[y][x + 1].lodLevel < s) {
                            /* right edge */
                            lx = mx + n;
                            ly = my + half;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            uv = T.bp;
                            poly = T.polys + (nx - 2) * 2;
                            D3FL_POS(p, lx, ly)
                            fs = (float)s;
                            dz.x = d3fl_AxisZ.x * fs;
                            dz.y = d3fl_AxisZ.y * fs;
                            dz.z = d3fl_AxisZ.z * fs;
                            idx = (ly * size + mx + n) & mask;
                            for (j = 0; j < ny - 1; j++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->x = lx;
                                    sp->y = ly;
                                    D3FL_LIGHT(v, light[idx])
                                    D3FL_COPY_TO(uv, poly[1].uv, 0x18)
                                    if (poly->flip & 1) {
                                        u = (uv[2] + uv[4]) * 0.5f;
                                        w = (uv[3] + uv[5]) * 0.5f;
                                        uv[4] = u;
                                        uv[5] = w;
                                        D3FL_COPY_TO(np, &poly[1], 0x28)
                                        poly[1].v[2] = (D3FLPOINT *)v;
                                        poly[1].uv = uv;
                                        T.nbpUsed++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    } else {
                                        u = (uv[2] + uv[0]) * 0.5f;
                                        w = (uv[3] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, &poly[1], 0x28)
                                        poly[1].v[0] = (D3FLPOINT *)v;
                                        poly[1].uv = uv;
                                        T.nbpUsed++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    }
                                    uv += 6;
                                    sp++;
                                    np->zOfs = 16.0f;
                                    np++;
                                    T.nbpUsed++;
                                    T.npolys++;
                                    v++;
                                    T.nsplit++;
                                    T.npoints++;
                                }
                                p.x += dz.x;
                                p.y += dz.y;
                                p.z += dz.z;
                                idx += s * size;
                                ly += s;
                                poly += (nx - 1) * 2;
                            }
                        }
                        if (x && map->tiles[y][x - 1].lodLevel > 0 && map->tiles[y][x - 1].lodLevel < s) {
                            /* left edge */
                            ly = half + my;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            uv = (float *)((D3FLTRIUV *)T.bp + T.nbpUsed);
                            poly = T.polys;
                            D3FL_POS(p, mx, ly)
                            fs = (float)s;
                            dz.x = d3fl_AxisZ.x * fs;
                            dz.y = d3fl_AxisZ.y * fs;
                            dz.z = d3fl_AxisZ.z * fs;
                            idx = (ly * size + mx) & mask;
                            for (j = 0; j < ny - 1; j++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->x = mx;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->y = ly;
                                    D3FL_LIGHT(v, light[idx])
                                    *(D3FLTRIUV *)uv = *(D3FLTRIUV *)poly->uv;
                                    if (poly->flip & 1) {
                                        u = (uv[4] + uv[0]) * 0.5f;
                                        w = (uv[5] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, poly, 0x28)
                                        poly->v[0] = (D3FLPOINT *)v;
                                        poly->uv = uv;
                                        T.nbpUsed++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[4] = u;
                                        uv[5] = w;
                                        np->v[2] = (D3FLPOINT *)v;
                                    } else {
                                        u = (uv[2] + uv[0]) * 0.5f;
                                        w = (uv[3] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, poly, 0x28)
                                        poly->v[0] = (D3FLPOINT *)v;
                                        poly->uv = uv;
                                        T.nbpUsed++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    }
                                    uv += 6;
                                    sp++;
                                    np->zOfs = 16.0f;
                                    np++;
                                    T.nbpUsed++;
                                    T.npolys++;
                                    v++;
                                    T.npoints++;
                                    T.nsplit++;
                                }
                                p.x += dz.x;
                                p.y += dz.y;
                                p.z += dz.z;
                                idx += s * size;
                                ly += s;
                                poly += (nx - 1) * 2;
                            }
                        }
                        if (y && map->tiles[y - 1][x].lodLevel > 0 && map->tiles[y - 1][x].lodLevel < s) {
                            /* top edge */
                            lx = half + mx;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            uv = (float *)((D3FLTRIUV *)T.bp + T.nbpUsed);
                            poly = T.polys;
                            D3FL_POS(p, lx, my)
                            fs = (float)s;
                            dx.x = d3fl_AxisX.x * fs;
                            dx.y = d3fl_AxisX.y * fs;
                            dx.z = d3fl_AxisX.z * fs;
                            idx = (my * size + half + mx) & mask;
                            for (i = 0; i < nx - 1; i++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->x = lx;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->y = my;
                                    D3FL_LIGHT(v, light[idx])
                                    if (poly->flip & 1) {
                                        D3FL_COPY_TO(uv, poly[1].uv, 0x18)
                                        u = (uv[2] + uv[0]) * 0.5f;
                                        w = (uv[3] + uv[1]) * 0.5f;
                                        uv[0] = u;
                                        uv[1] = w;
                                        D3FL_COPY_TO(np, &poly[1], 0x28)
                                        poly[1].uv = uv;
                                        poly[1].v[0] = (D3FLPOINT *)v;
                                        T.nbpUsed++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    } else {
                                        D3FL_COPY_TO(uv, poly->uv, 0x18)
                                        u = (uv[2] + uv[4]) * 0.5f;
                                        w = (uv[3] + uv[5]) * 0.5f;
                                        uv[4] = u;
                                        uv[5] = w;
                                        D3FL_COPY_TO(np, poly, 0x28)
                                        poly->uv = uv;
                                        poly->v[2] = (D3FLPOINT *)v;
                                        T.nbpUsed++;
                                        uv += 6;
                                        D3FL_COPY_TO(uv, np->uv, 0x18)
                                        np->uv = uv;
                                        uv[2] = u;
                                        uv[3] = w;
                                        np->v[1] = (D3FLPOINT *)v;
                                    }
                                    uv += 6;
                                    sp++;
                                    np->zOfs = 16.0f;
                                    np++;
                                    T.nbpUsed++;
                                    T.npolys++;
                                    v++;
                                    T.npoints++;
                                    T.nsplit++;
                                }
                                p.x += dx.x;
                                p.y += dx.y;
                                p.z += dx.z;
                                idx += s;
                                lx += s;
                                poly += 2;
                            }
                        }
                        if (y < 7 && map->tiles[y + 1][x].lodLevel > 0 && map->tiles[y + 1][x].lodLevel < s) {
                            /* bottom edge */
                            lx = half + mx;
                            ly = n + my;
                            v = (D3PVERT *)T.points + T.npoints;
                            np = T.polys + T.npolys;
                            D3FL_POS(p, lx, ly)
                            uv = (float *)((D3FLTRIUV *)T.bp + T.nbpUsed);
                            fs = (float)s;
                            dx.x = d3fl_AxisX.x * fs;
                            dx.y = d3fl_AxisX.y * fs;
                            dx.z = d3fl_AxisX.z * fs;
                            poly = T.polys + (ny - 2) * (nx - 1) * 2;
                            idx = (ly * size + half + mx) & mask;
                            for (i = 0; i < nx - 1; i++) {
                                if (poly->v[0]) {
                                    h = height[idx];
                                    a.x = d3fl_AxisY.x * h;
                                    a.y = d3fl_AxisY.y * h;
                                    a.z = d3fl_AxisY.z * h;
                                    v->x = a.x + p.x;
                                    v->y = a.y + p.y;
                                    v->z = a.z + p.z;
                                    sp->x = lx;
                                    sp->p = (D3FLPOINT *)v;
                                    sp->y = ly;
                                    D3FL_LIGHT(v, light[idx])
                                    pp = poly;
                                    if (!(poly->flip & 1))
                                        pp = poly + 1;
                                    D3FL_COPY_TO(uv, pp->uv, 0x18)
                                    u = (uv[2] + uv[4]) * 0.5f;
                                    w = (uv[3] + uv[5]) * 0.5f;
                                    uv[4] = u;
                                    uv[5] = w;
                                    D3FL_COPY_TO(np, pp, 0x28)
                                    pp->uv = uv;
                                    pp->v[2] = (D3FLPOINT *)v;
                                    T.nbpUsed++;
                                    uv += 6;
                                    D3FL_COPY_TO(uv, np->uv, 0x18)
                                    np->uv = uv;
                                    uv[2] = u;
                                    uv[3] = w;
                                    uv += 6;
                                    np->v[1] = (D3FLPOINT *)v;
                                    np->zOfs = 16.0f;
                                    T.nbpUsed++;
                                    T.npolys++;
                                    v++;
                                    sp++;
                                    T.npoints++;
                                    T.nsplit++;
                                    np++;
                                }
                                p.x += dx.x;
                                p.y += dx.y;
                                p.z += dx.z;
                                poly += 2;
                                idx += s;
                                lx += s;
                            }
                        }
                    }
                    if (T.lodLevel > 0 && T.nsplit > 0 && !changed)
                        /* same levels: move the edge vertices */
                        for (i = 0, sp = T.split; i < T.nsplit; i++, sp++) {
                            h = height[sp->y * size + sp->x];
                            fx = (float)sp->x;
                            p.x = d3fl_AxisX.x * fx + d3fl_Origin.x;
                            p.y = d3fl_AxisX.y * fx + d3fl_Origin.y;
                            p.z = d3fl_AxisX.z * fx + d3fl_Origin.z;
                            fy = (float)sp->y;
                            q.x = d3fl_AxisZ.x * fy + p.x;
                            q.y = d3fl_AxisZ.y * fy + p.y;
                            q.z = d3fl_AxisZ.z * fy + p.z;
                            ((D3PVERT *)sp->p)->x = d3fl_AxisY.x * h + q.x;
                            ((D3PVERT *)sp->p)->y = d3fl_AxisY.y * h + q.y;
                            ((D3PVERT *)sp->p)->z = d3fl_AxisY.z * h + q.z;
                        }
                    T.lod = T.lodLevel;
                }
            /* clipping, projection and culling */
            for (y = 0, t = map->tiles[0]; y < 8; y++)
                for (x = 0; x < 8; x++, t++)
                    if (t->lodLevel) {
                        npts = t->npoints;
                        npolys = t->npolys;
                        d3_ClipMesh(t->clipCode, (D3PVERT *)t->points, npts, (D3PTRI *)t->polys, npolys);
                        if (d3_FogOn) {
                            for (i = 0, v = (D3PVERT *)t->points; i < npts; i++, v++)
                                if (v->flags < 0) {
                                    k = 1.0f / v->z;
                                    v->sx = k * v->x * d3_ScaleX + d3_CenterX;
                                    v->sy = k * v->y * d3_ScaleY + d3_CenterY;
                                    v->rhw = k;
                                    d3_Dist2 = v->x * v->x + v->y * v->y + v->z * v->z;
                                    v->fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                                        ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                                }
                        } else {
                            for (i = 0, v = (D3PVERT *)t->points; i < npts; i++, v++)
                                if (v->flags < 0) {
                                    k = 1.0f / v->z;
                                    v->sx = k * v->x * d3_ScaleX + d3_CenterX;
                                    v->sy = k * v->y * d3_ScaleY + d3_CenterY;
                                    v->rhw = k;
                                }
                        }
                        for (i = 0, poly = t->polys; i < npolys; i++, poly++)
                            if ((fl = poly->flags) < 0) {
                                v2 = (D3PVERT *)poly->v[2];
                                v1 = (D3PVERT *)poly->v[1];
                                v0 = (D3PVERT *)poly->v[0];
                                if ((v2->flags | v1->flags | v0->flags) & 0x10)
                                    fl |= 0x40;
                                else if ((v0->sx - v1->sx) * (v0->sy - v2->sy) >= (v0->sx - v2->sx) * (v0->sy - v1->sy))
                                    fl &= 0xbf;
                                else
                                    fl &= 0x7f;
                                poly->flags = fl;
                            }
                    }
            map->dirty = 0;
        }
        if (water)
            d3fl_UpdateWater(d3_Time, map->nregions, map->regions);
        /* queue the visible triangles */
        for (y = 0, t = map->tiles[0]; y < 8; y++)
            for (x = 0; x < 8; x++, t++)
                if (t->lodLevel) {
                    if (t->zmin <= d3_MinZ)
                        d3_MinZ = t->zmin;
                    if (t->zmax >= d3_MaxZ)
                        d3_MaxZ = t->zmax;
                    clip = (D3CLIPINFO *)d3_ClipInfo[t->clip & 0x3f];
                    d3_TriCount += t->npolys;
                    d3_VertCount += t->npoints;
                    d3_DrawnVerts += t->npoints;
                    npolys = min((unsigned int)t->npolys, d3_SortMax - d3_SortCount);
                    for (i = 0, poly = t->polys; i < npolys; i++, poly++)
                        if (poly->flags < 0) {
                            if ((tex = poly->tex) && tex->name[0] == '*' && !(c = tex->cache)->flag) {
                                tex->reserved = 0;
                                if (!tex->surf)
                                    tex_Prepare(tex);
                                if (surf = dd_LockSurfaceFlags(tex->surf, 0x20)) {
                                    d3fl_Render(surf, &c->t);
                                    dd_UnlockSurface(tex->surf);
                                    if (d3fl_Mips)
                                        d3d_BuildMipmaps(tex->surf, 0);
                                    c->flag = 1;
                                }
                            }
                            d3_SortPtr->item = poly;
                            if (tex) {
                                d3_SortPtr->key = ((D3TEXTURE *)tex - (D3TEXTURE *)d3_Textures) + 1;
                                tex->stamp = d3_FrameCount;
                            } else
                                d3_SortPtr->key = 0;
                            poly->clip = clip;
                            d3_SortPtr++;
                            d3_SortCount++;
                        }
                    d3fl_DrawWater(map, t);
                }
    }
}

#undef T

/* 0x446180, renders the cache textures of all tiles at their current detail level and precaches them;
   returns the number of textures drawn */
int d3fl_Prerender(D3FLMAP *map)
{
    int count = 0;
    unsigned int n, s, nx, ny, i, j, x, y, tx, ty, mx, my;
    int size;
    unsigned char *types;
    D3FLTEX *tex;
    D3FLCACHE *c;
    D3FLTILE *t;
    void *p;

    if (d3_Hardware && d3_Camera) {
    n = map->n;
    size = map->size;
    types = map->types;
    d3fl_UpdateTiles(map);
    for (ty = 0; ty < 8; ty++)
        for (tx = 0; tx < 8; tx++) {
            my = ty * n;
            mx = tx * n;
            t = &map->tiles[ty][tx];
            if (!t->lodLevel)
                s = map->lod ? map->lod : 1;
            else
                s = t->lodLevel;
            if (s) {
                ny = n / s + 1;
                nx = ny;
                if (tx == 7)
                    nx -= 4 / s;
                if (ty == 7)
                    ny -= 4 / s;
                for (j = 0; j < ny - 1; j++)
                    for (i = 0; i < nx - 1; i++) {
                        tex = d3fl_GetTexture(types, size, mx + i * s, my + j * s, s, map->src);
                        if (tex && tex->name[0] == '*') {
                            c = tex->cache;
                            if (!c->flag) {
                                tex->reserved = 0;
                                if (!tex->surf)
                                    tex_Prepare(tex);
                                if (p = dd_LockSurfaceFlags(tex->surf, 0x20)) {
                                    d3fl_Render(p, &c->t);
                                    dd_UnlockSurface(tex->surf);
                                    if (d3fl_Mips)
                                        d3d_BuildMipmaps(tex->surf, 0);
                                    c->flag = 1;
                                    count++;
                                }
                            }
                            if (!(tex->state & 0x80)) {
                                d3d_PrecacheTexture(tex->d3dtex);
                                tex->state |= 0x80;
                            }
                        }
                    }
            }
        }
        return count;
    }
    return 0;
}
/* 0x4463b0, map point under the world position pos (x/z) and, if h is given, the ground height there,
   interpolated on the triangle of the (lod: current detail level) mesh
   @deep B: read the whole 111-line asmdiff against 0x4463b0..0x446750 and diffclass: the first 131 instructions are equal, then the bilinear height samples are computed with ebp/edx/esi/edi in other roles (the flat indices and neighbours are the same expressions) and the four fild byte samples are scheduled in another order, 46 instructions equal again, and the second pick block repeats the same register swap; no different constant, callee, condition or field. Register allocation only, needs vargen/try.py */

char map_Pick(D3FLMAP *map, D3VECTOR *pos, char lod, int *px, int *py, float *h)
{
    D3VECTOR4 d;
    float u, v, h0, du, dv;
    int x, y, k, ix;
    unsigned int s, x1, y1;
    unsigned char *div;

    if (!map)
        return 0;
    d.x = pos->x - map->orgx;
    d.z = pos->z - map->orgz;
    if (map->scale != 0.0f)
        d.x /= map->scale;
    if (map->negScale != 0.0f)
        d.z /= map->negScale;
    *px = x = (int)(d.x + 0.5f);
    *py = (int)(d.z + 0.5f);
    if (*px < 0 || *px >= map->size || *py < 0 || *py >= map->size)
        return 0;
    if (h) {
        div = map->divide[0];
        s = 1;
        if (lod) {
            k = map->tiles[*py / map->n][x / map->n].lodLevel;
            if (k == 2 || k == 4) {
                s = k;
                div = map->divide[k >> 1];
            }
        }
        ix = x & ~(s - 1);
        y = *py & ~(s - 1);
        u = (d.x - ix) / s;
        v = (d.z - y) / s;
        if (div && (div[y / s * map->size / s + ix / s] & 0x80)) {
            if (u + v > 1.0f) {
                x1 = (ix + s) & map->mask;
                y1 = (y + s) & map->mask;
                h0 = map->height[y1 * map->size + x1];
                du = map->height[y1 * map->size + ix] - h0;
                dv = map->height[y * map->size + x1] - h0;
                u = 1.0f - u;
                v = 1.0f - v;
            } else {
                h0 = map->height[y * map->size + ix];
                du = map->height[y * map->size + ((ix + s) & map->mask)] - h0;
                dv = map->height[((y + s) & map->mask) * map->size + ix] - h0;
            }
        } else if (u + (1.0f - v) > 1.0f) {
            x1 = (ix + s) & map->mask;
            y1 = (y + s) & map->mask;
            h0 = map->height[y * map->size + x1];
            du = map->height[y * map->size + ix] - h0;
            dv = map->height[y1 * map->size + x1] - h0;
            u = 1.0f - u;
        } else {
            x1 = (ix + s) & map->mask;
            y1 = (y + s) & map->mask;
            h0 = map->height[y1 * map->size + ix];
            du = map->height[y1 * map->size + x1] - h0;
            dv = map->height[y * map->size + ix] - h0;
            v = 1.0f - v;
        }
        *h = (du * u + dv * v + h0) * map->hscale + map->orgy;
    }
    return 1;
}
/* 0x446750, the visible tile vertex nearest to the screen position x, y within r pixels (weighted by depth);
   returns its map position as y << 16 | x and its camera-space position in out, or -1
   @deep B: read the whole 137-line asmdiff against 0x446750..0x446910 and diffclass: 12 equal instructions, then the ret/best and loop variables sit in other stack slots (0x10/0x14/0x18/0x1c/0x20/0x24 permuted; ret -1 through esi/0x18 in the original) and other registers (ebp vs eax/edx), with the fst/fld scheduling around the candidate block and the three nested counters in another order; no different constant, callee, condition or field. Register/slot allocation only, needs vargen/try.py */

int map_PickScreen(D3FLMAP *map, int x, int y, float r, D3VECTOR *out)
{
    D3PVERT *p;
    int tx, ty, s, nx, ny, n, i, k, j, ret;
    float r2, best, dx, dy, d, f;

    if (!map)
        return -1;
    r2 = r * r;
    best = 1.0f;
    ret = -1;
    for (ty = 0; ty < 8; ty++)
        for (tx = 0; tx < 8; tx++) {
            s = map->tiles[ty][tx].lodLevel;
            if (s > 0) {
                n = map->n / s + 1;
                if (tx == 7)
                    nx = n - 4 / s;
                else
                    nx = n;
                if (ty == 7)
                    ny = n - 4 / s;
                else
                    ny = n;
                n = ny * nx;
                p = (D3PVERT *)map->tiles[ty][tx].points;
                for (i = 0; i < n; i++, p++)
                    if (p->flags & 0x80) {
                        dx = x - p->sx;
                        dy = y - p->sy;
                        d = dx * dx + dy * dy;
                        if (d < r2) {
                            f = sqrt(d) / r * (p->z / d3_MaxZ);
                            if (f < best) {
                                k = i / nx * s + map->n * ty;
                                if (k < map->size) {
                                    j = map->n * tx + i % nx * s;
                                    if (j < map->size) {
                                        ret = k << 16 | j;
                                        out->x = p->x;
                                        out->y = p->y;
                                        out->z = p->z;
                                        best = f;
                                    }
                                }
                            }
                        }
                    }
            }
        }
    return ret;
}
/* 0x446910, water region flag of the map point x, y, or -1 */

int map_Region(D3FLMAP *map, int x, int y)
{
    D3FLWREG *r;

    if (x > 0 && y > 0 && x < map->size && y < map->size)
        for (r = map->tiles[y / map->n][x / map->n].wregions; r && r->y >= 0; r++)
            if (y == r->y && x >= r->x0 && x <= r->x1)
                return (unsigned char)r->flag;
    return -1;
}

/* ---- 0x446990..0x446e10, the end of the TU (worker d3if; was unk_446990.c until round 9). The next file,
   d3_interface.c, starts at 0x446e10 (its first string, the SelectTextureSet message at 0x5fbd08, follows
   the 1.0f constant of this TU). ---- */

/* 0x446990, distance along the view rays to the edge of the map volume (4 planes through the corners) */
float d3fl_FarDist(D3FLMAP *map, D3VECTOR *pos, D3MATRIX *m)
{
    D3VECTOR4 pl[4];
    D3VECTOR4 u, v;
    float dx, dy, dz, f, best, zmin, t, t4, t5, t6;
    int i, j;
    D3VECTOR4 *r;

    dx = map->orgx - pos->x;
    dy = map->orgy - pos->y;
    dz = map->orgz - pos->z;
    u.x = dy * m->_21 + dz * m->_31 + dx * m->_11;
    u.y = dy * m->_22 + dz * m->_32 + dx * m->_12;
    u.z = dx * m->_13 + dy * m->_23 + dz * m->_33;
    t4 = m->_21 * 0.0f;
    pl[0].x = (m->_11 * 0.0f - m->_31 * ai_FloatOne) + t4;
    t5 = m->_22 * 0.0f;
    pl[0].y = (m->_12 * 0.0f - m->_32 * ai_FloatOne) + t5;
    t6 = m->_23 * 0.0f;
    pl[0].z = (m->_13 * 0.0f - m->_33 * ai_FloatOne) + t6;
    pl[0].w = pl[0].x * u.x + pl[0].y * u.y + pl[0].z * u.z;
    pl[1].x = (m->_31 * 0.0f + t4) + m->_11;
    pl[1].y = (m->_32 * 0.0f + t5) + m->_12;
    pl[1].z = (m->_33 * 0.0f + t6) + m->_13;
    pl[1].w = pl[1].x * u.x + pl[1].y * u.y + pl[1].z * u.z;
    f = (float)(map->size - 4);
    v.x = f * map->scale + map->orgx;
    v.y = f * map->xAxisY + map->orgy;
    v.z = f * map->xAxisZ + map->orgz;
    v.x = f * map->zAxisX + v.x;
    v.y = f * map->zAxisY + v.y;
    v.z = f * map->negScale + v.z;
    v.x -= pos->x;
    v.y -= pos->y;
    v.z -= pos->z;
    u.x = v.y * m->_21 + v.z * m->_31 + v.x * m->_11;
    u.y = v.y * m->_22 + v.z * m->_32 + v.x * m->_12;
    u.z = v.x * m->_13 + v.y * m->_23 + v.z * m->_33;
    pl[2].x = (m->_31 * 0.0f + t4) - m->_11 * ai_FloatOne;
    pl[2].y = (m->_32 * 0.0f + t5) - m->_12 * ai_FloatOne;
    pl[2].z = (m->_33 * 0.0f + t6) - m->_13 * ai_FloatOne;
    pl[2].w = pl[2].x * u.x + pl[2].y * u.y + pl[2].z * u.z;
    pl[3].x = (m->_11 * 0.0f + t4) + m->_31;
    pl[3].y = (m->_12 * 0.0f + t5) + m->_32;
    pl[3].z = (m->_13 * 0.0f + t6) + m->_33;
    pl[3].w = pl[3].x * u.x + pl[3].y * u.y + pl[3].z * u.z;
    zmin = min(pl[0].w, min(pl[1].w, min(pl[2].w, pl[3].w)));
    best = 1e10f;
    for (r = d3_Frustum, i = 0; i < 5; i++, r++)
        for (j = 0; j < 4; j++) {
            t = pl[j].x * r->x + pl[j].z * r->z + r->y * pl[j].y;
            if ((t < -1e-07f || t > 1e-07f) && (t = pl[j].w / t, t > 0.0f) && (t > zmin && t < best))
                best = t;
        }
    if (best == 1e10)
        return fabs(zmin);
    return best;
}
/* 0x446dc0, sets the tile shift (low nibble of map->flags byte +0x1c71), rebuilding the tiles and the light map */

void d3fl_SetShift(D3FLMAP *map, unsigned char shift, char light)
{
    if (map->shift != shift) {
        d3fl_FreeTiles(map);
        map->shift = shift;
        d3fl_Inflate(map);
        if (light)
            d3fl_MakeLight(map);
    }
}

D3BOXVERT d3fl_Box[8] = {0};  /* 0x6334b8, result of d3fl_TileBox */
void *d3_Terrain = 0;  /* 0x633738, the current scene's terrain map (a D3FLMAP; d3_sm.c / d3s.c read it as D3TERRAIN) */
char d3fl_Mips = 0;  /* 0x63373c */
char d3fl_MapShift = 0;  /* 0x63373d, texture shift copied into D3FLMAP.shift (d3fl_Init argument) */
float d3fl_LightMargin = 0;  /* 0x633740, world units added to map->scale when a light's
                                             ground radius is converted to map cells (d3fl_LightCB; no writer) */
unsigned int d3fl_Count = 0;  /* 0x633744 */
D3FLCACHE * d3fl_Cache = 0;  /* 0x63374c */
unsigned char d3fl_DetailShift = 0;  /* 0x633750, 6 - log2(detail scale): */
unsigned char d3fl_DetailSizeShift = 0;  /* 0x633751, the byte right after d3fl_DetailShift (f3init.c's detail shift) */
D3VECTOR d3_SunColor = {0};  /* 0x633b1c, the sun colour, added to the ambient colour */
float d3fl_Shade[1024] = {0};  /* 0xfb9940, sun shade by -cos(angle) * 1023 */
unsigned long d3fl_ColorTab[3][256] = {0};  /* 0xfbac60, 8-bit channel -> 16-bit pixel bits */
D3VECTOR4 d3fl_LightCol = {0};  /* 0xfbb860, color per light-map unit */
unsigned int d3fl_TileN = 0;  /* 0xfbb870, pixels per tile in the texture being drawn */
int d3fl_DstPitch = 0;  /* 0xfbb874, bytes per texture row */
D3VECTOR4 d3fl_SunCol = {0};  /* 0xfbb880, added where the light map has bit 7 */
float d3fl_UV[2][8] = {0};  /* 0xfbb8a0 */
D3VECTOR4 d3fl_AxisZ = {0};  /* 0xfbb8e0, world step per map y */
unsigned long d3fl_W16[16 * 16] = {0};  /* 0xfbb900 */
D3VECTOR4 d3fl_AxisY = {0};  /* 0xfbbd00, world step per height unit */
int d3fl_SrcPitch = 0;  /* 0xfbbd10, source bytes per row step */
D3VECTOR4 d3fl_Ambient = {0};  /* 0xfbbd20 */
D3VECTOR4 d3fl_Origin = {0};  /* 0xfbbd30, map point (0, 0, 0) in world space */
unsigned long d3fl_W32[32 * 32] = {0};  /* 0xfbbd40 */
unsigned long d3fl_W64[64 * 64] = {0};  /* 0xfbcd40, blend weights for 64 pixels per tile */
int d3fl_SrcStep = 0;  /* 0xfc0d40, source bytes per pixel step */
D3VECTOR4 d3fl_AxisX = {0};  /* 0xfc0d50, world step per map x */
float d3fl_Jitter[1536] = {0};  /* 0xfc0d60 */
int d3fl_WPitch = 0;  /* 0xfc2560, weight bytes per row step */
const char d3fl_Suffix[3][11] = { "", "_high_1", "_high_2" };  /* 0x5ebaa8, "", "_high_1", "_high_2": source texture mip levels */
D3FLBLEND d3fl_Blend = d3fl_BlendC;  /* 0x5fb970 */
