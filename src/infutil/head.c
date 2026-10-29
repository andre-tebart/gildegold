/* inflate_util.c -- data and routines common to blocks and codes
 * Copyright (C) 1995-1998 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
   unit of rea 8cef7d5b (src/zlib/infutil.c); boundaries in decl/text.dcl, notes in src/infutil/open.md */

#include "zutil.h"
#include "infblock.h"
#include "inftrees.h"
#include "infcodes.h"
#include "infutil.h"

struct inflate_codes_state {int dummy;}; /* for buggy compilers */

/* And'ing with mask[n] masks the lower n bits */
/* 0x5fe4f0 */
uInt inflate_mask[17] = {
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
    0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};


