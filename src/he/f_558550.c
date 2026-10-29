/* 0x558550, the run handler of the animal hunt ("Getier", the second string 0x6274c0; ROUGH skeleton, 4 states by the jump table 0x5588a8): state 0 sends the hunters (0x505df0, 0x4c5230, 0x4c52e0 (.., "Getier" 0x6274c0)) and waits a random time (0x42ec00), state 1 waits for the due time (0x4fe210), looks for the building (0x501190, 0x4ffe90, the contact 0x501b50 / 0x50a810 owner) and pays out the catch (0x5092e0 and a random roll, 0x4c4880 (.., -1), the news 0x1805 / 0x1809 / 0x180a for the owner and the players of kind 6 / 7 (0x53dee0)); the end frees the persons (0x524260) and the record */

void he_Unk558550(int ev)
{
    int st = *(int *)(ev + 0x70);
    int i, p, b, q;
    char text[0x200];

    if (st < -2 || st > 1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (st) {
    case -2:
    case -1:
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0)
                sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0) {
                ai_Unk4c5230(*(int *)(p + 4));
                ai_Unk4c52e0(*(int *)(p + 4), *(int *)(ev + 0x10), -1, 0, 0, "Getier");
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(10), 0);
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b != 0) {
            q = gm_BuildingMainObject(b);
            txt_Format(text, 0x1805, b);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), q != 0 ? *(int *)(q + 2) : -1, 0, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
}
