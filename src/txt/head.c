/* txt.c -- original file name unknown ("txt_ParseRandomText", "txt_OpenText" strings).
   Text definition files: a definition file includes several text files whose lines are split into
   key/text fields and copied into one text pool. Extent in GildeGold.exe (inferred): starts at 0x42c3b0
   after the mss32 import thunk that ends sfx.c. Functions are kept in their original order.
   Names are ours except the two leaked ones.
   unit of rea 8cef7d5b (src/game/txt.c); boundaries in decl/text.dcl, notes in src/txt/open.md */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "txt.h"

