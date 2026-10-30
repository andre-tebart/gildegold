/* 0x417710, attaches an opened sound to obj */

D3SOUND *d3snd_Alloc(void);                                             /* 0x417800 */

D3SOUND *d3snd_Attach(SNDHANDLE *snd, D3OBJECT *obj, int volume, float range)
{
    D3SOUND *s = NULL;

    if (snd && obj && !obj->sound) {
        s = d3snd_Alloc();
        if (s && snd->entry) {
            sb_SetRestartC((char *)snd, 1);
            strncpy(s->name, snd->entry->name, 0x32);
            s->obj = obj;
            obj->sound = s;
            s->snd = snd;
            s->volume = volume;
            s->range = range * D3SND_INCH;
            s->hold = 1;
            d3snd_UpdateVolume(s);
            d3snd_UpdatePan(s);
            s->hold = 0;
            s->idle = 0;
        }
    }
    return s;
}
