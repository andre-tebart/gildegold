/* 0x4a7140, starts a game of character p in g: the stake from p's money, the deck, two random opponents */

int ai_PokerStart(AIPER *p, G10GAME *g)
{
    double d;
    int i, j, k;
    G10CARD *c;
    G10NPC *n;

    ai_Unkf83654 = 0;
    if (p == 0 || *((unsigned char *)p + 2) != 6 || g == 0)
        return 0;
    memset(ai_Unkf837e0, 0, sizeof(ai_Unkf837e0));
    ai_Unkf83654 = g;
    memset(g, 0, sizeof(G10GAME));
    ai_Unkf83654->seat[0].who = p;
    k = gm_ConvertMoney(gm_Unk509e30(*(int *)((unsigned char *)p + 0x178), ai_OwnPlayer), ai_OwnPlayer);
    d = k;
    if (!(10000.0 <= d))
        d = 10000.0;
    ai_Unkf83654->money = gm_PlayerFactor((int)(d / (log10(d) + 1.0) * 0.1), ai_OwnPlayer);
    ai_Unkf83654->stake = ai_Unkf83654->money / (util_Random(3) + 7);
    ai_Unkf83654->stake2 = ai_Unkf83654->stake;
    ai_Unkf83654->unkb8 = 4;
    k = 0;
    c = ai_Unkf83660;
    do {
        for (j = 0; j < 4; j++) {
            c->rank = j;
            c->suit = k;
            c++;
        }
        k++;
    } while (c < &ai_Unkf83660[24]);
    for (i = 0; i < 2; i++) {
        n = &ai_Unkf837e0[i];
        if (i == 0)
            n->female = (unsigned char)util_Random(2);
        else
            n->female = ai_Unkf837e0[0].female == 0;
        n->unk0d = (unsigned char)util_Random(4) + 1;
        strncpy(n->nname, ai_Unk143b178[util_Random(0x95)], 15);
        if (n->female == 0) {
            strncpy(n->vname, ai_Unk143ac5c[util_Random(0xbf)], 15);
            switch (util_Random(4)) {
            case 0:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buerger_MANN") + 0x639;
                break;
            case 1:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("Handwerker_MANN") + 0x639;
                break;
            case 2:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buerger3_DICKER") + 0x639;
                break;
            case 3:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("schneider1_MANN") + 0x639;
                break;
            }
        } else {
            strncpy(n->vname, ai_Unk143af70[util_Random(0x70)], 15);
            switch (util_Random(4)) {
            case 0:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buergerin_frau") + 0x639;
                break;
            case 1:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("buergerin2_frau") + 0x639;
                break;
            case 2:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("minerin_frau") + 0x639;
                break;
            case 3:
                ai_Unkf83654->seat[i + 1].model = unk_Unk4fb770("arbeiterin_frau") + 0x639;
                break;
            }
        }
        n->kind = 10;
        ai_Unkf83654->seat[i + 1].who = n;
    }
    return ai_Unk4a7430(g);
}
