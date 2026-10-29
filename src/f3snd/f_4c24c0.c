/* 0x4c24c0, fades the music to v in ms */

void snd_FadeMusic(float v, int ms)
{
    if (snd_TrackHandles.current != 0)
        msx_FadeMaster(v, ms);
}
