/* 0x5de470, mss32.lib's exit callback that MSS_auto_cleanup registers: shuts the sound library down (the
   original jumps to the AIL_shutdown import thunk at 0x5de480; sfx.h declares AIL_shutdown dllimport, so here
   the jump goes through the import slot directly: the same behaviour, other bytes) */

static void mss_AutoCleanupExit(void)
{
    AIL_shutdown();
}
