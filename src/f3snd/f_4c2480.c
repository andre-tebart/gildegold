/* 0x4c2480, ends the special track (snd_PlaySpecialTrack): the ambient sounds back on, and the music back to
   full volume (unless the special track still plays) */

void snd_EndSpecialTrack(void)
{
    snd_AmbientSaved = 0;
    snd_AmbientOn.on = (char)snd_AmbientOnSaved;
    if (snd_MusicEnabled != 0 && snd_TrackHandles.current != snd_SpecialTrack)
        snd_FadeMusic(1.0f, 3000);
}
