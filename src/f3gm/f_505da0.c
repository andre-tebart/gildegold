/* 0x505da0, a random age for the value n: for 0 a value of 0x15 .. 0x3e, below 42 n + (0 .. 5), else n + (0 .. 10) */

char gm_RandomAge(unsigned char n)
{
    char c;
    int u;

    if (n == 0) {
        c = (char)util_RandomU(0x2a);
        return c + 0x15;
    }
    u = 0xb;
    if ((float)n < 42.0f)
        u = 6;
    c = (char)util_RandomU(u);
    return n + c;
}
