/* hst.c -- the history texts (0x4adf40..0x4b39f0, after hmt.c): character predicates, the text tokens and text
   commands of a history text, its parser (the hst_* names are leaked by its error texts), the date labels and the
   yearly history dialogs. Its .data block starts at 0x601058. Names are ours except the hst_* ones.
   unit of rea 8cef7d5b (src/game/hst.c); boundaries in decl/text.dcl, notes in src/hst/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hst.h"
#include "txt.h"
typedef unsigned int SOCKET;            /* <winsock.h>, for net.h (as ai.c) */
#include "net.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define gm_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* 0x4fca40 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_CommandKillPlayer / ai_Unk4b86c0) */
#define d2_LastWindowPtrV (*(G12WIN * (*))&d2_LastWindowPtr)  /* 0x6ed980, this unit reads it as `G12WIN *d2_LastWindowPtr` */
