/* chc.c -- original file name unknown: character script commands ("CreateCharacter(): Could not load
   character"). Extent in GildeGold.exe (inferred): starts at 0x40ba10 right after the script builtins
   (evtc.c) and ends with the command registration at 0x40d270 (not here yet); the object commands
   follow at 0x40d680. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/chc.c); boundaries in decl/text.dcl, notes in src/chc/open.md */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these two with other types (void / int flag, D3ANIMFLAGS); ch.h has ours */
#define d3_Unk44e4a0 d3_Unk44e4a0_void
#define d3anim_StartAnim d3anim_StartAnim_d3h
#include "d3.h"
#include "d3_anim.h"
#undef d3_Unk44e4a0
#undef d3anim_StartAnim
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#include "chc.h"

