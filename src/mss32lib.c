/* mss32lib: the code member of the Miles Sound System's import library mss32.lib that the original links in
   (0x5de460..0x5de480, after the DINPUT stubs): MSS_auto_cleanup and its exit callback. No installation has
   mss32.lib; `dcp link` builds the import members from the exe's import directory and adds this unit's object
   to that archive (decl/text.dcl `libunit`), so LINK places it among the library contributions.
   The real names of the member's object and of the exit callback are unknown (mss_AutoCleanupExit is ours).
   unit of no rea source (rea kept them in sfx.c) */

#include <stdlib.h>

/* the import, as mss32.lib's own code calls it: through the import library's thunk (no dllimport) */
void __stdcall AIL_shutdown(void);

static void mss_AutoCleanupExit(void);

/* 0x5de460, mss32.lib's MSS_auto_cleanup: registers the library's exit callback (mss_AutoCleanupExit, 0x5de470)
   and returns 0 */
int MSS_auto_cleanup(void)
{
    atexit(mss_AutoCleanupExit);
    return 0;
}
/* 0x5de470, mss32.lib's exit callback that MSS_auto_cleanup registers: shuts the sound library down (a jump to
   the AIL_shutdown import thunk at 0x5de480) */

static void mss_AutoCleanupExit(void)
{
    AIL_shutdown();
}
