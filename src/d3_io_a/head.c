/* d3_io_a.c -- AGI / AGF text model reader ("d3_io_a:" allocation tags): line handlers that fill a reader
   context (materials, points, polygons). Extent in GildeGold.exe (inferred): starts at 0x44e570 (right after
   the object / animation loaders of unk_44df40.c) and continues past 0x44ee50, where the worker scope ended.
   Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_a.c); boundaries in decl/text.dcl, notes in src/d3_io_a/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d3_io_a.h"

#define AGI_TOK(sep) strtok(0, sep)
#define AGI_M(c) ((c)->mats[(c)->cur])

