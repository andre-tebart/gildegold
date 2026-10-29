/* 0x4a86f0, the NPC whose turn it is (unkb0) sets the stake: up to half the margin above the stake, or all of it
   if it bluffs */

void ai_Unk4a86f0(void)
{
    int n;

    if (ai_Unkf83654->unkb0 != 0) {
        n = ai_Unkf83654->seat[ai_Unkf83654->unkb0].unk20;
        if (n == 0)
            n = ai_Unkf83654->money - ai_Unkf83654->stake >> 1;
        else
            n = (unsigned short)(ai_Unkf83654->money - ai_Unkf83654->stake);
        ai_Unkf83654->stake2 = util_Random(n) + ai_Unkf83654->stake;
    }
}
