/* sndc.c -- original file name unknown: sound script commands (see include/sndc.h). Starts at
   0x40ffc0 and ends with the registration 0x410220. Names are ours.
   unit of rea 8cef7d5b (src/game/sndc.c); boundaries in decl/text.dcl, notes in src/sndc/open.md */
#include <stdio.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "sndc.h"

/* script constants registered by sndc_Register (0x5f3108..0x5f3124) */
int snd_W2 = -1, snd_W1 = -2, snd_M3 = -3, snd_M2 = -4, snd_M1 = -5, snd_HS = -6, snd_NONE = -7, snd_VAR = -8;  /* 0x5f3108 */

