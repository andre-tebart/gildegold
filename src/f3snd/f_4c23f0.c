/* 0x4c23f0, starts the special track of an event (fight, cutscene, ...): the first call saves the ambient flag
   and turns the ambient sounds off, then plays name (with name 0 only the music gets quieter, 0.4 over 2000 ms);
   snd_EndSpecialTrack ends it */

void snd_PlaySpecialTrack(char *name)
{
    unsigned int on;

    if (snd_AmbientSaved == 0) {
        on = snd_AmbientOn.word;
        snd_AmbientOn.on = 0;
        snd_AmbientSaved = 1;
        snd_AmbientOnSaved = (unsigned char)on;
    }
    if (snd_MusicEnabled == 0 || snd_MusicVolume == 0.0f)
        return;
    if (name != 0) {
        if (snd_TrackHandles.current != 0)
            msx_Stop(snd_TrackHandles.current, 1);
        snd_SpecialTrack = msx_Play(name, 0);
        snd_TrackHandles.current = snd_SpecialTrack;
        return;
    }
    snd_FadeMusic(0.4f, 2000);
}
