/* 0x504510, the date a character n (the player slot of 0x770b80) next gets its portion: from the character's kind (0x504ce0 must be 1 .. 5, 0x504ad0 gives a class 8 .. 12 mapped to 0 .. 4: 11 -> 2, 12 / 10 -> 3, 8 -> 0, 9 -> 4, else 1) 0x504ec0 gives a period; the date is computed from the clock 0x770ae0 (day, hour part 0x770ae4, with the fraction 6 .. 21 of the day: before 7 the hour + 6, above 21 the next day), compared with the character's own date +0xcf4 (0x4fe210), and when later it is written to out (3 words and a short, 0x4fe280 / 0x4fe140 add the fraction in ticks of 0x18 per day). Returns the fraction as a float (0.0 when the character has no portion kind, 1.0 when the date is not later). ROUGH: all float operands are lost in the pseudocode (the __ftol calls) */

float gm_Unk504510(int n, int *out)
{
    int k, cls, period;
    int d[4], e[4];
    unsigned short hour;
    unsigned t;
    float f;
    int c;

    k = gm_TitlesLeft(ai_Chars[n][0x164]);
    if (k == 0 || k > 5)
        return 0.0f;
    cls = gm_TitleGroup(ai_Chars[n][0x164]);
    if (cls == 0xb)
        period = 2;
    else if (cls == 0xc)
        period = 3;
    else if (cls == 10)
        period = 3;
    else if (cls == 8)
        period = 0;
    else if (cls == 9)
        period = 4;
    else
        period = 1;
    f = gm_GetSkill(ai_Chars[n], period);
    (void)(int)f;                                /* lost */
    memset(d, 0, sizeof(d));
    memset(e, 0, sizeof(e));
    d[0] = (int)f;                               /* lost */
    hour = (unsigned short)(int)f;               /* lost */
    e[0] = hour;
    if ((float)hour <= 21.0f) {
        if ((float)hour < 7.0f)
            e[0] = hour + 6;
    } else {
        d[0]++;
        e[0] = (short)(int)f;                    /* lost */
    }
    t = *(unsigned *)(ai_Time + 4);
    c = *(int *)(ai_Chars[n] + 0x174);
    e[1] = (short)t;
    e[3] = c;
    if (gm_CompareTime(e + 3, d) < 0) {
        gm_MinutesBetween(e + 0, e + 3);
        gm_MinutesBetween(e + 0, d);
        gm_AddTime(d, (t & 0xffff) + *(int *)ai_Time * 0x18, 0, 0);
        f = (float)((int (*)(struct GCLOCK *, int, int, int))gm_AddTime)(d, c * -0x18 - (t & 0xffff), 0, 0);
        out[0] = d[0];
        out[1] = e[0];
        out[2] = e[2];
        *(short *)(out + 3) = (short)e[3];
        return f;
    }
    return 1.0f;
}
