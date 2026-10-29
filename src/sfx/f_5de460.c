/* 0x5de460, mss32.lib's MSS_auto_cleanup: registers the library's exit callback (mss_AutoCleanupExit, 0x5de470)
   and returns 0. A member of the Miles import library (code, not an import), which no installation has: its two
   functions are written here so the program behaves the same; the original places them in .text after the DINPUT
   stubs, which no unit can own yet (the link puts them in this unit) */

#include <stdlib.h>

static void mss_AutoCleanupExit(void);

int MSS_auto_cleanup(void)
{
    atexit(mss_AutoCleanupExit);
    return 0;
}
