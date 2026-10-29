/* 0x574550, the alloc of the event 0xa1 (a meeting of up to four figures +0x8c ..): when one of them is in the building +0x10
   (0x505df0, +0x16c, +1) the start and the due clock +0x44 / +0x52 = now, two indices of the table 7 / 0x16 / 0xf / 0xa
   (0x505ca0 (4, out)) go to +0xc0 / +0xc2 and +0xc4 / +0xc6, the words +0xc8 .. +0xce = 0, +0xac = 0, +0xb0 / +0xb4 = 0xf0,
   +0xbc = 0, +0xd0 = -1 and state 0; else the event is freed */

void he_Unk574550(int ev)
{
    unsigned short w[4];
    int out[2];
    char *per;
    int i, bld;

    w[0] = 7;
    w[1] = 0x16;
    w[2] = 0xf;
    w[3] = 0xa;
    bld = 0;
    for (i = 0; i < 4; i++) {
        per = (char *)gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (per == 0)
            continue;
        bld = HM_I(per, 0x16c);
        if (bld == 0)
            continue;
        if (HM_I(bld, 1) == HM_I(ev, 0x10))
            break;
    }
    if (i == 4) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_Shuffled(4, out);
    *(unsigned short *)(ev + 0xce) = 0;
    *(unsigned short *)(ev + 0xc0) = w[out[0]];
    *(unsigned short *)(ev + 0xc2) = w[out[1]];
    *(unsigned short *)(ev + 0xcc) = 0;
    *(unsigned short *)(ev + 0xc4) = w[out[0]];
    *(unsigned short *)(ev + 0xc6) = w[out[1]];
    *(unsigned short *)(ev + 0xca) = 0;
    *(unsigned short *)(ev + 0xc8) = 0;
    HM_I(ev, 0xac) = 0;
    HM_I(ev, 0xb0) = 0xf0;
    HM_I(ev, 0xb4) = 0xf0;
    HM_I(ev, 0xbc) = 0;
    HM_I(ev, 0xd0) = -1;
    HM_I(ev, 0x70) = 0;
}
