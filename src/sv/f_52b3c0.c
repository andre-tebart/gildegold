/* 0x52b3c0, finds the 0x75ec80 record of the character kind +0x180 of c (when c is not flagged 0x1000000, +0x16c == b, and the record is not time-limited or the day time (0x770ae4)
   lies in the window 0x617830 / 0x617840 of the current hour slot) and copies its name to out; returns 1 or 0 */

int sv_Unk52b3c0(char *c, int b, char *out)
{
    int h, i;
    unsigned *p, u;

    h = gm_ClockSeason((unsigned)&ai_Time);
    i = 0;
    if (SV_I((unsigned)&full_data_75ec80) != 0) {
        p = (unsigned *)(unsigned)&full_data_75ec80;
        u = SV_U((unsigned)&full_data_75ec80);
        while (u != *(unsigned char *)(c + 0x180)) {
            u = p[8];
            p += 8;
            i++;
            if (u == 0)
                return 0;
        }
        if ((*(unsigned *)(c + 0x1cc) & ((unsigned)&txt_Keys + 0x181c0)) == 0 && *(int *)(c + 0x16c) == b
            && (((int *)(unsigned)&full_data_75ec80)[i * 8 + 5] == 0
                || ((float)SV_US((unsigned)&ai_TimeHour) < *(float *)((unsigned)&ai_Unk617840 + h * 4) && *(float *)((unsigned)&ai_Unk617830 + h * 4) <= (float)SV_US((unsigned)&ai_TimeHour)))) {
            strcpy(out, (char *)((unsigned)&full_data_75ec84 + i * 0x20));
            return 1;
        }
    }
    return 0;
}
