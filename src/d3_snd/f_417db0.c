/* 0x417db0, stops the slot's sound with the given fade */

void d3snd_Stop(D3SOUND *s, int unk)
{
    if (s && s->snd)
        sb_Stop(s->snd, unk);
}
