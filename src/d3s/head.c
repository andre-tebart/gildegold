/* d3s.c -- original file name unknown: object shadows ("d3s:" allocation tags, texture "Schatten").
   0x459d80..0x45ce60: the integer edge set-up and span functions and a software triangle rasterizer for
   shadow textures, the shadow texture cache, the shadow system set-up / shutdown, the terrain shadow mesh, and
   the object shadows. The start at 0x459d80 follows the mesh code of unk_4591a0.c (d3_GetBoxVerts ends at
   0x459d80); the end is proven by d3_sky.c, whose first D3DCHECK is at line 38 of that file.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3s.c); boundaries in decl/text.dcl, notes in src/d3s/open.md */
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
extern char d3_MipSkip[];

void *tex_Create(char *name, int size, char hicol, D3TEXFLAGS flags, unsigned char f0, unsigned char f6a,
                 int npal, unsigned char *pal, char mips);                              /* 0x461230 */

int d3s_EdgeX[4];  /* 0xfc50d0 */
int d3s_EdgeY[4];  /* 0xfc50e0 */
int d3s_EdgeC[3];  /* 0xfc29b0 */
extern int d3s_Next[6];                     /* 0x5eba80, per vertex: the next and the previous one */
int d3s_SlopeL;  /* 0xfc2de4 */
int d3s_SlopeR;  /* 0xfc2df8 */
int d3s_SlopeC;  /* 0xfc2dd0, slope of the interpolated value along the left edge */
int d3s_XL;  /* 0xfc5334 */
int d3s_XR;  /* 0xfc533c */
int d3s_CL;  /* 0xfc2de8, interpolated value at the left edge */
int d3s_Area;  /* 0xfe73b8 */
unsigned char * d3s_Dst;  /* 0xfc2940 */
D3SPANPIX * d3s_SpanRow;  /* 0xfb9270, current row of the span buffer */
int d3s_ShadowRadius;  /* 0xfb926c, rows / pixels the marked span widens around the shadow */
char d3s_Byte;  /* 0xfb9268 */
unsigned short * d3s_Dst16;  /* 0xfe73b0 */
int d3s_SpanLen;  /* 0xfe73a0 */
int d3s_Fill;  /* 0xfe75ac */
extern int d3_Pitch16;                      /* 0x14661a0 */
extern int d3_Pitch8;                       /* 0x1463588 */
extern void *d3s_ShadowBits;                 /* 0x1463590 */
unsigned int d3s_CacheCount = 0;  /* 0x633db8 */
unsigned int d3s_CacheAge = 0;  /* 0x633dbc */
D3SCENT * d3s_Cache = 0;  /* 0x633dd0 */
unsigned int d3s_MaxPolys = 0;  /* 0x633dc4 */
int d3s_MaxPolys2 = 0;  /* 0x633dc8 */
extern int d3s_ShadowMapSize;              /* 0x633dcc, cells per side of the object shadow map */
const unsigned char d3s_Palette[] = { 0, 0, 0, 0xff, 0xff, 0xff, 0, 0 };  /* 0x5ebbe0, palette */
float * d3s_Verts;  /* 0xfb9258, 6 floats each */
int d3s_NumVerts;  /* 0xfb9220 */
float * d3s_UVs;  /* 0xfb9218 */
int d3s_NumUVs;  /* 0xfb925c */
char d3s_Enabled;  /* 0xfb91f0 */
char d3s_LodTexSize;  /* 0xfb91d8, 1: the shadow texture size follows the mesh LOD */
unsigned char d3s_ShadowMips;  /* 0xfb921c, mip flag of the cached shadow textures */
unsigned int d3s_TexSize;  /* 0xfb9260 */
unsigned int d3s_MaxTexSize;  /* 0xfb91dc */
float d3s_QuadUV[6];  /* 0xfb9200 */
float d3s_QuadUV2[6];  /* 0xfb9240 */
extern D3OBJECT *d3s_Lights[4];             /* 0xfb91e0 */
extern unsigned int d3s_NumLights;          /* 0xfb9264 */
extern struct D3SM *d3sm_Shadow;            /* 0x633df8 */
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

