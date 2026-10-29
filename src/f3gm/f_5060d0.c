/* 0x5060d0, the birthday of an inactive character c (word +0x28 the birth day) into out; 0 for an active one, 1 else (month = ((x >> 3) % 7 + day % 19) % 12 + 1) */

int gm_BirthDate2(char *c, unsigned *out)
{
    short day;
    int clk[4];
    unsigned char d[16];
    unsigned x;

    if (c[8] != 0)
        return 0;
    day = *(short *)(c + 0x28);
    clk[0] = day;
    gm_ClockToDate(clk, d);
    x = *(unsigned *)(c + 0x30) ^ *(unsigned short *)(d + 2);
    d[1] = (char)(((x >> 3) % 7 + day % 0x13) % 0xc) + 1;
    d[0] = (char)((x >> 5) % 0x1c) + 1;
    out[0] = *(unsigned *)d;
    out[1] = *(unsigned *)(d + 4);
    *(short *)(out + 2) = *(short *)(d + 8);
    return 1;
}
