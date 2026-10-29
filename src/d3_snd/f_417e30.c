/* 0x417e30, plays snd once at obj's position without attaching it */

int d3snd_PlayAt(SNDHANDLE *snd, D3OBJECT *obj, int volume, float range)
{
    D3SOUND s;

    if (snd && obj) {
        s.range = range * D3SND_INCH;
        s.obj = obj;
        s.loops = 1;
        s.snd = snd;
        snd->loops = 1;
        snd->flags = 0;
        s.volume = volume;
        d3snd_UpdatePan(&s);
        d3snd_UpdateVolume(&s);
        if (snd->volume) {
            sb_Play(snd, 0, 0);
            return 1;
        }
    }
    return 0;
}
