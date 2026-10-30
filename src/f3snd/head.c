/* f3snd.c -- tag "f3snd: snd_AddToQueue" (mem_Alloc, 0x4c0230); the sound queue and ambient sounds: the playing list snd_Queue, the gusts ("Boehe_Leicht_s", "Boehe_Mittel_s",
   "Boehe_Stark_s", .data 0x602b10 right after rt.c's "rt_load"). Start: the first function after rt.c's
   save / load; no module tag. Names are ours.
   unit of rea 8cef7d5b (src/game/f3snd.c); boundaries in decl/text.dcl, notes in src/f3snd/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "f3snd.h"

/* d3_Time * 13, spelled so that the volatile is read three times */
#define F3SND_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

/* stdlib min / max (the body ai.h defines; f3snd cannot include ai.h, its AITIME ai_Time view differs) */
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#include "data_views.h"
#define d3snd_DetachI ((void (*)(int))d3snd_Detach)  /* 0x4177b0 (d3_snd.c: D3SOUND *): this unit's int handle */
#define d3snd_StopI ((void (*)(int, int))d3snd_Stop)  /* 0x417db0 (d3_snd.c: D3SOUND *): this unit's int handle */
#define d3snd_SetHoldI ((void (*)(int, int))d3snd_SetHold)  /* 0x417dd0 (d3_snd.c: D3SOUND *): this unit's int handle */
#define d3snd_AttachI ((int (*)(int, char *, int, float))d3snd_Attach)  /* 0x417710 (d3_snd.c: D3SOUND *(SNDHANDLE *, D3OBJECT *, int, float)): this unit's int handle / char * object view */
#define d3snd_SetVolumeI ((void (*)(int, int))d3snd_SetVolume)  /* 0x417e00 (d3_snd.c: D3SOUND *): this unit keeps the slot as an int handle */
#define ai_MarketBuildingV (*(G16BLDVIEW **)&ai_MarketBuilding)  /* 0x6f1bc8, ai_MarketBuilding (ai.c: AIBLD *) as this unit's G16BLDVIEW (its +0x4c / object fields) */
#define ai_FallbackBuildingB (*(unsigned char **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_PlayerBuildingB (*(unsigned char **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_CharsC (*(char (*)[768 * 0x21c])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) read flat, as this unit does */

