/* 0x423e70, sets the per-track fade duration in ms (msx_FadeTime) used by the update thread's
   fade-in / fade-out volume ramp; no return value, writes one module variable */

void msx_SetFadeTime(int ms)
{
    msx_FadeTime = ms;
}
