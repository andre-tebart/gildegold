/* 0x4b3a10, a random number below n (LCG ai_MissionSeed) */

unsigned int ai_MissionRandom(unsigned int n)
{
    ai_MissionSeed = ai_MissionSeed * 0x41c64e6d + 0x3039;
    return (*((unsigned short *)&ai_MissionSeed + 1) % 0x7fffu) % n;
}
