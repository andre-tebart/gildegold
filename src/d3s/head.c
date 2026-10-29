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

extern int d3s_EdgeX[4];
extern int d3s_EdgeY[4];
extern int d3s_EdgeC[3];
extern int d3s_Next[6];                     /* 0x5eba80, per vertex: the next and the previous one */
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
extern int d3_Pitch16;                      /* 0x14661a0 */
extern int d3_Pitch8;                       /* 0x1463588 */
extern void *d3s_ShadowBits;                 /* 0x1463590 */
D3OBJECT * d3s_Object = 0;  /* 0x633448, object the shadow polygons go to */
unsigned int d3s_CacheCount = 0;  /* 0x633db8 */
unsigned int d3s_CacheAge = 0;  /* 0x633dbc */
extern D3SCENT * d3s_Cache;
int d3s_Count = 0;  /* 0x633dc0 */
unsigned int d3s_MaxPolys = 0;  /* 0x633dc4 */
int d3s_MaxPolys2 = 0;  /* 0x633dc8 */
extern int d3s_ShadowMapSize;              /* 0x633dcc, cells per side of the object shadow map */
extern const unsigned char d3s_Palette[];
extern float * d3s_Verts;
extern int d3s_NumVerts;
extern float * d3s_UVs;
extern int d3s_NumUVs;
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

