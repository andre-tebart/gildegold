/* 0x4a79d0, seat s puts card k (0 or 1) away and draws a new one in its place */

void ai_Unk4a79d0(G10SEAT *s, int k)
{
    int o, n;

    if (k < 2) {
        o = ai_Unkf83660[s->card[k]].owner;
        ai_Unkf83660[s->card[k]].owner = 4;
        s->unk24 = s->card[k];
        n = ai_Unk4a7760(o);
        s->card[k] = n;
        ai_Unkf83660[n].shown = 0;
    }
}
