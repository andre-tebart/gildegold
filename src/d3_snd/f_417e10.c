/* 0x417e10, 1 while the slot's sound plays */

int d3snd_IsPlaying(D3SOUND *s)
{
    if (s && s->snd)
        return sb_IsPlaying(s->snd);
    return 0;
}
