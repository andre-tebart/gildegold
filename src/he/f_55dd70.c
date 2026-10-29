/* 0x55dd70, the allocation handler of the 0x55de30 event (ROUGH): another record of type 0x32 for the same figure (+0xac) is ended (+0xb0 = -1), the due clock is now + 1 and a missing figure frees the record; a flagged (2) record sends the message 0x1cc / 4 / 0x1000000 to the figure */

void he_Unk55dd70(int ev)
{
    char *e;
    int p;

    for (e = he_FindEvent(1, 0, 0x32); e != 0; e = he_NextEvent()) {
        if ((int)e != ev && *(int *)(e + 0xac) == *(int *)(ev + 0xac))
            *(int *)(e + 0xb0) = -1;
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 2) != 0)
        cm_SendSetField(*(int *)(p + 4), 0x1cc, 4, ((unsigned)&txt_Keys + 0x181c0), 0);
}
