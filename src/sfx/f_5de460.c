/* 0x5de460, mss32.lib's MSS_auto_cleanup: registers the library's exit callback (0x5de470) and returns 0;
   the original keeps the library member in .text before LIBC, so its bytes are source here */

#include <stdlib.h>

int MSS_auto_cleanup(void)
{
    atexit((void (*)(void))0x5de470);
    return 0;
}
