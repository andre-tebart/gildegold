/* 0x557b70, the run handler "he_RunLagerErweitern" (the warehouse extension, ROUGH skeleton): state 0 .. 3 (jump table 0x557d58): waits for the due time (0x4fe210), moves the person +0xac (0x4fea50; 0x4c3ba0 / 0x4c3d00 / 0x4c49a0 raise a field), tells the owner (the news 0x1bbb, 0x53dee0 with 0x501b50's contact and the 0x50a810 owner flag) and frees; the default is the debug text "he_RunLagerErweitern():Wrong LagerID..." (0x4222b0) and the free */

void he_Unk557b70(int ev)
{
    int st = *(int *)(ev + 0x70);
    int o = 0, b, q, own;
    char text[0x400];

    if (st < 0 || st > 3) {
        err_Message("he_RunLagerErweitern():Wrong LagerID...");
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
        return;
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    cm_BeginFieldBlock(o, *(int *)(o + 2));
    cm_AddFieldValues(1, 1, st, text);
    cm_SendFieldDeltas();
    b = gm_BuildQuery(1, 0, *(int *)(ev + 0x10));
    if (b != 0) {
        own = gm_FindCharacterByMaster(b);
        q = gm_BuildingMainObject(b);
        txt_Format(text, 0x1bbb, b);
        if (q != 0)
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, (own != 0 && (*(unsigned char *)(own + 0x1b4) & 1) != 0) ? 0x60b : 0x5f6, *(int *)(b + 1), *(int *)(q + 2), 0, 0);
        else
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x5f6, -1, 0, 0, 0);
    }
    he_Unk53e1f0((int *)ev);
}
