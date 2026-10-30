/* unk_4591a0.c -- 0x458e70..0x459d80, original file name unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4591a0.c); boundaries in decl/text.dcl, notes in src/unk_4591a0/open.md */
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_anim.h"
#include "d3_light.h"
#include "unk_4591a0.h"

/* d3_EnvMapCoords (was unk_458810.c: only 0x459300 calls it) works on the pose views of unk_458810.h; its
   prototypes there are for the other file. */
#include "unk_458810.h"

#define XENTRY(slot) ((slot)[0] * 0xc0 + *(int *)((slot)[0x1a] + 0x15c))

