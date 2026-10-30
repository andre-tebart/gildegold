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
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define d2_LastWindowPtrC (*(char **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) read bytewise, as this unit does */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define d3sm_GetVertexI ((int (*)())d3sm_GetVertex)  /* 0x45f100 (d3_sm.c: char, D3SM *, int, int, D3VECTOR *): @original this unit's callers read an int result (byte-matched unk_Unk4f86a0 (ord.c): no 0f be c0 after the call) */
#define ch_FindFreeCellK ((int (*)())ch_FindFreeCell)  /* 0x4072a0 (ch.h: char, 9 parameters): @original cm.c's call at 8227 passes 8 arguments without a prototype and reads an int */
#define ch_AddActionCBK ((struct CHACTION *(*)())ch_AddActionCB)  /* 0x40b400 (ch.h): @original cm.c's calls were compiled without a prototype (default promotions) */
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */
#define ahm_CreateAlchemistI ((int (*)(int *))ahm_CreateAlchemist)  /* 0x4ec500 (ahm.h: unsigned short): @original the caller 0x4ccf90 reads the id as an int (no and eax,0xffff) */

