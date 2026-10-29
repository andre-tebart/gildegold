/* 0x544550, draws 3 of 12 shuffled slots for a duel between the figures p2 and p3: slot bytes at +0xbc .. +0xbe and three words (slot + 1) * (the worth of p2 + the worth of p3, each at least 32000) / 640 at +0xc0 (ROUGH) */

void he_Unk544550(int ev, unsigned short *p2, unsigned short *p3)
{
    int tab[12];
    int base, i, w2, w3;
    int *pw;

    base = gm_Unk505e60(*p2, 0);
    w2 = gm_GetWorth(*p2) > 32000 ? gm_GetWorth(*p2) : 32000;
    w3 = gm_GetWorth(*p3) > 32000 ? gm_GetWorth(*p3) : 32000;
    for (i = 0; i < 0xc; i++)
        tab[i] = i + base / 2;
    gm_Shuffle(0xc, tab);
    pw = (int *)(ev + 0xc0);
    for (i = 0; i < 3; i++) {
        *(char *)(ev + 0xbc + i) = (char)tab[i];
        *pw = (int)((float)((unsigned char)tab[i] + 1) * (float)(w2 + w3) * 0.0015625f);
        pw++;
    }
}
