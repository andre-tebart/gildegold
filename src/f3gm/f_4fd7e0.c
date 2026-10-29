/* 0x4fd7e0, 1 when the character id is younger than its lifetime float (+0x20): age (+10) below it */

int unk_Unk4fd7e0(unsigned id)
{
    if ((float)*(unsigned short *)(ai_Chars[id & 0xffff] + 10) < *(float *)(ai_Chars[id & 0xffff] + 0x20))
        return 1;
    return 0;
}
