/* 0x506000, the birthday of the character c as a calendar record (month byte, day byte, year word) in out: the birth date (the clock minus the age +10, an inactive character from the word +0x28), the month = ((x >> 3) % 13 + the birth day % 11) % 12 + 1 and the day = (x >> 5) % 28 + 1 with x = the int +0x30 xor the year */

void gm_BirthDate(char *c, unsigned *out)
{
    int clk[4];
    unsigned char d[16];
    int day;
    unsigned x;

    ((int *)clk)[1] = *(int *)(ai_Time + 4);
    ((int *)clk)[2] = *(int *)(ai_Time + 8);
    day = *(int *)ai_Time;
    if (c[8] == 0)
        day = *(short *)(c + 0x28);
    day = day - *(unsigned short *)(c + 10);
    clk[0] = day;
    gm_ClockToDate(clk, d);
    x = *(unsigned *)(c + 0x30) ^ *(unsigned short *)(d + 2);
    d[1] = (char)(((x >> 3) % 0xd + day % 0xb) % 0xc) + 1;
    d[0] = (char)((x >> 5) % 0x1c) + 1;
    out[0] = *(unsigned *)d;
    out[1] = *(unsigned *)(d + 4);
    *(short *)(out + 2) = *(short *)(d + 8);
}
