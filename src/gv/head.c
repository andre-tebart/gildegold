/* gv.c -- the avatar table 0x60f310 (32 records of 0xf4 bytes, first name "neVmMaven") and the effect slots 0x6f1008 (8 of 0x170 bytes, "kugel" 0x611204). Starts at 0x4ed170 (the
   first user of the table 0x60f310, which begins the .data block after ahm.c's "ahm_AllocAhmInfo" 0x60f2f8); the end is unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/gv.c); boundaries in decl/text.dcl, notes in src/gv/open.md */
extern char ai_PlayerSlots[4][0x2f4];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
int d3t_CreateSmoke();  /* 0x416340, d3_tools.h: struct D3PARTSYS *(.., float .., struct D3COLOR); @original gv.c (0x4ee360) calls it without a prototype, its float operands as dword bit patterns (gv.c has no D3COLOR: d3.h is not in this unit) */
int d3t_CreateEarth();  /* 0x4168e0, d3_tools.h: as d3t_CreateSmoke; @original the same call without a prototype (0x4ee360) */
#define d3t_CreateExplosionK ((int (*)())d3t_CreateExplosion)  /* 0x416040 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateEarthK ((int (*)())d3t_CreateEarth)  /* 0x4168e0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateSmokeK ((int (*)())d3t_CreateSmoke)  /* 0x416340 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_GetMeshBoundsK ((int (*)())d3t_GetMeshBounds)  /* 0x413310 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
extern char ai_Unk91f700[];
#define GAPDATA __declspec(align(1)) unsigned char
#define ai_Unk5a9e40K ((int (*)())ai_Unk5a9e40)  /* 0x5a9e40: @original the call passes more arguments than the draft definition names, compiled without a prototype (as before the definition's spelling reached this unit) */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int ai_Unk6f1b88;
extern GAPDATA full_data_6f1ce4[];
extern GAPDATA full_data_91f6e3[];
extern GAPDATA full_data_91f6e4[];
#include "amt_types.h"
extern AMTREC amt_Table[];
extern int gm_PlayerRecords[688];

extern const float unk_Tab5ee034[];
int gm_ConvertMoney(int a, unsigned char k);   /* 0x5085b0 */
int gm_PlayerFactor(int a, unsigned char k);   /* 0x5085f0 */
int gm_Unk509e30(void *list, unsigned char p);   /* 0x509e30 */
int ls_GetLanguage();                     /* 0x5190a0 */
void ai_Unk4c5fa0(int a, int b, int c);                     /* 0x4c5fa0 */
int amt_FindOffice();                     /* 0x4e6030 */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);                     /* 0x4e6680 */
void ai_Unk4c4fa0(void *p);                     /* 0x4c4fa0 */
extern char *ai_JudgeCatholic;  /* 0x6f1bfc */
extern char *ai_JudgeProtestant;  /* 0x6f1c00 */

#define UNK_FW ((unsigned (*)())vfs_fwrite)
#define UNK_FR ((unsigned (*)())ls_Read)

