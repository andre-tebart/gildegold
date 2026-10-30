/* ahm.c -- the alchemist building and the recipes (name from the tags "ahm_AllocRecipePrototypes" / "ahm_AllocPlayerPrototypes": the data block 0x60f1b0 with the
table of 8 ints, then the strings "RCPID" .. "alchemy_data.ini"). Starts at 0x4eaa50 (the first function of the building kind 0x1e); the end is unknown.
   Names are ours.
   unit of rea 8cef7d5b (src/game/ahm.c); boundaries in decl/text.dcl, notes in src/ahm/open.md */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ahm.h"
#include "f3gm.h"
#include "net.h"
#include "vfs.h"
#define ai_TimeK (*(AHMCLOCK *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as ahm.c copies it: @original 3 ints and a short (0x4eb43a lea edx,[eax+0x52]; stores 4/4/4/2) */
#define cm_SendExSetHEI ((void (*)(int, char *))cm_SendExSetHE)  /* 0x4c4c60 (cm.h: unsigned char k): @original ahm.c pushes the key as an int (0x4eb43e push 0xfffffffe) */

#define vfs_freadU ((unsigned (*)(void *, int, int, VFSFILE *))vfs_fread)  /* 0x430bd0, vfs.h int: @original this unit compares the count as unsigned (ahm_LoadDfn 0x4ebabd test eax,eax; jbe for `<= 0`) */

unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: the function masks its argument to 16 bits (0x42ec00: test si,si; and esi,0xffff) and the shuffler passes the count through mov cx,WORD PTR ds:0x6f0e28 (0x4eadd8), so this TU's callers were compiled against the 16-bit parameter and result (ai.h's view is int) */

