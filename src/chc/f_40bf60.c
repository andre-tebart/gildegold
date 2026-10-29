/* 0x40bf60, action callback: plays the sound named in the action */

void chc_AniSoundCB(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    void *snd = sb_GetSample(0, CH_ACTOBJ(a)->name2);

    if (snd) {
        if (d3_Terrain)
            d3snd_PlayAt(snd, ch->obj, 0x7f, 3000.0f);
        else
            d3snd_PlayAt(snd, ch->obj, 0x7f, 1000.0f);
    }
}
