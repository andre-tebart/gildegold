/* d3_light.c -- dynamic lighting of mesh objects ("d3_light:cache", "d3_light:rlight_object"), original file
   name unknown: 0x451e80..0x4538a0. 0x451e80..0x452938 (light callbacks, d3_LightApply) by worker mesh,
   0x452940.. (was unk_452940.c) by worker d3x (round 11); one object view (D3OBJECT + D3_L, d3_light.h)
   since round 12. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_light.c); boundaries in decl/text.dcl, notes in src/d3_light/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_light.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern unsigned char txt_Keys;

