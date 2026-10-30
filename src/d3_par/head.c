/* d3_par.c -- particle systems ("d3_par:" allocation tags, 0x455630..0x457c30), original file name
   unknown: the list handling, the particle draw function, the three emitter callbacks and
   par_CreateEmitter. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_par.c); boundaries in decl/text.dcl, notes in src/d3_par/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_vec.h"
/* d3_engine.h declares the list head / tail pointers as void *; d3_par.h has the real types */
#define d3_ParHead d3_ParHead_engine
#define d3_ParTail d3_ParTail_engine
#include "d3_engine.h"
#undef d3_ParHead
#undef d3_ParTail
#include "mem.h"
#include "ecmd.h"
#include "d3_par.h"
#include "data_views.h"
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (unk_458810.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */

