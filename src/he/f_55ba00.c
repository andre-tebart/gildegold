/* 0x55ba00, the allocation handler of a building event: both clocks now, due 1 unit later, the building +0xac (none: free) is +0x10 and gets the message 0x5a / 2 / 0x80 when its byte +0x5a is 0 */

void he_Unk55ba00(int ev)
{
    int b;

    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(int *)(ev + 0x10) = *(int *)(b + 1);
    if (*(unsigned char *)(b + 0x5a) == 0)
        cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x80, 0);
}
