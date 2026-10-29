/* 0x56b3f0, a run handler (state 0 when due: the order 0x4c4880 of the good +0xac to the building +0xb4, its handle at +0xc8; state 1 waits for the handle (0x4c3ae0), and when it ended without error
   (not 2) takes the building (+0xb8 = its +2) and starts a countdown of 0x20 (+0xc0 / +0xc4); state 2 each round looks the building up (0x4fea50) and at the end of the countdown runs 0x50d380 for it and frees the record) */

void init_HookObject(char *o, unsigned short *p, int kind);

void he_Unk56b3f0(int ev)
{
    int bo[1], g;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        goto done;
    case 0:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) > 0)
            return;
        *(int *)(ev + 0xc8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(ev + 0xac), -1, *(int *)(ev + 0xb4), 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (cm_GetSlotState(*(int *)(ev + 0xc8)) == 0)
            return;
        if (cm_GetSlotState(*(int *)(ev + 0xc8)) == 2)
            goto done;
        g = cm_GetSlotValue(*(int *)(ev + 0xc8));
        *(int *)(ev + 0xb8) = *(int *)(g + 2);
        *(int *)(ev + 0xc0) = 0x20;
        *(int *)(ev + 0xc4) = 0;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        do {
            gm_FindById(0, bo, 0, *(int *)(ev + 0xb8));
            gm_AddTime(ev + 0x52, 0, 1, 0);
            if (*(int *)(ev + 0xc4) > *(int *)(ev + 0xc0)) {
                init_HookObject(*(int *)(bo[0] + 0x18), bo[0], 1);
                goto done;
            }
        } while (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) < 0);
        return;
    }
    return;
done:
    he_Unk53e1f0((int *)ev);
}
