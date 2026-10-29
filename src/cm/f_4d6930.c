/* 0x4d6930, the result of the fight in the record r (+0x354 / +0x358): 0x6f0cf8 set -> 0 and the ransom 0x6f0cfc, else 1 */

void ai_Unk4d6930(char *r)
{
    if (ai_Unk6f0cf8 != 0) {
        *(int *)(r + 0x354) = 0;
        *(int *)(r + 0x358) = ai_Unk6f0cfc;
    } else
        *(int *)(r + 0x354) = 1;
}
