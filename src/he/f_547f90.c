/* 0x547f90, a Ghidra miss (disassembled): the run handler of the "RndGeselle" event: the objects of +0xb0 / +0xac are fetched (0x4fea50); a gone one ends the event, in the state 0 with the finished job +0xbc the figure it returns (0x4c3b10) is sent to +0xb0 / +0xb4 ("RndGeselle", 0x4c52e0) and the news (id +0xb8, 0x20d + 0x23 for party 0 + its record byte +0x165) is shown (0x53dee0) (ROUGH) */

void he_Unk547f90(int ev)
{
    char text[0x200];
    int o2 = 0, o1 = 0;
    int r, k;

    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 4, 0);
    gm_FindById(&o1, 0, 0, *(int *)(ev + 0xb0));
    gm_FindById(0, 0, &o2, *(int *)(ev + 0xac));
    if (*(int *)(ev + 0x70) == -2 || o1 == 0)
        he_Unk53e1f0((int *)ev);
    if (*(int *)(ev + 0x70) == 0 && cm_GetSlotState(*(int *)(ev + 0xbc)) != 0) {
        r = cm_GetSlotValue(*(int *)(ev + 0xbc));
        if (r != 0) {
            k = ((*(char *)(r + 9) != 0) ? 0x23 : 0) + 0x20d + *(unsigned char *)(r + 0x165);
            ai_Unk4c52e0(*(int *)(r + 4), *(int *)(ev + 0xb0), *(int *)(ev + 0xb4), 0, 0, "RndGeselle");
            txt_Format(text, *(unsigned short *)(ev + 0xb8), *(unsigned short *)r, k, o2);
            he_MessageBoxQuickjump(*(int *)(ev + 0xac), *(int *)(ev + 0xac), text, 0, 0x5ec, *(int *)(ev + 0xb0), *(int *)(ev + 0xb4), 0, "_NACHRICHTEN_HS_00");
        }
        he_Unk53e1f0((int *)ev);
    }
}
