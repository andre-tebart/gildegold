/* d3_quad.c -- quad tree over the objects of a scene, 0x457c30..0x458810 (the zones d3q_Create .. d3q_Rebuild from 0x4582d0 on).
   Original file name unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_quad.c); boundaries in decl/text.dcl, notes in src/d3_quad/open.md */
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_quad.h"
#define d3_ClipBoxU ((unsigned int (*)(void *, float *, float *))d3_ClipBox)  /* 0x439650 (d3_engine.h: unsigned char): @original the byte-matched d3q_Mark reads the whole unsigned int result (no 25 ff 00 00 00 after the call) */

