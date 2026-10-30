/* d3_engine.c -- original path D:\develop\Units\d3\d3_engine.c (assert ref 0x43a939)
   Only the start of the file is here: its .data starts at 0x5fb5b0 (d3_DefaultScene) right after
   d3_d3d.c's strings and holds the d3_engine.c path, and the first functions (0x438c30..) use
   d3_DefaultScene. Extent in GildeGold.exe: 0x438c30..(beyond 0x439000, not established).
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_engine.c); boundaries in decl/text.dcl, notes in src/d3_engine/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3_fl.c defines these with char / unsigned int parameters; the 0x43c4e0 caller pushes ints. */
#define d3fl_SetMips d3fl_SetMips
#define d3fl_SetDetail d3fl_SetDetail
/* d3.h declares d3_ForEachObject as void; it returns a char. */
#define d3_ForEachObject d3_ForEachObject_void
/* and d3_UpdateObject's second parameter as int; it is a byte. */
#define d3_UpdateObject d3_UpdateObject_int
/* and d3_UpdateWorldMatrix's third parameter as int; it is a char. */
#define d3_UpdateWorldMatrix d3_UpdateWorldMatrix_int
#include "d3.h"
#undef d3_ForEachObject
#undef d3_UpdateObject
#undef d3_UpdateWorldMatrix
#include "d3_sky.h"
#include "d3_vec.h"
#include "d3_engine.h"
#include "d3_anim.h"
/* d3_mir.h types d3_MirDrawObject / d3_MirBeginFrame with D3MIRROR *; d3_engine.h keeps the D3OBJECT * views. */
#define d3_MirDrawObject d3_Unk4550e0_mirview
#define d3_MirBeginFrame d3_Unk4554b0_mirview
#include "d3_mir.h"
#undef d3_MirDrawObject
#undef d3_MirBeginFrame
#include "d3_light.h"
#include "data_views.h"
#define d3_SetBlendStateI ((void (*)(int, int, int))d3_SetBlendState)  /* 0x4387b0 (d3_d3d.c: unsigned char x3): this unit passes the three flags as int (d3_DrawSortedD3D, draft; the unsigned char prototype gives 2225 bytes against the original's 2211, the int view 2229: a lead, not settled) */
#define dd_ClearB ((void (*)(D3RECT *, unsigned char, unsigned char, unsigned char))dd_Clear)  /* 0x41e100 (dd_vesa.h: RECT *, int x3): @original this unit passes the fog colour as bytes, its D3RECT viewport */
#define d3_MirBeginFrameE ((void (*)(D3OBJECT *, D3OBJECT **))d3_MirBeginFrame)  /* 0x4554b0 (d3_mir.h: D3MIRROR *): this unit passes the mirror as its D3OBJECT ** view */
#define d3_PointToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_PointToRef)  /* 0x458b20 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (unk_458810.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */
/* The two list heads live inside their 0xfc2e00 / 0xfc2580 head sentinels (the original's +0x1fc /
   +0x160 relationship). As separate COMMON pointers the linker would not keep them adjacent, so the
   loader would link the first object into one address while the readers start from another. */
#define d3_StockListHead (*(D3OPS **)((char *)d3_Unkfc2e00 + 0x1fc))  /* 0xfc2ffc */
#define d3_AnimListHead (*(void **)((char *)d3_Unkfc2580 + 0x160))    /* 0xfc26e0 */
#define d3_Unkfc26e4 (*(int *)((char *)d3_Unkfc2580 + 0x164))         /* 0xfc26e4 */
extern D3INITCFG d3_Cfg;
extern char d3d_Config[];
extern char d3_AnimHead[];
extern char d3_StockHead[];
extern D3PARTSYS d3_ParEnd;
extern char d3d_UsedCard[];
void d3fl_FreeMap(struct D3FLMAP *map);     /* 0x43ff90, d3_fl.c (d3.h types d3_Terrain as void *) */
#define d3s_CountU (*(unsigned int (*))&d3s_Count)  /* 0x633dc0, this unit reads it as `unsigned int d3s_Count` */
#define d3sm_ShadowV (*(void * (*))&d3sm_Shadow)  /* 0x633df8, this unit reads it as `void *d3sm_Shadow` */
