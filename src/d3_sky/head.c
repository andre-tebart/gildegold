/* d3_sky.c -- original path D:\develop\Units\d3\d3_sky.c
   Extent in GildeGold.exe: 0x45ce60..0x45db40 (start: first D3DCHECK at line 38 of the original file;
   .data from 0x5fce40). Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_sky.c); boundaries in decl/text.dcl, notes in src/d3_sky/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these with int parameters; the callers here pass chars */
#define d3_BeginScene d3h_BeginScene
#define d3_SetBlendState d3h_SetBlendState
#include "d3.h"
#undef d3_BeginScene
#undef d3_SetBlendState
#include "d3_sky.h"

#define D3D_VERTEX_STRUCT D3DFVF_TLVERTEX

void d3_BeginScene(unsigned char fog, DWORD fogcol, unsigned char zbuffer);                                      /* 0x4385a0 */
void d3_SetBlendState(unsigned char balpha, unsigned char glow, unsigned char colorkey);                                             /* 0x4387b0 */
void d3_Lerp3(float *a, float *b, float t, float *out);                     /* 0x4539b0 */
float d3_VecAngle(D3VECTOR *a, D3VECTOR *b);                                            /* 0x4539f0 */
void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst);                                        /* 0x454040 */
extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74, (0, 0, 1) */
extern D3VECTOR4 d3_Frustum[5];             /* 0xfc2960, corner directions of the view */
extern char d3_FogOn;                       /* 0x6334a4, d3_engine.h */
extern unsigned long d3_FogColor;           /* 0x6334a0, d3_engine.h */
extern unsigned char d3_Hardware;                    /* 0x633450 */

extern int d3_Busy;                         /* 0x633b04, also counted by d3sm_Build */
D3SKY *d3_Sky = 0;                          /* 0x633dd4 */
int d3sky_Colors[6] = {0}; /* 0xfb91c0 */
#define d3_HardwareC (*(char (*))&d3_Hardware)  /* 0x633450, this unit reads it as `char d3_Hardware` */
