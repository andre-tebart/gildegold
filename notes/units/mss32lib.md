# mss32lib

The code member of mss32.lib (Miles Sound System) the original links in, at 0x5de460..0x5de480:

- `MSS_auto_cleanup` (0x5de460) registers `mss_AutoCleanupExit` with `atexit` and returns 0; `sfx` calls it.
- `mss_AutoCleanupExit` (0x5de470) jumps to the `AIL_shutdown` import thunk (0x5de480, the import member).

The real names of the member's object and of the callback are unknown; `mss_AutoCleanupExit` is ours.
