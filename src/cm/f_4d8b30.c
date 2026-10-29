/* 0x4d8b30, clears the 16 ints at 0x92ca80 */

void ai_Unk4d8b30()
{
    memset(ai_Unk92ca80, 0, 0x40);
}
