/* 0x4bb7d0, a random number below n (LCG ai_Unk6f0730; 0 for n 0) */

unsigned int ai_Unk4bb7d0(unsigned int n)
{
    if (n == 0)
        return 0;
    ai_Unk6f0730 = ai_Unk6f0730 * 0x41c64e6d + 0x3039;
    return (*((unsigned short *)&ai_Unk6f0730 + 1) % 0x7fffu) % n;
}
