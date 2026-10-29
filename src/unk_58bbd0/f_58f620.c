/* 0x58f620, the frame pump of the build screen (23 B): runs sv_RunFrame (0x5309a0, this callback, mask 0x67fff)
   until the screen ends; the caller's context is not used */

int sv_RunFrame(int cb, unsigned mask);

void ai_PumpFrames(int unused)
{
    int r;

    do {
        r = sv_RunFrame(ai_PumpFrames, 0x67fff);
    } while (r != 0);
}
