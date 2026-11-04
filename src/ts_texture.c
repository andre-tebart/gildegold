/* ts_texture -- the texture manager (original path D:\develop\Units\d3\ts_texture.c, the #line below; bounded by
   the d3_sm.c and d2.c tags): the texture slot table, the hardware surface cache and the 16-bit colour tables of
   the software renderer, 0x45f610..0x4616d0.
   Open: not included yet (dcp headers conflicts ts_texture HEADER): bio.h; held by own declarations that differ
   from the owner's: file_Exists.
   unit of rea 8cef7d5b (src/game/ts_texture.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\Units\\d3\\ts_texture.c"
/* ts_texture.c -- original path D:\develop\Units\d3\ts_texture.c
   Texture manager: the texture slot table, the hardware surface cache and the 16-bit colour tables
   of the software renderer. Extent in GildeGold.exe: 0x45f610..0x4616d0 (start inferred from .data
   and the D3DCHECK line number, end = start of zlib). Functions are kept in their original order.
   Names are ours.
   @named_by leaked
   @boundary proven */
struct VFSDIR;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares some of this file's functions and globals with partial types; hide those
   declarations, the full types are in ts_texture.h. */
#define tex_Load d3h_tex_Load
#define tex_Reload d3h_tex_Unk460e50
#define tex_Release d3h_tex_Release
#define tex_SetHiColor d3h_tex_Unk4611b0
#define tex_Unload d3h_tex_Unk461140
#define d3_Textures d3h_Textures
#include "d3.h"
#undef tex_Load
#undef tex_Reload
#undef tex_Release
#undef tex_SetHiColor
#undef tex_Unload
#undef d3_Textures
#include "ts_texture.h"
#include "d3_d3d.h"
#include "data_views.h"
#include "bmp.h"
#include "d3_engine.h"
#include "dd_vesa.h"
#include "mem.h"
#include "vfs.h"
extern const char d3_ScrollPool[];

/* engine globals used here */
extern unsigned char d3_MipSkip;
/* 0xfe79c0 is one 0x50-byte D3DTEXFMTCTX (d3d_UsedTexFormat, d3_d3d.c).  The names below were
   separate definitions in this unit; they are its fields (pf at +0x08, bpp2 at +0x28, pf2 at
   +0x2c, same at +0x4d) and must not claim the block again. */
#define d3_TexFormat8 d3d_UsedTexFormat.pf            /* 0xfe79c8 */
#define d3_TexFormat16Bpp d3d_UsedTexFormat.bpp2      /* 0xfe79e8 */
#define d3_TexFormat16 d3d_UsedTexFormat.pf2          /* 0xfe79ec */
#define d3_TexFormatsSame d3d_UsedTexFormat.same           /* 0xfe7a0d */
extern ULONG dd_Ref;                        /* 0x146356c, result of the last Release (dd_vesa.c) */
char ts_PathBuf[256] = {0};  /* 0x633e00 */
/* 0x5ebc20, the epsilon of d3sm_Pick and the scroll speed table are one block in the original: the double
   (a float 0.0001f promoted, d3_SnapEpsilon at +0) is at 0x5ebc20 and the speed table starts in its upper
   half at 0x5ebc24, so the two names keep their bytes only as views of one definition; the frame duration
   table d3_AnimDiv (12 dwords at +0x40, 0x5ebc60 .. 0x5ebc90) is part of the block too */
