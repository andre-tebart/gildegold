/* d3_io_fl.c -- d3_io_fl: the fast-chunk BGF loader ("d3_io_fl:LoadFastChunk"), mesh helpers and the
   fast-load chunk writer 0x451250 ("d3_io_fl:AddFLChunk(obj)"): 0x450860..0x451e7c (0x4506f0 / 0x4507a0 may
   open the TU; see d3_io_b.c). Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_fl.c); boundaries in decl/text.dcl, notes in src/d3_io_fl/open.md */
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

