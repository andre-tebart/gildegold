/* mem.c -- original file name unknown ("m_alloc_debug", "m:pool_alloc(tp_m_pool)").
   Tracked allocations: every block is recorded in a fixed table and counted per group, the tag prefix
   up to ':'. Pools hand out fixed-size zeroed slots from linked blocks.
   Extent in GildeGold.exe (inferred, not proven): 0x4223c0..0x422c90.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/mem.c); boundaries in decl/text.dcl, notes in src/mem/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "err.h"
#include "mem.h"

