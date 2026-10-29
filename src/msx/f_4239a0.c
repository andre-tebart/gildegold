/* 0x4239a0, music master volume scaled by the current fade (clamped to 0..1); stores the raw volume */

void msx_SetVolume(int vol)
{
    if (msx_Driver) {
        if (msx_Fade < 0.0f)
            msx_Fade = 0.0f;
        else if (msx_Fade > 1.0f)
            msx_Fade = 1.0f;
        EnterCriticalSection(&msx_Lock);
        sfx_SetMasterVolume(msx_Driver, (int)(vol * msx_Fade));
        LeaveCriticalSection(&msx_Lock);
        msx_Volume = vol;
        sprintf(msx_VolMsg, "Fademodifier is %f, Mastervolume is %d", msx_Fade, vol);
    }
}
