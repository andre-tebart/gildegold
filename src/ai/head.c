/* ai.c -- the AI unit (0x47d9e0..0x48e070): the character update (needs / mood, 0x47dab0), the partner search
   and the per-frame loop over the 768 characters (0x47e560), the dispatcher that runs a master's evaluation by
   building kind (0x47e990), and the masters' evaluations: the production masters (0x47edb0, 0x47f700, 0x4801f0,
   0x483fc0, 0x484a60), ai_CalcMeisterWache (0x480e70), ai_CalcAngriff (0x4817a0), ai_CalcMeisterDiebe (0x481ef0),
   ai_CalcMeisterRaeuber (0x482f00), the bank master (0x4836a0), the entry / slot tables (0x4854f0 .. 0x487ea0),
   the purchases and the order dispatcher (0x487fb0 .. 0x489870), ai_MeisterBewaffnen (0x489a70),
   ai_MeisterCollectTrans (0x489cb0), ai_MeisterTrade / ai_MeisterTradeAllgemein (0x48a4d0, 0x48bc80) and
   ai_MeisterTradeFernEinkauf (0x48d010, 0x48d810). Module tag "ai_" (messages "ai_Calc...(): ...").
   TU evidence (round 21): the unit's .data starts at 0x5fef04 (0x5fef00 belongs to the blit code before it) and
   its string literals 0x5fef0c..0x5ffd18 follow the function order without a new data block; 0x5ffd1c starts the
   next obj. Joined in round 21 from unk_47d960.c, unk_47ec20.c, unk_4817a0.c, unk_4854f0.c and unk_48a4d0.c, in
   round 22 from unk_483fc0.c, unk_4864b0.c, unk_487fb0.c and unk_489a70.c. Names are ours except the ones taken
   from the strings.
   unit of rea 8cef7d5b (src/game/ai.c); boundaries in decl/text.dcl, notes in src/ai/open.md */
extern int gm_PlayerRecords[688];
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int SOCKET;
#include "net.h"
#include "ai.h"
#include "data_views.h"
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define he_MessageBoxQuickjumpI ((void (*)(int, int, char *, int, int, int, int, int, int))he_MessageBoxQuickjump)  /* 0x53dee0 (he.c: char fl): @original this unit builds the flag as an int (byte-matched ai_Unk4baa80 / ai_Unk4baba0) */
#define gm_Unk505e60S ((int (*)(unsigned short, int))gm_Unk505e60)  /* 0x505e60 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_Unk494a30 / ai_Unk494ac0 / ai_Unk494d40) */
#define cm_SendRemoveGoodsI ((void (*)(int, int))cm_SendRemoveGoods)  /* 0x4c4960 (cm.h: short b): @original ai.c pushes the second argument as an int (byte-matched ai_SendRemoveEmptyGoods) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_Unk497c80 / ai_Unk49c880) */
extern char d3_Scenes[];
extern char ai_Unkf83ec0[];

/* full rebuild: the symbols whose original addresses the code above spells literally */

