/* 0x4a7430, deals a new round: all cards back, the three seats cleared */

int ai_Unk4a7430(G10GAME *g)
{
    G10SEAT *s;
    int i;

    if (g == 0)
        return 0;
    g->left = 0x18;
    g->unkb5 = 0;
    g->unka0 = 0;
    g->phase = 0;
    g->unkb0 = util_RandomU(3);
    g->unkbc = 0;
    for (i = 0; i < 24; i++) {
        ai_Unkf83660[i].owner = 3;
        ai_Unkf83660[i].shown = 0;
    }
    s = g->seat;
    i = 3;
    do {
        s->card[0] = -1;
        s->card[1] = -1;
        s->card[2] = -1;
        s->card[3] = -1;
        s->card[4] = -1;
        s->unk1c = 0;
        s->unk2c = 0;
        s->unk24 = -1;
        s->unk20 = util_RandomU(5) == 1;
        s->unk2c = 0;
        s->unk28 = -1;
        s++;
    } while (--i);
    return 1;
}
