/* d3_io_b.c -- binary BGF reader ("d3_io_b:" allocation tags): continues the AGI text reader of
   d3_io_a.c ("d3_io_a:"), probably the same TU (0x44e570..). A chunk parser (bgi_Parse) walks tables of
   {id, handler, count}; the handlers fill a BGIRD (the same context layout as the text reader's AGICTX).
   Names are ours.
   unit of rea 8cef7d5b (src/game/d3_io_b.c); boundaries in decl/text.dcl, notes in src/d3_io_b/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_io_t.h"
#include "d3_io_a.h"
#include "d3_io_b.h"

#define BGI_M(c) ((c)->mats[(c)->cur])
#define BGI_P(c) ((c)->polys[(c)->polyidx])

