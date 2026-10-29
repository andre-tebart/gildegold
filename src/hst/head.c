/* hst.c -- the history texts (0x4adf40..0x4b39f0, after hmt.c): character predicates, the text tokens and text
   commands of a history text, its parser (the hst_* names are leaked by its error texts), the date labels and the
   yearly history dialogs. Its .data block starts at 0x601058. Names are ours except the hst_* ones.
   unit of rea 8cef7d5b (src/game/hst.c); boundaries in decl/text.dcl, notes in src/hst/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hst.h"
#include "txt.h"

