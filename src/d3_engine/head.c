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
/* d3_mir.h types d3_MirDrawObject / d3_MirBeginFrame with D3MIRROR *; d3_engine.h keeps the D3OBJECT * views. */
#define d3_MirDrawObject d3_Unk4550e0_mirview
#define d3_MirBeginFrame d3_Unk4554b0_mirview
#include "d3_mir.h"
#undef d3_MirDrawObject
#undef d3_MirBeginFrame
#include "d3_light.h"
#include "data_views.h"
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