const char d3_ScrollPool[0x70] = {
    /* the double 0.0001f promoted to double: d3_SnapEpsilon (+0) */
    0x00, 0x00, 0x00, 0xe0, 0xe2, 0x36, 0x1a, 0x3f,
    /* d3_ScrollSpeed from +0x4 (15 floats); the frame durations follow at +0x40 */
    0x65, 0xcf, 0x0b, 0x38, 0x65, 0xcf, 0x8b, 0x38,
    0x18, 0xb7, 0xd1, 0x38, 0x65, 0xcf, 0x0b, 0x39,
    0x3f, 0xc3, 0x2e, 0x39, 0xf0, 0xaa, 0x74, 0x39,
    0x51, 0x49, 0x9d, 0x39, 0xac, 0xc5, 0x27, 0x38,
    0x18, 0xb7, 0x51, 0x38, 0x48, 0x06, 0x26, 0x3a,
    0x18, 0xb7, 0x51, 0x3a, 0xe7, 0x67, 0x7d, 0x3a,
    0xd7, 0xea, 0x98, 0x3a, 0x34, 0x80, 0xb7, 0x3a,
    /* d3_AnimDiv from +0x40 (12 dwords) */
    0x0d, 0x74, 0xda, 0x3a, 0x0f, 0x00, 0x00, 0x00,
    0x0d, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
/* d3_ScrollSpeed: view of d3_ScrollPool (tools/data_views.py) */
/* d3_AnimDiv: view of d3_ScrollPool (tools/data_views.py) */


void d3d_CreateTexture(DWORD size, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext, DWORD n, BYTE *pal, unsigned char mips);           /* 0x4383d0 */
void d3d_SetSurfacePalette(DDSURFACEDESC2 *desc, LPDIRECTDRAWSURFACE4 surf, DWORD n, BYTE *pal);                        /* 0x438180 */
char file_Exists(char *path);                                                               /* 0x401090, @original: the callers here read the result as a char (byte-matched ts_LoadAnim: test al, al), against bio.h's int view */
unsigned int util_Hash(unsigned char *buf, unsigned int len);                                                /* 0x401000 */

void d3d_LoadTexture(char *name, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext, unsigned char mips, DWORD w, DWORD h, int bpp);                  /* 0x437390 */
void ts_SyncAliases(TSTEXTURE *tex);                                                        /* 0x460de0 */
void tex_Reload(TSTEXTURE *tex, char keep);                                                 /* 0x460e50 */
TSTEXTURE *tex_CreateAlias(TSTEXTURE *src, TSFLAGS flags, char group);                      /* 0x4614f0 */

struct TSTEXTURE * tex_Load(char *name, TSFLAGS flags, unsigned char opts, unsigned char fmt);/* 0x4605c0 */

int ts_Unk633f00 = 0;               /* 0x633f00 */
TSCACHE *ts_Cache = 0;              /* 0x633f08 */
TSTEXTURE *d3_Textures = 0;             /* 0xfb9074 */
unsigned int ts_MaxHiCol = 0;                   /* 0xfb9078 */
unsigned int ts_CacheUsed = 0;          /* 0xfb907c */
unsigned int ts_CacheSize = 0;          /* 0xfb9080 */
unsigned int d3_TexCount = 0;           /* 0xfb9084 */
float ts_ScrollU[0x20] = {0}; /* 0xfb90a0 */
float ts_ScrollV[0x20] = {0}; /* 0xfb9120 */
int ts_NextGroup = 0;                   /* 0xfb91a0 */
TSHICOL *ts_HiCol = 0;                  /* 0xfb91a4 */
int ts_TexUsed = 0;                     /* 0xfb91a8 */
unsigned long ts_LastTime = 0;          /* 0xfb91ac */
unsigned int ts_HiColCount = 0;         /* 0xfb91b0 */
int ts_CacheReset = 0;                  /* 0xfb91b4, always 0: zeroed with the cache counters at init, never read (reasoned name) */
void *ts_Palette = 0;                   /* 0xfb91b8 */
int ts_Mips = 8;  /* 0x5fd0b0, initialized to 8; TODO define here once .data is matched */
/* 0x45f610, sets the surface cache to size entries: clears the cache counters and (when size > 0) allocates
   the TSCACHE array */

void ts_InitCache(unsigned int size)
{
    ts_CacheSize = size;
    ts_CacheReset = 0;
    ts_CacheUsed = 0;
    if (size > 0)
        ts_Cache = mem_Alloc(size * sizeof(TSCACHE), "d3_ts:SurfaceCache");
    else
        ts_Cache = 0;
}
/* 0x45f650, frees the surface cache: releases the texture and surface of every used entry, frees the
   TSCACHE array and resets the counters */

void ts_FreeCache(void)
{
    TSCACHE *sce;
    unsigned int i;

    if (ts_Cache) {
        for (i = 0, sce = ts_Cache; i < ts_CacheSize; i++, sce++) {
            if (sce->size > 0) {
                if (sce->d3dtex) {
                    dd_Ref = sce->d3dtex->lpVtbl->Release(sce->d3dtex);
                    sce->d3dtex = 0;
                }
                if (sce->surf) {
                    dd_Ref = sce->surf->lpVtbl->Release(sce->surf);
                    sce->surf = 0;
                }
                sce->size = 0;
            }
        }
        mem_Free(ts_Cache);
        ts_Cache = 0;
    }
    ts_CacheSize = 0;
    ts_CacheUsed = 0;
}
/* 0x45f6d0, stores tex's surface in cache entry sce (releasing what the entry held): its size, frame time,
   flags and palette, and the surface's texture */

void ts_CacheStore(TSCACHE *sce, TSTEXTURE *tex)
{
    if (sce->size > 0) {
        if (sce->d3dtex) {
            dd_Ref = sce->d3dtex->lpVtbl->Release(sce->d3dtex);
            sce->d3dtex = 0;
        }
        if (sce->surf) {
            dd_Ref = sce->surf->lpVtbl->Release(sce->surf);
            sce->surf = 0;
        }
    }
    sce->hicolor = tex->hicolor;
    sce->time = d3_FrameCount;
    sce->size = tex->size;
    sce->surf = tex->surf;
    sce->nomip = tex->nomip;
    if (sce->surf) {
        if (tex->bpp == 8) {
#line 133
            D3DCHECK(sce->surf->lpVtbl->SetPalette(sce->surf, NULL));
            sce->d3dtex = tex->d3dtex;
        } else
            sce->d3dtex = tex->d3dtex;
    } else
        sce->d3dtex = tex->d3dtex;
}
/* 0x45f790, puts tex's surface into the texture cache: a free entry when one exists, else the entry unused longest
   (its contents released); when every entry was used this frame, releases tex's D3D texture and surface instead */

void ts_CacheRelease(TSTEXTURE *tex)
{
    TSCACHE *sce, *best = 0;
    unsigned int i, oldest = d3_FrameCount;

    if (ts_CacheUsed == ts_CacheSize) {
        for (i = ts_CacheSize, sce = ts_Cache; i > 0; i--, sce++) {
            if (sce->size > 0 && sce->time < oldest) {
                oldest = sce->time;
                best = sce;
            }
        }
        if (best) {
            ts_CacheStore(best, tex);
            return;
        }
        if (tex->d3dtex) {
            dd_Ref = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (tex->surf) {
            dd_Ref = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
    } else {
        for (i = 0, sce = ts_Cache; i < ts_CacheSize; i++, sce++) {
            if (!sce->size) {
                ts_CacheStore(sce, tex);
                ts_CacheUsed++;
                return;
            }
        }
    }
}
/* 0x45f860, creates tex's D3D texture: the 16-bit format when hicolor, else the 8-bit one with npal, pal passed
   through, and mips from ts_Mips unless tex->nomip */

void ts_CreateSurface(TSTEXTURE *tex, int npal, unsigned char *pal)
{
    unsigned char mips = tex->nomip ? 0 : ts_Mips;

    if (tex->hicolor)
        d3d_CreateTexture(tex->size, &d3_TexFormat16, &tex->surf, &tex->d3dtex, 0, 0, mips);
    else
        d3d_CreateTexture(tex->size, &d3_TexFormat8, &tex->surf, &tex->d3dtex, npal, pal, mips);
}
/* 0x45f8d0, gives tex the surfaces of a cache entry of the same size, palettization and mip setting (the entry
   freed, its palette applied), the 8-bit palette set on it; with no such entry, creates a new surface */

void ts_CacheFetch(TSTEXTURE *tex, int npal, unsigned char *pal)
{
    TSCACHE *sce;
    unsigned int i;

    if (ts_CacheUsed > 0) {
        for (i = 0, sce = ts_Cache; i < ts_CacheSize; i++, sce++) {
            if (sce->size == tex->size && (sce->hicolor == tex->hicolor || d3_TexFormatsSame)
                && tex->nomip == sce->nomip) {
                sce->size = 0;
                ts_CacheUsed--;
                tex->surf = sce->surf;
                tex->d3dtex = sce->d3dtex;
                d3d_SetSurfacePalette(0, tex->surf, npal, pal);
                return;
            }
        }
    }
    ts_CreateSurface(tex, npal, pal);
}
/* 0x45f980, the live texture slot whose name matches name (case-insensitive), or 0 */

TSTEXTURE *tex_Find(char *name)
{
    char buf[0x40];
    TSTEXTURE *tex;
    unsigned int i;

    strcpy(buf, name);
    for (i = 0, tex = d3_Textures; i < d3_TexCount; i++, tex++) {
        if (tex->refs > 0 && !_strcmpi(buf, tex->name))
            return tex;
    }
    return 0;
}
/* 0x45f9f0, the VFS path of the "*<name>.BMP" file (the VFS search ignores case), or 0 */

char *tex_FindFile(char *name)
{
    char path[0x100];

    strcpy(path, "*");
    strcat(path, name);
    strcat(path, ".BMP");
    return vfs_GetPath(path, ts_Palette, vfs_Root, ts_PathBuf);
}
/* 0x45fa80, initializes the texture manager: count texture slots, maxhicol hi-colour tables, the mip level and
   the surface cache of cache entries */

void tex_Init(int count, int maxhicol, int mips, unsigned int cache)
{
    d3_TexCount = count;
    ts_Palette = 0;
    ts_MaxHiCol = maxhicol;
    ts_Mips = mips;
    ts_HiColCount = 0;
    ts_TexUsed = 0;
    d3_Textures = mem_Alloc(count * sizeof(TSTEXTURE), "d3_ts:InitTextures(tex)");
    if (maxhicol > 0)
        ts_HiCol = mem_Alloc(maxhicol * sizeof(TSHICOL), "d3_ts:InitTextures(hicol)");
    else
        ts_HiCol = 0;
    memset(ts_ScrollU, 0, sizeof(ts_ScrollU));
    memset(ts_ScrollV, 0, sizeof(ts_ScrollV));
    ts_NextGroup = 0;
    ts_LastTime = d3_Time;
    ts_InitCache(cache);
}
/* 0x45fb30, sets the VFS directory the texture loader takes its palettes from (under the VFS root) */

void tex_SetPalette(void *pal)
{
    ts_Palette = vfs_FindDir(pal, vfs_Root);
}
/* 0x45fb50, frees tex: a hardware texture's surfaces go through the cache (or are released), then its data buffer */

void tex_Free(TSTEXTURE *tex)
{
    if (d3_Hardware && tex->name[0] == '*' && tex->surf && tex->d3dtex) {
        ts_CacheRelease(tex);
        tex->d3dtex = 0;
        tex->surf = 0;
    } else {
        if (tex->d3dtex) {
            dd_Ref = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (tex->surf) {
            dd_Ref = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
    }
    if (tex->data) {
        mem_Free(tex->data);
        tex->data = 0;
    }
}
/* 0x45fbc0, drops one reference to tex: an animated master releases its frames first; at the last
   reference an alias only clears its surface handles, otherwise the slot passes to its first live alias
   (the other aliases re-pointed to it) or the texture is freed; ts_TexUsed counts the live textures */

void tex_Release(TSTEXTURE *tex)
{
    TSTEXTURE *t;
    unsigned int i, j;
    int index, n;

    if (tex && tex->refs > 0) {
        if (tex->anim > 0 && !tex->frame) {
            for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                if (t->refs > 0 && t->group == tex->group && t->frame > 0)
                    tex_Release(t);
            }
        }
        tex->refs--;
        if (tex->refs <= 0) {
            if (tex->alias) {
                tex->surf = 0;
                tex->d3dtex = 0;
                tex->data = 0;
            } else {
                index = tex - d3_Textures;
                for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                    if (t->refs > 0 && t->alias && t->mask == index) {
                        t->alias = 0;
                        t->mask = tex->mask;
                        n = t - d3_Textures;
                        for (j = i + 1, t++; j < d3_TexCount; j++, t++) {
                            if (t->refs > 0 && t->alias && t->mask == index)
                                t->mask = n;
                        }
                        break;
                    }
                }
                if (i == d3_TexCount)
                    tex_Free(tex);
            }
            tex->refs = 0;
            ts_TexUsed--;
        }
    }
}
/* 0x45fd00 */

void tex_Exit(void)
{
    TSTEXTURE *tex;
    unsigned int i;

    for (i = 0, tex = d3_Textures; i < d3_TexCount; i++, tex++) {
        while (tex->refs > 0)
            tex_Release(tex);
    }
    mem_Free(d3_Textures);
    d3_Textures = 0;
    for (i = 0; i < ts_HiColCount; i++) {
        mem_Free(ts_HiCol[i].tab);
        ts_HiCol[i].tab = 0;
    }
    ts_Palette = 0;
    if (ts_MaxHiCol > 0) {
        mem_Free(ts_HiCol);
        ts_HiCol = 0;
    }
    ts_HiColCount = 0;
    ts_TexUsed = 0;
    ts_Mips = 0;
    ts_FreeCache();
}
/* 0x45fdd0, a + (b - a) * t / n, rounded; a and b are swapped if a > b */

int ts_Lerp(float a, float b, float n, float t)
{
    float tmp;

    if (a > b) {
        tmp = a;
        a = b;
        b = tmp;
    }
    return (int)((b - a) / n * t + a + 0.5);
}
/* 0x45fe10, index of colour (r, g, b) in the table's palette; added (with its 63 shades) if missing */

unsigned char ts_AddColor(unsigned char r, unsigned char g, unsigned char b, TSHICOL *h)
{
    unsigned int i, j;

    for (i = 0; i < 0x100 - h->free; i++) {
        if (h->rgb[i][0] == r && h->rgb[i][1] == g && h->rgb[i][2] == b)
            break;
    }
    if (i == 0x100 - h->free) {
        h->tab[0x3f00 + i] = dd_RGB(r, g, b);
        h->rgb[i][0] = r;
        h->rgb[i][1] = g;
        h->rgb[i][2] = b;
        for (j = 0; j < 0x3f; j++)
            h->tab[(j << 8) + i] = dd_RGB(ts_Lerp(0, r, 62, j), ts_Lerp(0, g, 62, j), ts_Lerp(0, b, 62, j));
        h->free--;
    }
    return i;
}
/* 0x45ff70, finds or creates a colour table holding the n colours of pal; map receives their indices */

unsigned short *ts_GetHiColTab(unsigned char *pal, unsigned int n, unsigned char *map)
{
    TSHICOL *h = 0;
    unsigned char *p;
    unsigned int i, j, k, found;

    if (ts_HiColCount > 0) {
        if (n > ts_HiCol[ts_HiColCount].free && ts_HiColCount == ts_MaxHiCol)
            return 0;
        for (i = 0; i < ts_HiColCount; i++) {
            found = 0;
            for (j = 0; j < n; j++) {
                for (k = 0; k < 0x100 - ts_HiCol[i].free; k++) {
                    if (ts_HiCol[i].rgb[k][0] == pal[j * 3] && ts_HiCol[i].rgb[k][1] == pal[j * 3 + 1] && ts_HiCol[i].rgb[k][2] == pal[j * 3 + 2])
                        found++;
                }
            }
            if (n - found <= ts_HiCol[i].free) {
                h = &ts_HiCol[i];
                break;
            }
        }
    }
    if (!h) {
        ts_HiCol[ts_HiColCount].tab = mem_Alloc(0x8200, "d3_ts:HiColTab");
        ts_HiCol[ts_HiColCount].free = 0x100;
        h = &ts_HiCol[ts_HiColCount];
        if (n < 0x100)
            ts_AddColor(0, 0, 0, h);
        ts_HiColCount++;
    }
    for (i = 0, p = pal; i < n; i++, p += 3)
        map[i] = ts_AddColor(p[0], p[1], p[2], h);
    return h->tab;
}
/* 0x460130, a free slot: searched from the front for flags.b[0] == 0xff, else from the back */

TSTEXTURE *ts_AllocSlot(TSFLAGS flags, int *index)
{
    TSTEXTURE *tex;
    int i;

    if (ts_TexUsed == d3_TexCount) {
        if (index)
            *index = -1;
        return 0;
    }
    if (flags.b[0] >= 0xff && !(flags.b[2] & 1)) {
        for (i = 0, tex = d3_Textures; tex->refs > 0; i++, tex++)
            ;
    } else {
        for (i = d3_TexCount - 1, tex = &d3_Textures[i]; tex->refs > 0; i--, tex--)
            ;
    }
    if (index)
        *index = i;
    return tex;
}
/* 0x4601b0 */

void tex_AddRef(TSTEXTURE *tex, char all)
{
    TSTEXTURE *t;
    unsigned int i;

    if (tex) {
        if (all && tex->anim > 0 && !tex->frame && tex->group != -1) {
            for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                if (t->refs > 0 && t->group == tex->group)
                    t->refs++;
            }
        } else
            tex->refs++;
    }
}
/* 0x460220, loads the 8-bit pixels for the software renderer @deep B3: single code hunk at 0x46033c, 14 hunks / 8 asm lines, 11/352 bytes: write-address reassociation only - orig lea edx,[eax+ecx]; add eax,ecx (destructive row+x, store [eax+data]); ours lea edx,[ecx+eax]; add edx,ecx (data+x+row). spell.py: no menu rule on the hunk, verdict 'escalate with the diffclass verdict'. Read: full disasm of the inner loop (row from imul, size reloaded at the bottom in both), listing var map. Tried by earlier match sessions (all canonicalise to these 11 B): ~900 spellings - index operand orders, pointer types/casts (360), byte temp, IV forms, i=y*size reuse (already in), x++ in index, declaration orders, n=i+x, while form. Base means exhausted; readability already readable. Remaining is VC6 register allocation of the two sums. */

void ts_LoadSoft(char *file, TSTEXTURE *tex)
{
    unsigned char remap[0x100];
    unsigned char used[0x100];
    unsigned char pal[0x300];
    unsigned char *data;
    unsigned int i, n, x, y;
    int count;

    if (ts_MaxHiCol) {
        data = mem_Alloc(tex->size * tex->size, "d3_ts:tLoadTextureSoft");
        bmp_Load(file, data, pal, 7);
        n = tex->size * tex->size;
        for (i = 0; i < n; i++)
            used[data[i]] = 1;
        count = 0;
        for (i = 0; i < 0x100; i++) {
            if (used[i]) {
                pal[count * 3] = pal[i * 3];
                pal[count * 3 + 1] = pal[i * 3 + 1];
                pal[count * 3 + 2] = pal[i * 3 + 2];
                remap[i] = count;
                count++;
            }
        }
        for (i = 0; i < tex->size * tex->size; i++)
            data[i] = remap[data[i]];
        tex->hicol = ts_GetHiColTab(pal, count, remap);
        tex->data = mem_Alloc(tex->size * tex->size, "d3_ts:LoadTextureSoft(data)");
        for (y = 0; y < tex->size; y++) {
            for (x = 0; x < tex->size; x++) {
                /* the row offset in the reused i (not a fresh local, which VC6 forward-substitutes)
                   keeps the remapped byte in bl like the original */
                i = y * tex->size;
                tex->data[i + x] = remap[data[i + x]];
            }
        }
        mem_Free(data);
    }
}
/* 0x460380, a name ending in "_A?<n>" is frame n of an animation: loads frame n + 1 if its file
   exists, else marks the group complete with n + 1 frames */

void ts_LoadAnim(TSTEXTURE *tex, TSFLAGS flags, unsigned char opts, unsigned char fmt)
{
    char name[0x40];
    char base[0x40];
    char upper[0x40];
    char path[0x100];
    char digits = 0;
    unsigned int i;
    TSFLAGS f;
    int n;
    char *p, *s;
    TSTEXTURE *t;

    strcpy(name, tex->name);
    strcpy(base, name);
    strcpy(upper, name);
    _strupr(upper);
    i = strlen(name);
    while (isdigit(name[--i]) && i > 0)
        digits = 1;
    if (i < strlen(name) - 1 && i > 1 && upper[i - 1] == 'A' && upper[i - 2] == '_' && digits) {
        n = atoi(&name[i + 1]);
        if (!n || (flags.b[2] & 4)) {
            f = flags;
            f.b[2] |= 4;
            tex->group = ts_NextGroup;
            tex->frame = n;
            base[i + 1] = 0;
            p = tex_FindFile(name);
            if (p) {
                s = strrchr(p, '/');
                if (s)
                    *s = 0;
                sprintf(path, "%s/%s%i.bmp", p, base, n + 1);
                if (file_Exists(path)) {
                    sprintf(path, "%s%i", base, n + 1);
                    tex_Load(path, f, opts, fmt);
                    return;
                }
                for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                    if (t->refs > 0 && t->group == ts_NextGroup && strstr(t->name, base))
                        t->frames = n + 1;
                }
                ts_NextGroup++;
            }
        }
    }
}
/* 0x4605c0, loads a texture or adds a reference to it; a loaded texture with other parameters is
   shared through an alias slot */

TSTEXTURE *tex_Load(char *name, TSFLAGS flags, unsigned char opts, unsigned char fmt)
{
    char upper[0x40];
    char copy[0x40];
    int h, w, bpp;
    TSTEXTURE *tex, *t, *a;
    unsigned int i;
    int ok;
    char *path;
    unsigned char f69 = fmt >> 4;
    unsigned char f6a = fmt & 0xf;
    unsigned char f5 = (opts & 0x40) && !(flags.b[0] >= 0xff && !(flags.b[2] & 1)) ? 1 : 0;
    unsigned char speed = (opts >> 2) & 0xf;
    unsigned char f4 = (opts >> 1) & 1;
    unsigned char f0 = opts & 1;

    tex = tex_Find(name);
    if (tex) {
        for (i = tex - d3_Textures, t = tex; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && !strcmp(name, t->name) && t->flags.all == flags.all && t->f0 == f0
                && t->f5 == f5 && t->f6a == f6a && t->f69 == f69 && t->f4 == f4 && t->speed == speed) {
                tex = t;
                break;
            }
        }
        if (tex == t) {
            tex_AddRef(tex, 1);
            return tex;
        }
        a = ts_AllocSlot(flags, 0);
        D3_ZERO_AT(a, sizeof(TSTEXTURE));
        strncpy(a->name, name, 0x3f);
        a->size = tex->size;
        a->origsize = tex->origsize;
        a->filebpp = tex->filebpp;
        a->data = tex->data;
        a->surf = tex->surf;
        a->d3dtex = tex->d3dtex;
        if (tex->alias)
            a->mask = tex->mask;
        else
            a->mask = tex - d3_Textures;
        a->unk58 = 0;
        a->stamp = 0;
        a->flags = flags;
        a->f0 = f0;
        a->f5 = f5;
        a->f6a = f6a;
        a->f69 = f69;
        a->f4 = f4;
        a->alias = 1;
        a->hicolor = tex->hicolor;
        a->nomip = tex->nomip;
        a->noscale = tex->noscale;
        a->bpp = tex->bpp;
        a->anim = 0;
        a->refs = 1;
        a->speed = speed;
        a->group = -1;
        ts_TexUsed++;
        ts_LoadAnim(a, flags, opts, fmt);
        return a;
    }
    path = tex_FindFile(name);
    if (path)
        ok = bmp_GetInfo(path, &w, &h, &bpp);
    else {
        ok = 1;
        bpp = 0;
        h = 0;
        w = 0;
    }
    tex = ts_AllocSlot(flags, 0);
    strcpy(upper, name);
    strcpy(copy, name);
    _strupr(upper);
    if (ok && tex && w == h) {
        D3_ZERO_AT(tex, sizeof(TSTEXTURE));
        strncpy(tex->name, name, 0x3f);
        tex->data = 0;
        tex->size = w;
        tex->origsize = w;
        tex->mask = tex->size * tex->size - 1 | tex->size - 1;
        tex->filebpp = bpp;
        tex->unk58 = 0;
        tex->stamp = 0;
        tex->flags = flags;
        tex->f5 = f5;
        tex->f0 = f0;
        tex->f69 = f69;
        tex->f6a = f6a;
        tex->f4 = f4;
        tex->alias = 0;
        tex->hicolor = !d3d_UsedTexFormat.fourcc && bpp > 8 ? 1 : 0;
        tex->nomip = strstr(upper, "_NM") != 0;
        if (tex->flags.b[2] & 2)
            tex->noscale = 1;
        else
            tex->noscale = strstr(upper, "_NS") != 0;
        tex->anim = 0;
        tex->refs = 1;
        tex->group = -1;
        tex->speed = speed;
        ts_TexUsed++;
        ts_LoadAnim(tex, flags, opts, fmt);
        return tex;
    }
    return 0;
}
/* 0x460aa0, the opts and fmt tex_Load was called with */

