/* io_text.c -- the text builders and the text engine (0x5142e0 .. 0x519010): the tag "io_Text():" ("io_Text(): Missing Random-Text (e.g. {r1})...", "io_Text(): too many icons before redraw!", 0x6235d8 / 0x623618); the names of the characters with the "$A" markup (0x5143d0 / 0x514e90), the town display name (0x5142e0, "_STADTAUSWAHL_%s_INFO+0" 0x623564). The block starts where f3init.c's string block ends (0x623564, after 0x513970's strings) and ends before the language getter 0x519010 (its global 0x62364c starts the "ls" unit: "ls:tmp" 0x623654). Drafts of layout 2026-10-23.
   unit of rea 8cef7d5b (src/game/io_text.c); boundaries in decl/text.dcl, notes in src/io_text/open.md */
extern char d2_Wins[];
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gv.h"
#include "txt.h"
#include "data_views.h"

struct GCLOCK;
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);  /* 0x4fe2c0, f3gm.c: the calendar record of a clock */

extern char ch_EmptyName[];             /* 0x631090 */
extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, the player slots */
extern int __mb_cur_max;                /* 0x62e3a0, the CRT multibyte state (MB_CUR_MAX of ctype.h) */
extern unsigned short *_pctype;         /* 0x62e194, the CRT ctype table (the 0x100 entry values) */

