/* 0x5525a0, the run handler of the migration event (ROUGH): +0xac people (at most sqrt(+0xac + +0xb0)) of the 0x300 characters (the ring from a random start; id != 0xffff, kind <= 4, byte +0xc = +0xb4) are sent away (0x4c3ba0, 0x4c3c00 (1, 1, 0xc, ..), 0x4c49a0); +0xb0 / +0xac are reduced and the event posted 0x18 later, or 5 later with the handle -1 when not enough were found */

void he_Unk5525a0(int ev)
{
    int cnt = 0, n, a, b, ring, left, ch;
    double s;
    int x[4];

    if (*(int *)(ev + 0x70) < 0 || *(int *)(ev + 0xb0) <= 0 || *(int *)(ev + 0xac) <= 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    a = *(int *)(ev + 0xac);
    b = *(int *)(ev + 0xb0);
    s = sqrt((double)(a + b));
    if (s > (double)a)
        n = (int)(double)*(int *)(ev + 0xac);
    else
        n = (int)sqrt((double)(*(int *)(ev + 0xb0) + *(int *)(ev + 0xac)));
    ring = (unsigned short)util_Random(0x300);
    for (left = 0x300; left != 0; left--) {
        if (cnt >= n)
            break;
        ch = (unsigned)&ai_Chars + ring * 0x21c;
        if (*(unsigned short *)ch != 0xffff && *(unsigned char *)(ch + 2) <= 4 && *(unsigned char *)(ch + 0xc) == *(int *)(ev + 0xb4)) {
            cm_BeginFieldBlock(ch, *(int *)(ch + 4));
            cm_AddFieldDelta(1, 1, 0xc, x);
            cm_SendFieldDeltas();
            cnt++;
        }
        ring = (ring + 1) % 0x300;
    }
    if (cnt < n) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
    *(int *)(ev + 0xac) = *(int *)(ev + 0xac) - n;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
