/* f3snd.c -- tag "f3snd: snd_AddToQueue" (mem_Alloc, 0x4c0230); the sound queue and ambient sounds: the playing list snd_Queue, the gusts ("Boehe_Leicht_s", "Boehe_Mittel_s",
   "Boehe_Stark_s", .data 0x602b10 right after rt.c's "rt_load"). Start: the first function after rt.c's
   save / load; no module tag. Names are ours.
   unit of rea 8cef7d5b (src/game/f3snd.c); boundaries in decl/text.dcl, notes in src/f3snd/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "f3snd.h"

/* d3_Time * 13, spelled so that the volatile is read three times */
#define F3SND_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

/* stdlib min / max (the body ai.h defines; f3snd cannot include ai.h, its AITIME ai_Time view differs) */
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#include "data_views.h"

