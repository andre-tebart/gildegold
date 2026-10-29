/* 0x557d70, the allocation handler of the group event: the +0xac persons (ids from +0x8c) that exist are sent (0x524260 (0, 0, type, ev)); with none the record is freed, else state 1 and the event is posted 5 later */

void he_Unk557d70(int ev)
{
    int i, p, any = 0;

    for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
        p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
        if (p != 0) {
            any |= 1;
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        }
    }
    if (any == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x70) = 1;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
