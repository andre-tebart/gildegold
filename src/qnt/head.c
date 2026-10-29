/* qnt.c -- original file name unknown.
   Octree color quantizer: counts the image's colors in a 6-level octree (5 bits per channel), merges
   the leaves with the fewest pixels into their parents until ncolors are left (min-heap), builds the
   palette and maps the image, optionally with error diffusion.
   Extent in GildeGold.exe (inferred, not proven): 0x4206d0..0x4214a0.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/qnt.c); boundaries in decl/text.dcl, notes in src/qnt/open.md */
#include <stdlib.h>
#include <string.h>
#include "qnt.h"

#define QTOTAL(h) qnt_Level[(h).level][(h).node].total

