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
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern unsigned char ai_Unka69140;


