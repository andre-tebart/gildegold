/* unk_476c00.c -- 0x476c00..0x479610 of the "d2" 2D layer, probably D:\develop\Units\d2.v1\GFX.C (its own TU,
   not d2.c's): the item list shown next to the pointer (id / amount pairs, drawn with the pointer picture), pointer
   picture, hover and scroll wheel handling of windows, the surfaces of the item list, editing of input fields
   (keys, mouse drag, slider objects), input focus, mouse handling and hit rectangles, 24 bit bmp loading, and the
   GFX.C surfaces (create, free, lock, unlock, blits). One TU: d2_FocusInput matches only with d2_CountPointerItems defined
   above it; the GFX.C __LINE__ 0x43f inside d2_MakeSurface fits a file that starts about at 0x476c00 (round 20).
   Names are ours.
   unit of rea 8cef7d5b (src/game/gfx.c); boundaries in decl/text.dcl, notes in src/gfx/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3.h"
#include "di_input.h"
#include "tb.h"
#include "txt.h"
#include "dd_vesa.h"
#include "gfx.h"
#include "unk_47c5e0.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRACE */
#endif
#include "data_views.h"
#define d2_MoveObjI ((void (*)(int, int, int))d2_MoveObj)  /* 0x46cda0 (d2.c: short x, short y): @original this unit passes the coordinates as ints (d2_CreatePicObj) */
extern char ai_Unkfa5f04[];

