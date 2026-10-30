/* evt.c -- original file name unknown ("evt_LoadScript", "evt_RunScript", "evt_EnterFunction").
   Interpreter for the C-like event scripts in x:\engine\gfx\scripts\: tokenizer, declarations,
   function calls, while/if, expression evaluation and calls into engine functions (EVTBUILTIN).
   Extent in GildeGold.exe (inferred): starts at 0x4273d0 after the sample library and continues past
   0x42abc0. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/evt.c); boundaries in decl/text.dcl, notes in src/evt/open.md */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
/* ch.h's block (see ch.c): d3.h declares these two with other types, ch.h has ours */
#define d3_Unk44e4a0 d3_Unk44e4a0_void
#include "d3.h"
#include "d3_anim.h"
#undef d3_Unk44e4a0
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#include "evt.h"
#include "evtc.h"

