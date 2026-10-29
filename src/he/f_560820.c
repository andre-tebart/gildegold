/* 0x560820, ends a duplicate: of the records of type 0x3c (0x53e400 / 0x53e300) the one with the same +0xac and +0xb4 as `ev` (not ev itself) sets the state -1 of `ev`; a building (+0xb4, 0x501190) with +0x65 = -1 or a figure +0xac (0x505df0) with the flag +0x1b1 does the same; the due clock is now and the event posted (0x4fe140 (.., 0, 0, 1)) */

void he_Unk560820(int ev)
{
    char *e;
    int b, p;

    for (e = he_FindEvent(1, 0, 0x3c); e != 0; e = he_NextEvent()) {
        if ((int)e != ev && *(int *)(e + 0xb4) == *(int *)(ev + 0xb4) && *(int *)(e + 0xac) == *(int *)(ev + 0xac))
            *(int *)(ev + 0x70) = -1;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    if (b != 0 && *(int *)(b + 0x65) == -1)
        *(int *)(ev + 0x70) = -1;
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0 && *(unsigned char *)(p + 0x1b1) != 0)
        *(int *)(ev + 0x70) = -1;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
