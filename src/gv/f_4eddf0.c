/* 0x4eddf0, resets the effect slots (0x6f1008, 8 of 0x170 bytes): all zero, then id -1 and the floats +8 / +0xc / +0x10 = 1.0; copies the four ints 0x770ae0 .. to 0x6f0e38 ..; clears 8 ints at 0x925100 and sets the first to -1 */

void unk_ResetEffectSlots(void)
{
    int *p;
    int i;

    memset(gv_Effects, 0, 0xb80);
    for (p = (int *)gv_Effects; (int)p < (unsigned)((unsigned)gv_Effects + 0xb80); p += 0x5c) {
        p[0] = -1;
        *(float *)&p[2] = 1.0f;
        *(float *)&p[3] = 1.0f;
        *(float *)&p[4] = 1.0f;
    }
    *(int *)ai_Unk6f0e38 = *(int *)ai_Time;
    *(int *)ai_Unk6f0e3c = *(int *)(ai_Time + 4);
    *(int *)(ai_Unk6f0e38 + 8) = *(int *)(ai_Time + 8);
    *(int *)(ai_Unk6f0e38 + 12) = *(int *)(ai_Time + 12);
    memset(ai_Unk925100, 0, 0x20);
    *(int *)ai_Unk925100 = -1;
}
