/* unk_4b6480.c -- message callbacks: each one writes text 0x12e7 + type for a character and sends it (0x53dda0);
   called through the table ai_Unk602570 by 0x4bb6c0. Its .data (the table) follows unk_4b39f0.c's literals; unit
   start and name are guesses (open boundary). Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4b6480.c); boundaries in decl/text.dcl, notes in src/unk_4b6480/open.md */
#include <string.h>
#include <windows.h>
#include "unk_4b6480.h"
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define he_MessageBoxQuickjumpI ((void (*)(int, int, char *, int, int, int, int, int, int))he_MessageBoxQuickjump)  /* 0x53dee0 (he.c: char fl): @original this unit builds the flag as an int (byte-matched ai_Unk4baa80 / ai_Unk4baba0) */
#define gm_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* 0x4fca40 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_CommandKillPlayer / ai_Unk4b86c0) */
#define cm_SendExSetHEI ((void (*)(int, void *))cm_SendExSetHE)  /* 0x4c4c60 (cm.h: unsigned char k): @original the callers push the key as an int (byte-matched ai_Unk4b8c80) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_Unk497c80 / ai_Unk49c880) */
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_600c00[];
extern GAPDATA full_data_600c01[];

