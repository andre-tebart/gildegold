/* d3_interface
   unit of rea 8cef7d5b (src/game/d3_interface.c); boundaries in decl/text.dcl, notes in src/d3_interface/open.md */
#line 1 "D:\\develop\\Units\\d3\\d3_interface.c"
/* d3_interface.c -- "D:\develop\Units\d3\d3_interface.c" (string at 0x5fbd94): the engine's object API
   (creating, deleting, showing objects) and the scene switch. Starts at 0x446e10 (after the d3_fl TU,
   whose last function is d3fl_SetShift at 0x446dc0); the extent beyond 0x447e60 is not known here.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
#define d3_SetBlendState d3_SetBlendState_int
#define d3_SetParent d3_SetParent_void      /* d3.h declares it void; it returns char */
#include "d3.h"
#undef d3_SetBlendState
#undef d3_SetParent
#include "d3_vec.h"
#include "d3_engine.h"
/* d3_mir.h types d3_MirDrawObject / d3_MirBeginFrame with D3MIRROR *; d3_engine.h keeps the D3OBJECT * views. */
#define d3_MirDrawObject d3_Unk4550e0_mirview
#define d3_MirBeginFrame d3_Unk4554b0_mirview
#include "d3_mir.h"
#undef d3_MirDrawObject
#undef d3_MirBeginFrame
#include "d3_d3d.h"
#define vfs_Root vfs_Root_void              /* d3_fl.h's declarations clash with vfs.h */
#define vfs_DirPath vfs_Unk42fbf0_void
#include "d3_fl.h"
#undef vfs_Root
#undef vfs_DirPath
#include "d3_interface.h"
#include "bio.h"
#include "d3_light.h"
#include "data_views.h"
#define d3_PointToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_PointToRef)  /* 0x458b20 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (unk_458810.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */
extern char ai_Unk6339db[];

struct D3IOOBJ *d3io_LoadObject(char *a, char *b);  /* 0x44e080, d3_io_t.c (d3_io_t.h cannot be included here: d3_fl.h vfs views) */
void d3fl_FreeMap(D3FLMAP *map);  /* 0x43ff90, d3_fl.c; declared here because a d3_fl.h prototype perturbs d3_tools.c's d3t_UpdateSunRays */

