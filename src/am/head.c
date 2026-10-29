/* am.c -- the AI "methods" unit, 0x48e070..: its parts in address order.
   0x48e070..0x490b90 (was unk_48e070.c / unk_48f250.c / the head of unk_490830.c): AI action kinds 0x40 / 0x41 / 0x42 of the masters' upgrade decision
   ("building_upgrade"): decide callbacks (int, character, flag, out1, out2) fill two AIARG records and return the
   action kind (or 0), execute callbacks (character, arg1, arg2) carry it out. Starts the object after the AI
   unit (its first global, ai_Unk5ffd1c = -1 at 0x5ffd1c, precedes the string "building_upgrade").
   Names are ours.
   0x490c10..0x492fd0: the registration of the graveyard actions ("Graveyard Owner",
   "Handle Grave", "Use Seance", 0x490c10) and the decision callbacks of the AI decision table at 0x611258
   (29 records of 0x24 bytes: min, max, current value, callback at +0x20). A callback looks at a character
   (needs, money, game day) and, when the decision should change, stores the new value and returns 1.
   The TU starts before 0x490c10 (0x490690 / 0x490830 / 0x4908d0 / 0x490b90 are its callbacks). Names are
   ours.
   0x495ee0..0x497490 (was unk_495ee0.c): AI action callbacks. A decide callback (int, character, flag, out1, out2)
   fills two AIARG records and returns the action kind (or 0); an execute callback (character, arg1, arg2)
   carries the action out; an evaluate callback (score a, score b, action index, character, flag, arg1, arg2)
   rates it. 0x495ee0 sends every kind-3 character a random flag set for field 0x1e8. The registration code
   (0x4a1700..) stores these into AIACT records. Names are ours.
   unit of rea 8cef7d5b (src/game/am.c); boundaries in decl/text.dcl, notes in src/am/open.md */
extern char ai_PlayerSlots[];
#include "txt_views.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "am.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern unsigned char ai_Unka69140;
extern unsigned char txt_Keys;
extern unsigned char he_Events;

int gm_Unk5044b0(int i, int p);        /* 0x5044b0, @original: the byte-matched callers (ai_Unk49ed10, ai_Unk498c70) read the result without the unsigned char mask (no `and eax,0xff` after the call) */

int ai_Unk5ffd1c = -1;                  /* 0x5ffd1c */
G6RING * ai_Unk60034c = ai_Unkf83c20;  /* 0x60034c, the next entry */
int ai_Unk600350 = 1;  /* 0x600350, the step */
char ai_DataPath[608];  /* 0x75dca0, the data path */
/* ai_Unk7e3f80: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e3f84: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4274: view of ai_PlayerSlots (tools/data_views.py) */
G6RING ai_Unkf83c20[32];  /* 0xf83c20 */
char ai_Unkf83da0[260];  /* 0xf83da0, the ini file of the actions */
int ai_Unkf83ea4;  /* 0xf83ea4, the depth of the evaluation stack */
AIACT ai_Unkf83ec0[36];  /* 0xf83ec0, the registered actions */
/* ai_Unkf84c40: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84c60: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84cd4: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84cf4: view of ai_Unkf83ec0 (tools/data_views.py) */
float ai_Unkf86894;  /* 0xf86894 */
float ai_Unkf868b4;  /* 0xf868b4 */
int ai_Unkf86900[73];  /* 0xf86900, the actions being evaluated */
int ai_Unk6f0470 = 0;  /* 0x6f0470, number of characters of kind 5, 6, 7 */
AGCH ai_Unk6f0478 = {0};  /* 0x6f0478 */
G9SHOT ai_Unk6f0698 = {0};  /* 0x6f0698 */
unsigned int ai_Unk6f070c = 0;  /* 0x6f070c, the number of entries */
int ai_Unk6f2464 = 0;  /* 0x6f2464, set: write the action table */

