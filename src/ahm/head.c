/* ahm.c -- the alchemist building and the recipes (name from the tags "ahm_AllocRecipePrototypes" / "ahm_AllocPlayerPrototypes": the data block 0x60f1b0 with the
table of 8 ints, then the strings "RCPID" .. "alchemy_data.ini"). Starts at 0x4eaa50 (the first function of the building kind 0x1e); the end is unknown.
   Names are ours.
   unit of rea 8cef7d5b (src/game/ahm.c); boundaries in decl/text.dcl, notes in src/ahm/open.md */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ahm.h"
#include "f3gm.h"
#include "net.h"

unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: the function masks its argument to 16 bits (0x42ec00: test si,si; and esi,0xffff) and the shuffler passes the count through mov cx,WORD PTR ds:0x6f0e28 (0x4eadd8), so this TU's callers were compiled against the 16-bit parameter and result (ai.h's view is int) */

