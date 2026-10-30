/* rd.c -- tag "rd_" (rd_AllBauplatzToSupermap, rd_GetBKSize in its error texts): helpers that place animals (a building's "dummy_TUER", free cells near a point, a random dummy
   by name), then the vegetation map and the roads. Start: the first user of "dummy_TUER" (.data 0x602854, right
   after anm.c's literals); the road code 0x4bcc20 uses the same literal (no cross-obj pooling in this exe, so one
   obj). The .bss gap 0x6f0744 after anm's globals hints at a new obj here. Names are ours.
   unit of rea 8cef7d5b (src/game/rd.c); boundaries in decl/text.dcl, notes in src/rd/open.md */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "anm.h"
#include "f3gm.h"
#include "d3.h"
#include "d3_light.h"
#include "data_views.h"
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (as ai.h: unsigned short (int)): this unit's callers were compiled against the 16-bit parameter (as ahm.c's) */

