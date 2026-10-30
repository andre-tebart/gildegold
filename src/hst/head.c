/* hst.c -- the history texts (0x4adf40..0x4b39f0, after hmt.c): character predicates, the text tokens and text
   commands of a history text, its parser (the hst_* names are leaked by its error texts), the date labels and the
   yearly history dialogs. Its .data block starts at 0x601058. Names are ours except the hst_* ones.
   unit of rea 8cef7d5b (src/game/hst.c); boundaries in decl/text.dcl, notes in src/hst/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hst.h"
#include "txt.h"
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define gm_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* 0x4fca40 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_CommandKillPlayer / ai_Unk4b86c0) */

