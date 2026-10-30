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
extern char ai_PlayerSlots[4][0x2f4];
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
extern unsigned char txt_Keys;  /* @original ai_Unk498c70 (0x498c70, byte) tests flags1c8 & 0x1000000 as &txt_Keys + 0x181c0 of this byte view: the char[0x4800][0x50] spelling, a view macro or the literal flip its add operands at +0x4cc (shape) */
extern unsigned char he_Events[339968];

int gm_Unk5044b0(int i, int p);        /* 0x5044b0, @original: the byte-matched callers (ai_Unk49ed10, ai_Unk498c70) read the result without the unsigned char mask (no `and eax,0xff` after the call) */

int ai_Unk5ffd1c = -1;                  /* 0x5ffd1c */
G6RING * ai_Unk60034c = ai_Unkf83c20;  /* 0x60034c, the next entry */
int ai_Unk600350 = 1;  /* 0x600350, the step */
int ai_Unk6f0470 = 0;  /* 0x6f0470, number of characters of kind 5, 6, 7 */
AGCH ai_Unk6f0478 = {0};  /* 0x6f0478 */
G9SHOT ai_Unk6f0698 = {0};  /* 0x6f0698 */
unsigned int ai_Unk6f070c = 0;  /* 0x6f070c, the number of entries */
int ai_Unk6f2464 = 0;  /* 0x6f2464, set: write the action table */
char ai_DataPath[608] = {0};  /* 0x75dca0, the data path */
/* ai_Unk7e3f80: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e3f84: view of ai_PlayerSlots (tools/data_views.py) */
/* ai_Unk7e4274: view of ai_PlayerSlots (tools/data_views.py) */
G6RING ai_Unkf83c20[32] = {0};  /* 0xf83c20 */
char ai_Unkf83da0[260] = {0};  /* 0xf83da0, the ini file of the actions */
int ai_Unkf83ea4 = 0;  /* 0xf83ea4, the depth of the evaluation stack */
AIACT ai_Unkf83ec0[36] = {0};  /* 0xf83ec0, the registered actions */
/* ai_Unkf84c40: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84c60: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84cd4: view of ai_Unkf83ec0 (tools/data_views.py) */
/* ai_Unkf84cf4: view of ai_Unkf83ec0 (tools/data_views.py) */
float ai_Unkf86894 = 0;  /* 0xf86894 */
float ai_Unkf868b4 = 0;  /* 0xf868b4 */
int ai_Unkf86900[73] = {0};  /* 0xf86900, the actions being evaluated */

#include "time_view.h"
#define d3t_GetObjectBoundsV ((void (*)(void *, void *, void *))d3t_GetObjectBounds)  /* 0x413150 (d3_tools.c: D3VECTOR *, D3VECTOR *, D3OBJECT *): this unit's untyped view */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define unk_Unk4f0e80I ((void (*)(AGCH *, int, void *, int, int, int))unk_Unk4f0e80)  /* 0x4f0e80 (gv.c: char b): @original the byte-matched ai_Unk490b90 loads s2->kind as a dword (8b 51 10, not 8a) */
#define gm_Unk505e60S ((int (*)(unsigned short, int))gm_Unk505e60)  /* 0x505e60 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_Unk494a30 / ai_Unk494ac0 / ai_Unk494d40) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_Unk497c80 / ai_Unk49c880) */
#define amt_GetKindB ((int (*)(unsigned char, void *))amt_GetKind)  /* 0x4e6f80 (amt.h: int k): @original ai_Unk4a05e0 passes (unsigned char)k without widening */

