/* 0x55bd20, the allocation handler of a building event: the building +0xac (when found) gets the message 0x5a / 2 / 0x400, the due clock is now */

void he_Unk55bd20(int ev)
{
    int b;

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b != 0)
        cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x400, 0);
    *(HETIME *)(ev + 0x52) = ai_Time;
}
