/* 0x44e570, exactly one bit set */
char agi_IsPow2(int n)
{
    int i;
    int bits = 0;

    for (i = 0; i < 32; i++) {
        bits += n & 1;
        n >>= 1;
    }
    return bits == 1;
}
