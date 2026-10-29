/* 0x552820, the run handler of a one-shot event: states -1 / -2 free the record; else unless flag 4 the event is posted with the handle -1 (0x4c4c60 (-1, ev), due clock now, handle in +0x84) */

void he_Unk552820(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st == -1 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
