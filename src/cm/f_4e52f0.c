/* 0x4e52f0, a random number below n (logged "cut_rnd: %i"), 0 for n == 0 */

unsigned int ai_Unk4e52f0(unsigned int n)
{
    char buf[0x40];

    if (n == 0)
        return 0;
    ai_Unk92c4e0 = ai_Unk92c4e0 * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rnd: %i", G28_HI % 0x7fffu);
    return (G28_HI % 0x7fffu) % n;
}
