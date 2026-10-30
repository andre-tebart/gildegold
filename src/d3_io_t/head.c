/* d3_io_t.c -- BGF mesh loader and object / animation file loading of the d3 scene I/O ("d3_io_t:" / "d3_io:"
   allocation tags), 0x44cd40..0x44e570; tag "d3_int_io:" (d3_int_io.c) is a separate TU. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_t.c); boundaries in decl/text.dcl, notes in src/d3_io_t/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_io_t.h"
#include "d3_io_fl.h"
#include "data_views.h"
#include "d3_int_io.h"
#include "d3_anim.h"
#define d3anim_NextFrameIo ((int (*)(D3IOANIMFLAGS, int, int, int, int))d3anim_NextFrame)  /* 0x4324e0 (d3_anim.h: D3ANIMFLAGS f): @original this unit passes its field-wise D3IOANIMFLAGS (12 bytes), as the original TU did */
extern D3IOANIMDATA d3_AnimHead;
extern D3IOOBJ d3_StockHead;