void tex_GetParams(TSTEXTURE *tex, unsigned char *opts, unsigned char *fmt)
{
    if (tex && opts && fmt) {
        *opts = tex->f0 | tex->f4 << 1 | tex->speed << 2 | tex->f5 << 6;
        *fmt = tex->f6a | tex->f69 << 4;
    }
}
/* 0x460b00, the slot holding frame `frame` of the animation of d3_Textures[index] (0 = the texture itself, NULL if the
   frame is not loaded; `frame` is unsigned char: the original compares the byte directly) */

TSTEXTURE *tex_GetFrame(int index, unsigned char frame)
{
    TSTEXTURE *tex = &d3_Textures[index];
    int group;

    if (frame) {
        group = tex->group;
        if (tex->flags.b[0] >= 0xff && !(tex->flags.b[2] & 1)) {
            for (tex = d3_Textures; tex <= &d3_Textures[d3_TexCount - 1]; tex++) {
                if (tex->refs > 0 && tex->group == group && tex->frame == frame)
                    return tex;
            }
        } else {
            for (tex = d3_Textures + d3_TexCount - 1; tex >= d3_Textures; tex--) {
                if (tex->refs > 0 && tex->group == group && tex->frame == frame)
                    return tex;
            }
        }
        tex = 0;
    }
    return tex;
}
/* 0x460b90, sets the current frame of the object's animated textures */

