/* 0x4242c0, returns the first playing non-ambient (music) track, NULL when none; callers hold
   msx_Lock */

MSXTRACK *msx_GetPlaying(void)
{
    int i;

    for (i = 0; i < MSX_TRACKS; i++)
        if (!msx_Tracks[i].bAmbient && msx_Tracks[i].playing)
            return &msx_Tracks[i];
    return NULL;
}
