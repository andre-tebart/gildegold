/* 0x4241a0, starts a master music fade to `vol` (0..1) over `ms` milliseconds; ms == 0 sets the
   factor at once. Does nothing without a driver or with vol outside 0..1; no return value, takes
   msx_Lock and leaves the fade to the update thread */

void msx_FadeMaster(float vol, int ms)
{
    if (msx_Driver && !(vol < 0.0f) && !(vol > 1.0f)) {
        EnterCriticalSection(&msx_Lock);
        if (ms == 0) {
            msx_Fade = vol;
            msx_SetVolume(msx_Volume);
            LeaveCriticalSection(&msx_Lock);
            return;
        }
        msx_FadeDuration = ms;
        msx_FadeTo = vol;
        msx_FadeFrom = msx_Fade;
        msx_FadeStart = MSX_TIME;
        LeaveCriticalSection(&msx_Lock);
    }
}
