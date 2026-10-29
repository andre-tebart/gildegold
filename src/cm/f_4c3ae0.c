/* 0x4c3ae0, the state of slot h, -1: no slot */

int cm_GetSlotState(int h)
{
    if (h >= 0 && h < 0x8000)
        return ai_Unkf31160[h].state;
    return -1;
}
