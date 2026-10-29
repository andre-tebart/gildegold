/* 0x5de470, mss32.lib's exit callback that MSS_auto_cleanup registers: shuts the sound library down (a jump to
   the AIL_shutdown import thunk at 0x5de480) */

static void mss_AutoCleanupExit(void)
{
    AIL_shutdown();
}
