/* 0x417cf0, plays the slot's sound with loops/delay (sb_Resume when the slot is held); resets idle */

void d3snd_Play(D3SOUND *s, int a, int loops, int delay)
{
    char buf[256];

    if (!s || !s->snd)
        return;
    if (!s->snd->entry) {
        sprintf(buf, "Entry-Handle missing for %s", s->name);
        sprintf(buf, "3dSound-Handle is attached to %s", s->obj->name);
        return;
    }
    s->snd->loops = loops;
    s->loops = loops;
    sb_SetLoops((struct SBSAMPLE *)s->snd,loops);
    s->delay = delay;
    if (!s->hold)
        sb_PlayH(s->snd, a, delay);
    else
        sb_Resume((struct SBSAMPLE *)s->snd);
    s->idle = 0;
}
