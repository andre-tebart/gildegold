/* 0x54c570, a Ghidra miss (disassembled; 756 B): the run handler of the accident simulation ("RunSimUnfall: calculating at <Day = %i Time = %i.2:%i.2>"): the first run picks a start from 16 primes (+0xb0) and a random start figure (+0xac, of 0x300); state 0 walks the 0x300 figures of the ring in 12 steps per run (+0xac += +0xb0, +0xb4 counts) and for a figure that passes 0x54c510 and has a boss rolls 25 - 0.0125 * 20 * 0x5092e0 (.., 2, 1) against a random 0 .. 100: the accident 0x5414a0 (with a random damage 0x4fca40 of -50 - random(1 + 2 * ..)); when all 0x300 are done the due clock is set on and the counters reset, else it is posted 0x1e units ahead (ROUGH) */

void he_Unk54c570(int ev)
{
    char text[0x100];
    int primes[16];
    int i, n, start, rec, boss, cnt, cur;
    float r, sk, lim, pr;

    if (*(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    sprintf(text, "-------> RunSimUnfall: calculating at <Day = %i Time = %i.2:%i.2>", *(int *)(unsigned)&ai_Time, *(unsigned short *)(unsigned)&ai_TimeHour, *(unsigned short *)((unsigned)&ai_Time + 0x6));
    if (*(int *)(ev + 0xb0) == 0) {
        int pr0[16] = { 1, 3, 5, 7, 0xb, 0xd, 0x11, 0x13, 0x2ed, 0x2ef, 0x2f3, 0x2f5, 0x2f9, 0x2fb, 0x2fd, 0x2ff };

        for (i = 0; i < 16; i++)
            primes[i] = pr0[i];
        *(int *)(ev + 0xb0) = primes[(unsigned short)util_Random(0x10)];
        *(int *)(ev + 0xac) = (unsigned short)util_Random(0x300);
    }
    if (*(int *)(ev + 0x70) != 0)
        return;
    cur = *(int *)(ev + 0xac);
    for (n = 12; n > 0; n--) {
        rec = (unsigned)&ai_Chars + cur * 0x21c;
        if (he_Unk54c510(rec) != 0 && *(unsigned short *)(*(int *)(rec + 0x16c) + 0x27) != 0xffff) {
            r = util_RandomF() * 100.0f;
            sk = (float)gm_BuildingSkill(*(int *)(rec + 0x16c), 2, 1) * 0.0125f;
            if (!(25.0f - 20.0f * sk <= r)) {
                lim = 2.0f - r;
                if (lim < 0.0f)
                    lim = 0.0f;
                if (lim > 2.0f)
                    lim = 2.0f;
                pr = lim * 255.0f + 1.0f;
                gm_AddHealth(cur, -50 - (unsigned short)util_Random((int)pr));
                he_Unk5414a0(ev, (unsigned short *)rec, (unsigned)&ai_Chars + *(unsigned short *)(*(int *)(rec + 0x16c) + 0x27) * 0x21c, *(int *)(rec + 0x16c), lim, 0);
            }
        }
        cur = (*(int *)(ev + 0xb0) + cur) % 0x300;
        *(int *)(ev + 0xb4) = *(int *)(ev + 0xb4) + 1;
        if (*(int *)(ev + 0xb4) == 0)
            break;
    }
    *(int *)(ev + 0xac) = cur;
    cnt = *(int *)(ev + 0xb4);
    if (cnt >= 0x300) {
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0;
        *(int *)(ev + 0xb4) = 0;
        *(int *)(ev + 0x58) = 0;
        *(short *)(ev + 0x56) = 0xa;
        return;
    }
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    if (*(unsigned short *)(ev + 0x56) >= 0x12) {
        *(int *)(ev + 0x58) = 0;
        *(short *)(ev + 0x56) = 0xa;
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
    }
}
