/* gv.c -- the avatar table 0x60f310 (32 records of 0xf4 bytes, first name "neVmMaven") and the effect slots 0x6f1008 (8 of 0x170 bytes, "kugel" 0x611204). Starts at 0x4ed170 (the
   first user of the table 0x60f310, which begins the .data block after ahm.c's "ahm_AllocAhmInfo" 0x60f2f8); the end is unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/gv.c); boundaries in decl/text.dcl, notes in src/gv/open.md */
extern char ai_PlayerSlots[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "data_views.h"
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
extern char ai_Unk91f700[];
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern unsigned char ai_Unk6f1b88;
extern GAPDATA full_data_6f1ce4[];
extern GAPDATA full_data_91f6e3[];
extern GAPDATA full_data_91f6e4[];
extern unsigned char amt_Table;
extern char gm_PlayerRecords[];

extern const float unk_Tab5ee034[];
int gm_ConvertMoney(int a, unsigned char k);   /* 0x5085b0 */
int gm_PlayerFactor(int a, unsigned char k);   /* 0x5085f0 */
int gm_Unk509e30(void *list, unsigned char p);   /* 0x509e30 */
int ls_GetLanguage();                     /* 0x5190a0 */
void ai_Unk4c5fa0(int a, int b, int c);                     /* 0x4c5fa0 */
int amt_FindOffice();                     /* 0x4e6030 */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);                     /* 0x4e6680 */
void ai_Unk4c4fa0(void *p);                     /* 0x4c4fa0 */
extern char *ai_JudgeCatholic, *ai_JudgeProtestant;   /* 0x6f1bfc / 0x6f1c00 */

#define UNK_FW ((unsigned (*)())vfs_fwrite)
#define UNK_FR ((unsigned (*)())ls_Read)