char tex_Animate(D3OBJECT *obj, unsigned int time)
{
    TSTEXTURE **texs;
    TSTEXTURE *tex, *f;
    TSTRI *tri;
    unsigned int seed;
    int i, k, n, ntris, group;

    if (obj->info && ((TSMESH *)obj->info)->unk10 && ((TSMESH *)obj->info)->nkeys > 0) {
        texs = ((TSMESH *)obj->info)->texs;
        seed = util_Hash(&obj, 4);
        ntris = ((TSMESH *)obj->info)->ntris;
        n = ((TSMESH *)obj->info)->unk10->ntex;
        for (i = 0; i < n; i++) {
            tex = texs[i];
            if (tex && tex->frames > 0 && !tex->frame && tex->speed > 0 && tex->refs > 0) {
                group = tex->group;
                f = tex_GetFrame(tex - d3_Textures, (time / d3_AnimDiv[tex->speed] + seed) % tex->frames);
                for (k = ntris, tri = ((TSMESH *)obj->info)->tris; k > 0; k--, tri++) {
                    if (tri->tex && tri->tex->group == group)
                        tri->tex = f;
                }
            }
        }
    }
    return 1;
}
/* 0x460cb0, advances the texture scroll offsets (wrapped to 0..1) */

void ts_UpdateScroll(unsigned long time)
{
    unsigned long dt;
    float f, s;
    int i;

    if (dt = time - ts_LastTime) {
        f = (float)dt;
        ts_LastTime = time;
        for (i = 1; i < 16; i++) {
            s = f * d3_ScrollSpeed[i];
            ts_ScrollV[i] += s;
            if (ts_ScrollV[i] > 1.0f)
                ts_ScrollV[i] -= 1.0f;
            ts_ScrollU[i] += s;
            if (ts_ScrollU[i] > 1.0f)
                ts_ScrollU[i] -= 1.0f;
        }
        for (i = 1; i < 16; i++) {
            s = f * d3_ScrollSpeed[i];
            ts_ScrollV[i + 16] -= s;
            if (ts_ScrollV[i + 16] < 0.0f)
                ts_ScrollV[i + 16] += 1.0f;
            ts_ScrollU[i + 16] -= s;
            if (ts_ScrollU[i + 16] < 0.0f)
                ts_ScrollU[i + 16] += 1.0f;
        }
    }
}
/* 0x460de0, copies the surfaces of a texture to its aliases */

