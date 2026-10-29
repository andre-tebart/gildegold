/* d3_anim.c -- original file name unknown; the allocation tags are "d3_anim:CalculateAnimNormals...".
   Vertex animations: a cache of loaded animations (LRU list, purged above d3anim_CacheMax bytes) and
   up to three animation slots per animated object. Only the part from 0x434a80 is here; the file
   starts at or before 0x4324e0: d3anim_SetFrame only matches with d3anim_NextFrame defined earlier in the
   same file (VC6 then knows that the call leaves edx alone). Extent in GildeGold.exe: ..0x4359f0, where
   d3_d3d.c starts.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_anim.c); boundaries in decl/text.dcl, notes in src/d3_anim/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_anim.h"

extern D3ANIMDATA d3anim_Head;              /* 0xfc2580, list head; d3anim_Head.next at 0xfc26e0 */
extern D3ANIMDATA d3anim_Tail;              /* 0xfe7440, list end */
extern unsigned int d3anim_Clock;           /* 0x63343c */
unsigned int d3anim_CacheMax = 4194304;  /* 0x5f927c, bytes */
extern int d3anim_Busy;                     /* 0x633b04 */

