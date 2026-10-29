/* 0x4e3210, the scene's tick counter 0x6f0d80: 0x6f0d88 tells whether it was past 0x6f0d84 */

int ai_Unk4e3210(void)
{
    ai_Unk6f0d88 = ai_Unk6f0d80 > ai_Unk6f0d84;
    ai_Unk6f0d80++;
}
