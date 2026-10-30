/* zutil.c -- target dependent utility functions for the compression library
 * Copyright (C) 1995-1998 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
   unit of rea 8cef7d5b (src/zlib/zutil.c); boundaries in decl/text.dcl, notes in notes/units/zutil.md */

/* @(#) $Id$ */

#include "zutil.h"

struct internal_state      {int dummy;}; /* for buggy compilers */

#ifndef STDC
extern void exit OF((int));
#endif

extern char ch_EmptyName[];      /* 0x631090 (ch.h), the shared empty string: the original pools "" into it */

/* 0x5fe570 */
const char *z_errmsg[10] = {
"need dictionary",     /* Z_NEED_DICT       2  */
"stream end",          /* Z_STREAM_END      1  */
ch_EmptyName,          /* Z_OK              0  */
"file error",          /* Z_ERRNO         (-1) */
"stream error",        /* Z_STREAM_ERROR  (-2) */
"data error",          /* Z_DATA_ERROR    (-3) */
"insufficient memory", /* Z_MEM_ERROR     (-4) */
"buffer error",        /* Z_BUF_ERROR     (-5) */
"incompatible version",/* Z_VERSION_ERROR (-6) */
ch_EmptyName};


/* unlinked */
const char * ZEXPORT zlibVersion()
{
    return ZLIB_VERSION;
}


/* exported to allow conversion of error code to string for compress() and
 * uncompress()
 */
/* unlinked */
const char * ZEXPORT zError(err)
    int err;
{
    return ERR_MSG(err);
}








#ifndef STDC
extern voidp  calloc OF((uInt items, uInt size));
extern void   free   OF((voidpf ptr));
#endif

/* 0x468770, zlib's default allocation hook (zalloc): items*size bytes, uninitialised - this build uses
 * malloc where upstream uses calloc */
voidpf zcalloc (opaque, items, size)
    voidpf opaque;
    unsigned items;
    unsigned size;
{
    if (opaque) items += size - size; /* make compiler happy */
    /* GildeGold: malloc instead of upstream calloc (0x468770) */
    return (voidpf)malloc(items * size);
}
/* 0x468790, zlib's default free hook (zfree): frees what zcalloc returned */

void  zcfree (opaque, ptr)
    voidpf opaque;
    voidpf ptr;
{
    free(ptr);
    if (opaque) return; /* make compiler happy */
}
