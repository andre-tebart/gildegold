/* 0x5652b0, a Ghidra miss: the allocation handler of the event 0x50 (ROUGH): the characters of kind 6 (0x501190 (1, 6) / 0x501030) without 0x502520 / 0x5024f0 and with a +0x61 object are collected (at most 16 are kept as targets); +0xb0 .. +0xeb = -1; the chosen
   character +0xac (random pick when -1) gets the nearby ones within +0xf0 (0x453e80 on their +0x61 positions + 0x4c) as followers at +0xac + n * 4; +0x100 / +0x104 = 0; the run clock is now */

void he_Unk5652b0(int ev)
{
    int list[0x100];
    int n, i, o, c, k;

    n = 0;
    list[0] = 1;
    for (o = gm_BuildQuery(1, 6); o != 0; o = gm_BuildNext()) {
        if (gm_IsHomeType(o) == 0 && gm_IsType10(o) == 0 && *(int *)(o + 0x61) != 0)
            list[n++ & 0xffff] = o;
    }
    for (i = 0; i < 0xf; i++)
        *(int *)(ev + 0xb0 + i * 4) = -1;
    if (*(int *)(ev + 0xac) == -1) {
        c = list[(unsigned short)util_Random(n)];
        *(int *)(ev + 0xac) = *(int *)(c + 1);
    } else {
        c = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    }
    k = 0;
    n &= 0xffff;
    for (i = 0; i < n && (unsigned short)k < 0x10; i++) {
        o = list[i];
        if (o != c && d3_VecEqual(*(int *)(o + 0x61) + 0x4c, *(int *)(c + 0x61) + 0x4c, (float)*(int *)(ev + 0xf0)) != 0)
            *(int *)(ev + 0xac + (unsigned short)k++ * 4) = *(int *)(o + 1);
    }
    *(int *)(ev + 0x100) = 0;
    *(int *)(ev + 0x104) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
}
