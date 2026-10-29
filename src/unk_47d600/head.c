/* unk_47d600.c -- 0x47d600.., the shape drawing loops for 8 bit shapes, written in inline assembly; it ends with
   0x47d960 (moved here from unk_47d960.c, round 20). Uses unk_47c5e0.h. Names are ours. Kept apart from shp.c by
   subject only: the 0xcc before 0x47d600 is the linker's fill after d2_ShpBlit16ClipXY's unpadded section (its pragma),
   not an object end (corrected 2026-10-23).
   unit of rea 8cef7d5b (src/game/unk_47d600.c); boundaries in decl/text.dcl, notes in src/unk_47d600/open.md */
#include "data_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d2.h"
#include "shp.h"
#include "unk_47c5e0.h"

