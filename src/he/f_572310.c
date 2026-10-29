/* 0x572310, the meeting search of the event 0x90 (a = the building, b = the radius kind 2 / 1 / other, c = the kind of the meeting, d = the low 2 bits of the character's +4): the building pointer +0x61 must exist; each of the 0x100 record slots of 0x889f74 (0xa9 bytes: the kind of 0x7e3f30 and the player word +0x27) whose kind bit (0x807c4116 / bit 0) is allowed, with a building and a character +0x61 within the radius (1500 / 800 / 400) of the building: each character record 0x770b80 (0x21c, id word != -1, kind +2 < 10, +4 & 3 == d) that this record owns (+0x16c = 3, +0x170 = 2, own building = 1) gets the lottery 0x5720f0 (chance (1 - dist / radius) * 0.5 + 0.25, c): mode 3 -> 0x5417d0 (0, ch, building, the record, v, f), mode 1 of a kind 6 / 7 -> the text 0x163c and the message 0x53dda0 (+0x5f7, 0x631090) */


void he_Unk572310(int a, int b, int c, unsigned int d)
{
    char text[0x800];
    char *per, *rec, *other, *bldrec, *ch;
    float x, y, z, dist, lim, f;
    unsigned int lo, hi;
    int i, n, v, mode;
    unsigned short bld;

    per = *(char **)(a + 0x61);
    if (per == 0)
        return;
    if (b == 2)
        lim = 1500.0f;
    else if (b == 1)
        lim = 800.0f;
    else
        lim = 400.0f;
    x = *(float *)(per + 0x4c);
    y = *(float *)(per + 0x50);
    z = *(float *)(per + 0x54);
    for (n = 0; n < 0xa900; n += 0xa9) {
        rec = (char *)(*(int *)(unsigned)&ai_Blds + n);
        lo = 0;
        hi = 0;
        if (*(unsigned char *)((unsigned)&ai_BTypes + rec[0] * 0x24d) < 0x20)
            lo = 1 << *(unsigned char *)((unsigned)&ai_BTypes + rec[0] * 0x24d);
        else
            hi = 1 << (*(unsigned char *)((unsigned)&ai_BTypes + rec[0] * 0x24d) - 0x20);
        bld = *(unsigned short *)(rec + 0x27);
        if (bld == 0xffff || bld == *(unsigned short *)(unsigned)&ai_PlayerChar)
            continue;
        other = *(char **)(rec + 0x61);
        if (other == 0)
            continue;
        if ((lo & 0x807c4116) == 0 && (hi & 1) == 0)
            continue;
        dist = sqrt((x - *(float *)(other + 0x4c)) * (x - *(float *)(other + 0x4c)) +
                    (y - *(float *)(other + 0x50)) * (y - *(float *)(other + 0x50)) +
                    (z - *(float *)(other + 0x54)) * (z - *(float *)(other + 0x54)));
        if (dist > lim)
            continue;
        bldrec = (char *)((unsigned)&ai_Chars + bld * 0x21c);
        for (i = 0; i < 0x80; i++) {
            ch = (char *)((unsigned)&ai_Chars + i * 0x21c);
            if (*(short *)ch == -1 || HM_B(ch, 2) >= 0xa || (HM_I(ch, 4) & 3) != (int)d)
                continue;
            if (HM_I(ch, 0x16c) != 0 && HM_I(ch, 0x16c) == (int)rec)
                mode = 3;
            else if (HM_I(ch, 0x170) != 0 && HM_I(ch, 0x170) == (int)rec)
                mode = 2;
            else if (bldrec != ch)
                continue;
            else
                mode = 1;
            v = he_Unk5720f0((unsigned short *)ch, (1.0f - dist / lim) * 0.5f + 0.25f, c, &f);
            if (v != 0) {
                if (mode == 3)
                    he_Unk5417d0(0, ch, bldrec, rec, v, f);
                else if (mode == 1 && (HM_B(ch, 2) == 6 || HM_B(ch, 2) == 7)) {
                    txt_Format(text, 0x163c, v + 0x1632);
                    he_ShowNews(HM_I(ch, 4), -1, text, 0, 0x5f7, (char *)(unsigned)&ch_EmptyName);
                }
            }
        }
    }
}
