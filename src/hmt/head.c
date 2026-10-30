/* hmt.c -- historical events ("hmt_CreateBinaryFile", historical.ini / historical.dfn), 0x4ac140..; name from
   the "hmt_" tag. The table of 51 records at 0x5ed1e8 (0x4ac140..0x4ac2f0: size, random picks, text ids 0x1e2b /
   0x1e35 + n, prices, applying a record) has no .data; it is put here because the .data block of 0x4ac440.. starts
   at 0x600f60 right after unk_4a7140.c's ("Blutrache" 0x600f54). Starts after the registration 0x4abed0 that ends
   unk_4a7140.c. Names are ours.
   unit of rea 8cef7d5b (src/game/hmt.c); boundaries in decl/text.dcl, notes in src/hmt/open.md */
extern char ai_PlayerSlots[4][0x2f4];
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmt.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#include "cm_types.h"
extern G17STAT ai_Unkf31160[];

/* the SDK calls of hmt_CreateBinaryFile; windows.h is not included (its min/max and the other SDL macros change the
   emitted code of the unit), the two entry points are declared here as the SDK does, __stdcall */
__declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void *hModule, char *file, unsigned long size);
__declspec(dllimport) unsigned long __stdcall GetPrivateProfileStringA(const char *sec, const char *key,
                                                const char *def, char *buf, unsigned long size, const char *file);

/* ai.h declares cm_AddFieldValues with an int field; the caller 0x4ac2f0 passes an unsigned short (no masking) */
#define CM_AddFieldValuesShort ((void (*)(int, int, unsigned short, void *))cm_AddFieldValues)

