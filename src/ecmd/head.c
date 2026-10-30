/* ecmd.c -- original file name unknown: object script commands ("ecmd_KillObject: Could not find object
   to kill"). Extent in GildeGold.exe (inferred): starts at 0x40d680 right after the character commands
   (chc.c, whose registration ends at 0x40d678) and ends with the command registration at 0x40f960.
   Functions are kept in their original order. Names are ours (ocmd_ + the script name).
   unit of rea 8cef7d5b (src/game/ecmd.c); boundaries in decl/text.dcl, notes in src/ecmd/open.md */
struct D3OBJECT;
struct D3SURF;
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these two with other types (void / int flag, D3ANIMFLAGS); ch.h has ours */
#define d3_Unk44e4a0 d3_Unk44e4a0_void
#include "d3.h"
#include "d3_anim.h"
#undef d3_Unk44e4a0
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#define d3anim_StartAnimCh ((int (*)(void *, char *, CHANIMFLAGS))d3anim_StartAnim)  /* 0x435550 (d3.h: D3ANIMFLAGS f): @original this unit passes its field-wise CHANIMFLAGS, 12 bytes like D3ANIMFLAGS, as the original TU did */
#include "ecmd.h"
#include "d3_light.h"
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests and clears */

#define OCMD_DEG 0.017453292f       /* degrees to radians */

struct D3IOANIMDATA *d3_LoadAnimStream(char *name, char *file, char unk);           /* 0x44e4a0, d3_io_t.c */

