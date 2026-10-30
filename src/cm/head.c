/* cm.c -- the game messages exchanged with the server (net.c): the size of a message by its type, the send
   ring ai_Unka69140 (0x8000 slots) and its states, the receive list ai_Unka68d24 (0x2000 records), long messages split
   into parts of type 7, the field blocks of an object (delta / plain values). Its .data block starts at 0x60bd80 after
   f3snd.c's literals (0x302, 2, -1 globals, the handler table 0x60bd98 of 0x4c6a20..). Tag "cm_ExecCommand" (0x4c4190,
   its texts at 0x60c0e8..). Names are ours.
   unit of rea 8cef7d5b (src/game/cm.c); boundaries in decl/text.dcl, notes in src/cm/open.md */
extern char d2_Wins[];
extern char d2_Radios[];
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "net.h"
#include "cm.h"
#include "d3.h"
#include "d3_quad.h"
#include "d3_light.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
extern char ai_GfxFloor[];
extern char ai_Unk92cae0[16][0x35c];
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_930370[];
extern char gm_PlayerRecords[];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_60d5a4[];
extern GAPDATA full_data_60d5b4[];
extern GAPDATA full_data_92bcb8[];

int ai_AlwaysTrue(void);                 /* the handler tables at 0x60bd98 / 0x60bf40 */
int sv_ReturnZero(void);
void sv_UpdateClock(void);
void unk_Unk511600(int s);

#define G17_I(m, o) (*(int *)&(m).data[o])
#define G17_S(m, o) (*(short *)&(m).data[o])
/* the block at 0x936420 is 0x24 bytes: the count then the eight player states at +4 (one object, so the
   0x24-byte memset / memcpy in 0x4cd580 and 0x4cdxx cannot run over a neighbour that LINK placed there) */
#define ai_Unk936424 (*(int (*)[8])((char *)ai_Unk936420 + 4))  /* 0x936424, the eight player states (a view) */

#include "time_view.h"

