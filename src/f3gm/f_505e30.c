/* 0x505e30, the age (word +10) of the character with the number id as a float */

float gm_CharAge(unsigned id)
{
    return (float)*(unsigned short *)(ai_Chars[id & 0xffff] + 10);
}
