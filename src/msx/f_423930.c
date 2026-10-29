/* 0x423930 */

void msx_SetTrackVolume(MSXTRACK *t, int vol)
{
    if (msx_Driver && t) {
        EnterCriticalSection(&msx_Lock);
        t->volume = vol;
        if (t->fademode == 0 && t->playing) {
            t->curvol = vol;
            if (t->stream)
                sfx_SetStreamVolume(t->stream, vol);
        }
        LeaveCriticalSection(&msx_Lock);
    }
}
