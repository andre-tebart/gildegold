/* sfx.c -- original file name unknown (mem_Alloc tag "sfx_OpenDigOutput").
   Digital sound output on top of the Miles Sound System: up to 16 waveOut drivers, each with a table of
   sample and stream handles. Extent in GildeGold.exe (inferred): starts at 0x42b720 after the event script
   interpreter and ends at 0x42c398 (sfx_NumDrivers), followed by the mss32 import thunk
   AIL_get_timer_highest_delay at 0x42c3a0. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/sfx.c); boundaries in decl/text.dcl, notes in src/sfx/open.md */
#include <windows.h>
#include "sfx.h"

