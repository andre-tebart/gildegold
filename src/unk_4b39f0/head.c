/* unk_4b39f0.c -- missions ("_AUFTRAEGE_VERGABE_HS_%.2d", "special\mission"): the running missions
   ai_Missions, the mission types ai_MissionTypes (.data right after hst.c's literals), an own random generator.
   Unit start and name are guesses (open boundary). Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4b39f0.c); boundaries in decl/text.dcl, notes in src/unk_4b39f0/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_4b39f0.h"
#include "di_input.h"
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define ai_GameStateB (*(unsigned char (*))&ai_GameState)  /* 0x6f25ac, this unit reads it as `unsigned char ai_GameState` */
#define d2_ObjsV (*(G13OBJ * (*))&d2_Objs)  /* 0xf8f080, this unit reads it as `G13OBJ *d2_Objs` */
