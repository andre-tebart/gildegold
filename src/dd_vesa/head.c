/* dd_vesa
   unit of rea 8cef7d5b (src/game/dd_vesa.c); boundaries in decl/text.dcl, notes in src/dd_vesa/open.md */
#line 1 "D:\\develop\\Units\\dd_vesa\\Dd_vesa.c"
/* dd_vesa.c -- D:\develop\Units\dd_vesa\Dd_vesa.c (leaked assert path), the DirectDraw display layer:
   driver and device enumeration (with a selection dialog from d3_resource.dll), mode setup in a window,
   fullscreen or through GDI, locking, clearing, flipping and pixel format helpers.
   Extent in GildeGold.exe: 0x41bdd0..0x41ebc0. Functions are kept in their original order. Names are ours
   unless quoted in dd_vesa.h.
   @named_by leaked
   @boundary plausible */
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3.h"
#include "dd_vesa.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRIGGER / REA_DUMP_BACKBUFFER */


#endif
#include "data_views.h"
extern char txt_Keys[];

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern signed char ai_Relations[768][768];

/* 0x418ce0..0x41bdd0: unk_418ce0.c until layout 2026-10-22 (character to key code table, DirectDraw / Direct3D
   error text); joined: the .data block starts with dd_vesa's -1 at 0x5f3c48, then d3d_Error's texts, then the
   Dd_vesa.c path 0x5f65e0 (one block). */
