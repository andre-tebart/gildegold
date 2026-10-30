/* anm.c -- animals: module tag "anm:animals" (mem_Alloc tag), the animal names "DOG", "CAT", "BIRD", "COW", "SHEEP",
   "CHICK", "PIG", "HORSE" (.data 0x602688, right after unk_4b6480.c's table), "katze_KATZE", "hund_HUND", ..
   Start: 0x4bb810 (first user of the tag); 0x4bb720 / 0x4bb7d0 before it post random messages (unk_4b6480.c).
   Names are ours.
   unit of rea 8cef7d5b (src/game/anm.c); boundaries in decl/text.dcl, notes in src/anm/open.md */
#include <stdio.h>
#include <string.h>
#include "anm.h"
extern unsigned char ai_GfxDetail;  /* 0x6f35c4 (unk_5d33b0.c: OPTIONBYTES): @original anm_Update reads it as a byte object (0x4bb884 mov al, byte ptr [0x6f35c4]); through the 4-byte union, .byte[0] or a char view of it, VC6 loads the dword (mov eax, dword ptr) */
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (as ai.h: unsigned short (int)): this unit's callers were compiled against the 16-bit parameter (as ahm.c's) */

