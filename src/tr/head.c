/* tr.c -- original file name unknown ("tr_startup" string). Extent in GildeGold.exe (inferred):
   0x42ea90 (after the time base, tb.c) up to 0x42ec95; the virtual file system (vfs.c)
   starts at 0x42eca0. Its .data: tr_Key1/tr_Key2 at 0x5f8fd0 with their strings, then "tr_startup".
   The random helpers may be a TU of their own (no data to tell). Functions are kept in their original
   order. Names are ours except tr_startup.
   unit of rea 8cef7d5b (src/game/tr.c); boundaries in decl/text.dcl, notes in src/tr/open.md */
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
#include "tr.h"

char *tr_Key1 = "*bRaIn DeaD%&";  /* 0x5f8fd0 */
char *tr_Key2 = "%aRscH Ab :-)";  /* 0x5f8fd4 */
TRREC * tr_First = 0;  /* 0x6333f0 */
TRREC * tr_End = 0;  /* 0x6333f4 */
TRREC * tr_Table = 0;  /* 0x6333f8 */

