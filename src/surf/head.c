/* surf.c -- original file name unknown.
   Scaling and pixel format conversion between locked DirectDraw surfaces (used for the snow mipmaps
   in d3_tools.c and by d3_d3d.c). Box filter when shrinking, bilinear interpolation (nearest for
   8-bit) when growing. Extent in GildeGold.exe (inferred, not proven): 0x41ebc0..0x4206d0, right after the
   last Dd_vesa.c function. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/surf.c); boundaries in decl/text.dcl, notes in src/surf/open.md */
#include <windows.h>
#include <ddraw.h>
#include "d3.h"
#include "surf.h"

