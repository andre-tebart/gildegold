/* 0x423a50, ambient master volume */

void msx_SetAmbVolume(int vol)
{
    if (msx_AmbDriver) {
        EnterCriticalSection(&msx_Lock);
        sfx_SetMasterVolume(msx_AmbDriver, vol);
        msx_AmbVolume = vol;
        LeaveCriticalSection(&msx_Lock);
    }
}
