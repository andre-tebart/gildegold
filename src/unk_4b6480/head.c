/* unk_4b6480.c -- message callbacks: each one writes text 0x12e7 + type for a character and sends it (0x53dda0);
   called through the table ai_Unk602570 by 0x4bb6c0. Its .data (the table) follows unk_4b39f0.c's literals; unit
   start and name are guesses (open boundary). Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4b6480.c); boundaries in decl/text.dcl, notes in src/unk_4b6480/open.md */
#include <string.h>
#include <windows.h>
#include "unk_4b6480.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_600c00[];
extern GAPDATA full_data_600c01[];

