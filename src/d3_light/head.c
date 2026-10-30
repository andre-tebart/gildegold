/* d3_light.c -- dynamic lighting of mesh objects ("d3_light:cache", "d3_light:rlight_object"), original file
   name unknown: 0x451e80..0x4538a0. 0x451e80..0x452938 (light callbacks, d3_LightApply) by worker mesh,
   0x452940.. (was unk_452940.c) by worker d3x (round 11); one object view (D3OBJECT + D3_L, d3_light.h)
   since round 12. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_light.c); boundaries in decl/text.dcl, notes in src/d3_light/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_light.h"
#define d3_DirToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_DirToRef)  /* 0x458950 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_PointToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_PointToRef)  /* 0x458b20 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (unk_458810.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern char txt_Keys[0x4800][0x50];

