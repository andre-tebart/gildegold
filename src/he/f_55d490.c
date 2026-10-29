/* 0x55d490, the allocation handler of the harvest event (ROUGH): the due clock is now (+1), state 0; the building +0xac (0x501190) and the field object +0xb8 (0x4bc670) and the worker +0xb0 (0x505df0; a free worker without job / not of the job 0x28: the group 0x524260 (building, 0xfd contact via 0x4ffe90, ..)) are set up, the job handle +0xc0 = 0x4c5050 (building, 1), +0xb4 = -1 and the word +0xbc = the minute word; a missing worker frees the record */

void he_Unk55d490(int ev)
{
    int b, p, o;

    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    *(int *)(ev + 0x70) = 0;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b != 0)
        veg_FindPlantId(b, *(int *)(ev + 0xb8));
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        sv_MoveToGroup(0, 0, 0, ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (b != 0 && *(int *)(b + 0x10) != -1 && *(unsigned char *)(p + 0x180) != 0x28) {
        o = gm_Query(0, 1, 0, 0xfd);
        sv_MoveToGroup(b, o, *(unsigned short *)p, ev);
    }
    *(int *)(ev + 0xc0) = ((int (*)(int, int))cm_SetPlant)(*(int *)(ev + 0xac), 1);
    *(int *)(ev + 0xb4) = -1;
    *(unsigned short *)(ev + 0xbc) = *(unsigned short *)(ev + 0xbc);
}
