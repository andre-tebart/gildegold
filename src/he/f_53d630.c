/* 0x53d630, makes room in the 16 slots: finds the slot whose event (record + 0x44, a clock) is the oldest, frees it (0x53d800 / 0x53e1f0) and returns 0xf; -1 when no slot is used */

int he_Unk53d630(void)
{
    int clk[4];
    int best = 0;
    int bi = -1;
    int i = 0;
    int *p;

    clk[0] = U_I((unsigned)&ai_Time);
    clk[1] = U_I((unsigned)&ai_TimeHour);
    clk[2] = U_I(((unsigned)&ai_Time + 0x8));
    *(short *)(clk + 3) = *(short *)((unsigned)&ai_Time + 0xc);
    p = (int *)(unsigned)&full_data_6faa80;
    do {
        if (p[0] != 0 && p[1] != -1) {
            if (gm_CompareTime(p[0] + 0x44, clk) < 0) {
                best = p[0];
                clk[0] = *(int *)(best + 0x44);
                clk[1] = *(int *)(best + 0x48);
                clk[2] = *(int *)(best + 0x4c);
                *(short *)(clk + 3) = *(short *)(best + 0x50);
                bi = i;
            }
        }
        p += 3;
        i++;
    } while ((int)p < (unsigned)((unsigned)full_data_6faa80 + 0xc0));
    if (best != 0) {
        he_Unk53d800(best);
        he_Unk53e1f0(best);
        return 0xf;
    }
    if (bi != -1) {
        __asm int 3
    }
    return -1;
}
