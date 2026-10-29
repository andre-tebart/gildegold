/* 0x4c3b10, the value of slot h */

int cm_GetSlotValue(int h)
{
    if (h >= 0 && h < 0x8000)
        return ai_Unkf31160[h].value;
    return 0;
}
