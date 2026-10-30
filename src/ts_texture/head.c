/* ts_texture
   unit of rea 8cef7d5b (src/game/ts_texture.c); boundaries in decl/text.dcl, notes in src/ts_texture/open.md */
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
extern const char d3_ScrollPool[];

/* engine globals used here */
extern unsigned int d3_FrameCount;          /* 0x63343c */
extern char d3_Hardware;                    /* 0x633450, nonzero: Direct3D textures (else software) */
extern unsigned char d3_MipSkip;
/* 0xfe79c0 is one 0x50-byte D3DTEXFMTCTX (d3d_UsedTexFormat, d3_d3d.c).  The names below were
   separate definitions in this unit; they are its fields (pf at +0x08, bpp2 at +0x28, pf2 at
   +0x2c, same at +0x4d) and must not claim the block again. */
#define d3_TexFormat8 d3d_UsedTexFormat.pf            /* 0xfe79c8 */
#define d3_TexFormat16Bpp d3d_UsedTexFormat.bpp2      /* 0xfe79e8 */
#define d3_TexFormat16 d3d_UsedTexFormat.pf2          /* 0xfe79ec */
#define d3_Unkfe7a0d d3d_UsedTexFormat.same           /* 0xfe7a0d */
extern long d3_ddhr;                        /* 0x146356c, result of the last unchecked DirectDraw call */
void * d3_Unk6333fc = 0;  /* 0x6333fc */
unsigned char d3_MipSkip = 0;  /* 0x63342c */
char d3_Unk633e00[256] = {0};  /* 0x633e00 */
/* 0x5ebc20, the epsilon of d3sm_Pick and the scroll speed table are one block in the original: the double
   (a float 0.0001f promoted, d3_SnapEpsilon at +0) is at 0x5ebc20 and the speed table starts in its upper
   half at 0x5ebc24, so the two names keep their bytes only as views of one definition */
const char d3_ScrollPool[64] = {
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
};
/* d3_ScrollSpeed: view of d3_ScrollPool (tools/data_views.py) */
/* d3_AnimDiv: view of d3_ScrollPool (tools/data_views.py) */


void d3d_CreateTexture(DWORD size, DDPIXELFORMAT *pf, LPDIRECTDRAWSURFACE4 *surf, LPDIRECT3DTEXTURE2 *surftext, DWORD n, BYTE *pal, unsigned char mips);           /* 0x4383d0 */
void d3d_SetSurfacePalette(DDSURFACEDESC2 *desc, LPDIRECTDRAWSURFACE4 surf, DWORD n, BYTE *pal);                        /* 0x438180 */
struct VFSDIR * vfs_FindDir(char *path, struct VFSDIR *d);                                                      /* 0x42f7e0 */
char * vfs_GetPath(char *name, struct VFSDIR *d, struct VFSDIR *stop, char *out);                                   /* 0x42fca0 */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);                                              /* 0x41e670 */
char file_Exists(char *path);                                                               /* 0x401090, @original: the callers here read the result as a char (byte-matched ts_LoadAnim: test al, al), against bio.h's int view */
unsigned char bmp_GetInfo(char *path, int *w, int *h, int *bpp);                                     /* 0x401410 */
unsigned char bmp_Load(char *path, unsigned char *data, unsigned char *pal, int flags);       /* 0x4014d0 */
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
int ts_Unkfb91b4 = 0;                   /* 0xfb91b4 */
void *ts_Palette = 0;                   /* 0xfb91b8 */
int ts_Mips = 8;  /* 0x5fd0b0, initialized to 8; TODO define here once .data is matched */

