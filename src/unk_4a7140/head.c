/* unk_4a7140.c -- the card poker game of the tavern against two NPCs (0x4a7140..): the game record 0xf83654,
   the deck 0xf83660 and the two opponents 0xf837e0. The am unit ends before it (its last .data string is
   "%sgebaeude/*%s_%c.ogr" 0x600478; this unit's block starts with the hand names 0x600490). Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4a7140.c); boundaries in decl/text.dcl, notes in src/unk_4a7140/open.md */
#include "txt_views.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_4a7140.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */

/* full rebuild: the symbols whose original addresses the code above spells literally */
#include "cm_types.h"
extern G17MSG ai_Unka69140[];


