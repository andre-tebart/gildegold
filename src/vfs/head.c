/* vfs.c -- original file name unknown: the virtual file system ("vfs_Tfopen", "vfs_init" strings).
   The translation unit starts at 0x42eca0 (after tr.c, "tr_startup"; its .data starts with
   gz_magic at 0x5f9004, right after "tr_startup", and "vfs:IGetSubDir" of 0x42ed70 follows) and ends at
   0x4315c0, where d3_anim.c starts. Functions are kept in their original order. Names are ours except
   vfs_IGetSubDir.
   unit of rea 8cef7d5b (src/game/vfs.c); boundaries in decl/text.dcl, notes in src/vfs/open.md */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include "vfs.h"

static int gz_magic[2] = {0x1f, 0x8b};   /* 0x5f9004, the first data of this file (as in gzio.c) */
struct MPOOL * vfs_EntryPool = 0;  /* 0x633400 */
struct MPOOL * vfs_FilePool = 0;  /* 0x633404 */
char vfs_IgnoreCase = 0;  /* 0x633408 */
int vfs_OpenCount = 0;  /* 0x63340c */
const char vfs_Ext[][6] = { ".BIN5", ".BIN4", ".BIN3", ".BIN2", ".BIN1", ".BIN0", ".BIN" };  /* 0x5eba40, ".BIN5", ".BIN4", ... */