void ts_SyncAliases(TSTEXTURE *tex)
{
    TSTEXTURE *t;
    unsigned int i;
    int index;

    if (!tex->alias && tex->name[0] != '*') {
        index = tex - d3_Textures;
        for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && t->alias && t->mask == index) {
                t->surf = tex->surf;
                t->d3dtex = tex->d3dtex;
                t->data = tex->data;
                t->size = tex->size;
                t->origsize = tex->origsize;
            }
        }
    }
}
/* 0x460e50, reloads a texture (after a mode change); keep: the software pixels are kept */

void tex_Reload(TSTEXTURE *tex, char keep)
{
    TSTEXTURE *t;
    unsigned int i;
    unsigned char skip;
    char *path;

    if (!tex || tex->refs <= 0)
        return;
    if (tex->alias)
        tex = &d3_Textures[tex->mask];
    skip = d3_MipSkip;
    if (d3_Hardware) {
        if (tex->surf && !keep)
            goto done;
        if (tex->surf) {
            dd_Ref = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
        if (tex->d3dtex) {
            dd_Ref = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (keep && tex->data) {
            mem_Free(tex->data);
            tex->data = 0;
        }
        if (tex->mask && tex->name[0] != '*' && (path = tex_FindFile(tex->name))) {
            i = tex->nomip ? 0 : ts_Mips;
            d3_MipSkip = tex->noscale ? 0 : skip;
            if (tex->hicolor) {
                d3d_LoadTexture(path, &d3_TexFormat16, &tex->surf, &tex->d3dtex, i, tex->origsize, tex->origsize, tex->filebpp);
                tex->bpp = d3_TexFormat16Bpp;
            } else {
                d3d_LoadTexture(path, &d3_TexFormat8, &tex->surf, &tex->d3dtex, i, tex->origsize, tex->origsize, tex->filebpp);
                tex->bpp = d3d_UsedTexFormat.bpp;
            }
            tex->size = tex->origsize >> d3_MipSkip;
        }
    } else {
        if (!tex->surf && !tex->d3dtex && tex->data && !keep)
            goto done;
        if (tex->surf) {
            dd_Ref = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
        if (tex->d3dtex) {
            dd_Ref = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (!keep && tex->data)
            mem_Free(tex->data);
        tex->data = 0;
        if (tex->name[0] == '*')
            tex_Release(tex);
        else if (tex->mask) {
            ts_LoadSoft(tex_FindFile(tex->name), tex);
            tex->bpp = 8;
        }
    }
done:
    ts_SyncAliases(tex);
    d3_MipSkip = skip;
    if (tex->frames > 1 && !tex->frame) {
        for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && t->group == tex->group && t->frame > 0)
                tex_Reload(t, keep);
        }
    }
}
/* 0x461090, reloads all textures; cb(1, done, total) reports progress */

void tex_ReloadAll(char keep, void (*cb)(int, int, int))
{
    TSTEXTURE *t;
    unsigned int i;
    int n, total;

    total = 0;
    if (cb) {
        for (total = 0, i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && !t->alias)
                total++;
        }
    }
    for (n = 0, i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
        if (t->refs > 0 && !t->alias) {
            tex_Reload(t, keep);
            if (cb)
                cb(1, n, total);
            n++;
        }
    }
    ts_Unk633f00 = 0;
}
/* 0x461140, drops the surfaces of a texture and reloads it */

char tex_Unload(TSTEXTURE *tex)
{
    if (tex && d3_Hardware && !tex->alias) {
        if (tex->surf) {
            dd_Ref = tex->surf->lpVtbl->Release(tex->surf);
            tex->surf = 0;
        }
        if (tex->d3dtex) {
            dd_Ref = tex->d3dtex->lpVtbl->Release(tex->d3dtex);
            tex->d3dtex = 0;
        }
        if (tex->mask && tex->name[0] != '*')
            tex_Reload(tex, 0);
    }
    return 0;
}
/* 0x4611b0, switches a texture between the 8-bit and the 16-bit surface format */

char tex_SetHiColor(TSTEXTURE *tex, unsigned char hicolor)
{
    if (tex && d3_Hardware) {
        if (tex->alias)
            tex = &d3_Textures[tex->mask];
        if (d3_TexFormat8.dwRBitMask != d3_TexFormat16.dwRBitMask || d3_TexFormat8.dwGBitMask != d3_TexFormat16.dwGBitMask
            || d3_TexFormat8.dwBBitMask != d3_TexFormat16.dwBBitMask) {
            if (tex->hicolor != hicolor) {
                tex->hicolor = hicolor;
                return tex_Unload(tex);
            }
        }
    }
    return 0;
}
/* 0x461230, creates an empty texture slot (name, size, flags; hardware: fetched from the surface cache, software: data buffer
   and 16-bit colour table); the field stores are in this order for VC6's bitfield / zero-register code (see tex_CreateHi) */

TSTEXTURE *tex_Create(char *name, int size, char hicol, TSFLAGS flags, unsigned char f0, unsigned char f6a,
                      int npal, unsigned char *pal, char mips)
{
    unsigned char map[0x100];
    TSTEXTURE *tex;
    int index;
    int bpp;

    tex = ts_AllocSlot(flags, &index);
    if (!tex)
        return 0;
    D3_ZERO_AT(tex, sizeof(tex));
    strncpy(tex->name, name, 0x3f);
    tex->size = size;
    tex->origsize = size;
    tex->data = 0;
    tex->mask = size * size - 1 | size - 1;
    tex->flags = flags;
    tex->surf = 0;
    tex->d3dtex = 0;
    tex->src = 0;
    tex->unk58 = 0;
    tex->stamp = 0;
    tex->f0 = f0;
    tex->f6a = f6a;
    tex->alias = 0;
    tex->hicolor = !hicol || d3d_UsedTexFormat.fourcc ? 1 : 0;
    tex->nomip = !mips;
    bpp = (unsigned char)(tex->hicolor ? d3_TexFormat16Bpp : d3d_UsedTexFormat.bpp);
    tex->refs = 1;
    tex->bpp = bpp;
    tex->filebpp = bpp;
    tex->anim = 0;
    tex->group = -1;
    ts_TexUsed++;
    if (d3_Hardware) {
        ts_CacheFetch(tex, npal, pal);
        return tex;
    }
    tex->hicol = ts_GetHiColTab(pal, npal, map);
    tex->data = mem_Alloc(tex->size * tex->size, "d3_ts:LoadTextureSoft(data)");
    return tex;
}
/* 0x4613b0, an empty 16-bit texture (hardware only); the order of the field stores is needed for the bytes
   (VC6 sinks the bitfield stores f0 / hicolor / nomip and f6a behind the zero stores) */

TSTEXTURE *tex_CreateHi(char *name, int size, TSFLAGS flags, unsigned char f0, unsigned char f6a, char mips)
{
    TSTEXTURE *tex;
    int index;

    tex = ts_AllocSlot(flags, &index);
    if (tex && d3_Hardware) {
        D3_ZERO_AT(tex, sizeof(tex));
        strncpy(tex->name, name, 0x3f);
        tex->size = size;
        tex->origsize = size;
        tex->data = 0;
        tex->mask = size * size - 1 | size - 1;
        tex->flags = flags;
        tex->surf = 0;
        tex->d3dtex = 0;
        tex->src = 0;
        tex->unk58 = 0;
        tex->stamp = 0;
        tex->f0 = f0;
        tex->f6a = f6a;
        tex->alias = 0;
        tex->hicolor = 1;
        tex->nomip = !mips;
        tex->refs = 1;
        tex->anim = 0;
        tex->group = -1;
        tex->bpp = d3_TexFormat16Bpp;
        ts_TexUsed++;
        return tex;
    }
    return 0;
}
/* 0x4614c0 */

TSTEXTURE *tex_Prepare(TSTEXTURE *tex)
{
    if (d3_Hardware && tex->hicolor)
        ts_CacheFetch(tex, 0, 0);
    return tex;
}
/* 0x4614f0, a new slot sharing the surfaces of src (group: also for the other frames) */

TSTEXTURE *tex_CreateAlias(TSTEXTURE *src, TSFLAGS flags, char group)
{
    TSTEXTURE *a;
    unsigned int i;

    if (!src)
        return 0;
    a = ts_AllocSlot(flags, 0);
    if (a) {
        D3_COPY_AT(a, src, sizeof(TSTEXTURE));
        a->flags = flags;
        a->refs = 1;
        a->src = src;
        a->alias = 1;
        if (src->alias)
            a->mask = src->mask;
        else
            a->mask = src - d3_Textures;
        ts_TexUsed++;
        if (a->group != -1 && a->frames > 0 && group) {
            a->group = ts_NextGroup;
            for (i = 0; i < d3_TexCount; i++) {
                if (d3_Textures[i].refs > 0 && d3_Textures[i].group == src->group)
                    tex_CreateAlias(&d3_Textures[i], flags, 0);
            }
            ts_NextGroup++;
        }
    }
    return a;
}
/* 0x461610, releases an alias made by tex_CreateAlias; returns its source */

TSTEXTURE *tex_ReleaseAlias(TSTEXTURE *tex, char group)
{
    TSTEXTURE *src;
    unsigned int i;

    if (!tex)
        return 0;
    if (!tex->src)
        src = tex;
    else {
        src = tex->src;
        tex->src = 0;
        if (tex->group != -1 && tex->frames > 0 && group) {
            for (i = 0; i < d3_TexCount; i++) {
                if (d3_Textures[i].refs > 0 && d3_Textures[i].group == tex->group)
                    tex_ReleaseAlias(&d3_Textures[i], 0);
            }
        }
        tex_Release(tex);
    }
    return src;
}
/* 0x4616a0, the texture to use for a request with `flags`: a slot loaded without a fixed place (flags.b[0] 0xff,
   not from the back) gets its own alias when the request names a place, else tex itself */

TSTEXTURE *tex_AliasForFlags(TSTEXTURE *tex, TSFLAGS flags, char group)
{
    if (tex->flags.b[0] >= 0xff && !(tex->flags.b[2] & 1) && flags.b[0] != 0xff)
        return tex_CreateAlias(tex, flags, group);
    return tex;
}
