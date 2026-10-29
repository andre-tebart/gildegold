/* 0x4177b0, stops the slot's sound, marks it stopped and unlinks it from the slot and its object */

void d3snd_Detach(D3SOUND *s)
{
    if (s->snd) {
        sb_Stop(s->snd, 0);
        s->snd->flags |= 8;
        sb_SetRestart((char *)s->snd, 0);
        s->snd = NULL;
    }
    if (s->obj) {
        s->obj->sound = NULL;
        s->obj = NULL;
    }
}
