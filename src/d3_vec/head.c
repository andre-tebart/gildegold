/* d3_vec.c -- light budget counters, angle / vector / matrix helpers: 0x453920..0x454890 (worker vec;
   0x454040.. by worker vec2, was unk_454040.c; round 12). Original file name unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_vec.c); boundaries in decl/text.dcl, notes in src/d3_vec/open.md */
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_vec.h"
#include "f3init.h"

extern unsigned int d3_LightKeysCulled;     /* 0x633b08 */
extern unsigned int d3_LightKeysVisible;    /* 0x633b0c */
extern unsigned int d3_LightGeneration;     /* 0x633b10 */
extern unsigned int d3_LightKeyBudgetCulled;  /* 0x5fcc6c */

