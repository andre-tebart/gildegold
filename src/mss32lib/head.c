/* mss32lib: the code member of the Miles Sound System's import library mss32.lib that the original links in
   (0x5de460..0x5de480, after the DINPUT stubs): MSS_auto_cleanup and its exit callback. No installation has
   mss32.lib; `dcp link` builds the import members from the exe's import directory and adds this unit's object
   to that archive (decl/text.dcl `libunit`), so LINK places it among the library contributions.
   unit of no rea source (rea kept them in sfx.c); notes in src/mss32lib/open.md */

#include <stdlib.h>

/* the import, as mss32.lib's own code calls it: through the import library's thunk (no dllimport) */
void __stdcall AIL_shutdown(void);
