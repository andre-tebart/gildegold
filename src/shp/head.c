/* unk_47ab30.c -- 0x479d00..0x47c5e0, the shape ("shp") TU of the d2 layer (memory tags "d2:shp:...",
   "shp_t:..."): light table and colour masks, grabbing a rectangle of a surface into a run-length coded shape
   (8, 16 or 24 bit), clipped drawing, conversion between 8 / 16 / 24 bit shapes and of whole banks. Its own TU:
   its .data starts at 0x5fed74 (colour keys, 640 / 480 / 800) after the GFX.C strings, the strings of
   0x479d00.. follow it; d2_ShpConvert24To16 gets closer (595 -> 303 bytes) once d2_SetLineTable is defined above it
   (round 20). The TU runs on to 0x47d210 (the 0xcc after it is the linker's fill after that function's unpadded
   section, see its pragma; not an object end): 0x47c5e0.. adds shapes to a bank, draws shapes of a
   bank, animated shapes, two 16 bit drawing loops in inline assembly (joined from unk_47c5e0.c, round 20). Names
   are ours.
   unit of rea 8cef7d5b (src/game/shp.c); boundaries in decl/text.dcl, notes in src/shp/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "err.h"
#include "di_input.h"
#include "txt.h"
#include "dd_vesa.h"
#include "tb.h"
#define d2_DrawShapeHalfDark d2h_Unk47c770_void
#define d2_DrawShapeMode2 d2h_Unk47c970_void
#define d2_StartAnim d2h_Unk47cd20_int
#include "d2.h"
#undef d2_DrawShapeHalfDark
#undef d2_DrawShapeMode2
#undef d2_StartAnim
#include "shp.h"
#include "unk_47c5e0.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRACE */
#endif
#include "data_views.h"

int d2_ColourFormatSet = 0;  /* 0x6f0450 */
unsigned int d2_HighBitMask32;  /* 0xf8a640 */
extern int d2_PaletteDepth;                    /* 0xf8a64c */

