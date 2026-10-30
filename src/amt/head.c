/* amt.c -- the offices ("Aemter": the office table 0x925120 and its code; name from the tags "amt_calc_zuenfte()" 0x60f11c and
   "amt_fio_LoadAemter" 0x60f174), starts at 0x4e5cd0 (the init of the table; the cut_ code of cm.c ends with the debug dump 0x4e5bf0).
   The unit's .data starts at 0x60ec90 (a float table, first user 0x4e60b0). End unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/amt.c); boundaries in decl/text.dcl, notes in src/amt/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "amt.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int gm_PlayerRecords[688];

#define AMT_TABA_END ((int)&amt_TabA[15 * 10])  /* 0x5edee8, the end of the 15 rows of amt_TabA */
#define AMT_MONEY_UNIT 64  /* money in the character messages counts in multiples of this: the amounts of the 0x4c4830 sites are built as n * 0x40 */

#include "time_view.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */

