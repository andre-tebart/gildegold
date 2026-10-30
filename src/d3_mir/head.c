/* d3_mir.c -- mirrors ("d3_mir:" allocation tags), original file name unknown: 0x454780..0x455630.
   The outline of the mirror polygon becomes a set of clipping planes; the objects whose mirrored bounding box
   is inside them are drawn a second time, mirrored at the mirror plane. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_mir.c); boundaries in decl/text.dcl, notes in src/d3_mir/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
/* d3_engine.h declares these two with other parameter types */
#define d3_MirBeginFrame d3_Unk4554b0_decl
#define d3_MirDrawObject d3_Unk4550e0_decl
#include "d3_engine.h"
#undef d3_MirBeginFrame
#undef d3_MirDrawObject
#include "d3_mir.h"
#include "data_views.h"
#define d3_TriNormalM ((void (*)(D3VECTOR *, D3PVERT *, D3PVERT *, D3VECTOR4 *))d3_TriNormal)  /* 0x454700 (d3_vec.h: D3VECTOR4 * x4): this unit passes the origin and its polygon vertices */
#define d3_DirToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_DirToRef)  /* 0x458950 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */

#define D3M_SURF(o) ((D3MIRSURF *)(o)->info)
#define D3M_BITS(o) (*(D3MIRBITS *)&(o)->objectFlags)
#define D3M_PTS(mesh) ((mesh)->mirrorPoints)                     /* "d3_mir:MirroredPoints" */
#define D3M_TRIS(mesh) ((mesh)->mirrorPolys)                     /* "d3_mir:MirroredPolys" */
#define D3M_SRC(mesh) (*(D3MIRSURF **)&(mesh)->mirrorSource)    /* the surface they were made from */

