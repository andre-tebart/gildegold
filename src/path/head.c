/* path.c -- original file name unknown.
   A* path search on a square grid: 8 neighbours, terrain cost per node type and unit type, Manhattan
   distance heuristic, open list as a sorted linked list in the nodes. path_Search2 grows the search
   from both ends and stops where they meet. Extent in GildeGold.exe (inferred, not proven):
   0x424f30..0x425bc0. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/path.c); boundaries in decl/text.dcl, notes in src/path/open.md */
#include <stdlib.h>
#include "path.h"

