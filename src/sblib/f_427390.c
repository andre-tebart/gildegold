/* 0x427390 */

void sb_SetLoops(SBSAMPLE *s, int loops)
{
    if (sb_Driver && s) {
        s->loops = loops;
        sfx_SetSampleLoopCount(s->handle, loops);
    }
}
