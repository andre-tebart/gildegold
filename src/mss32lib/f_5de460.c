/* 0x5de460, mss32.lib's MSS_auto_cleanup: registers the library's exit callback (mss_AutoCleanupExit, 0x5de470)
   and returns 0 */

static void mss_AutoCleanupExit(void);

int MSS_auto_cleanup(void)
{
    atexit(mss_AutoCleanupExit);
    return 0;
}
