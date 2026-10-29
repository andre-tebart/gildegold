/* 0x558a40, the run handler of the sabotage event (ROUGH skeleton): waits for the due time, rolls the success from the saboteur's skill (0x509400 x2, 0x5092e0 and the _ftol rolls) against the building, then marks the building / person (0x4c3ba0 / 0x4c3d00 / 0x4c49a0) and tells the owner (0x501ce0, the news "_NACHRICHTEN_HS_81" 0x53dee0); the group is released (0x524260) and the record freed */

void he_Unk558a40(int ev)
{
    int p, b, q;
    char text[0x200];

    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
        return;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b != 0) {
        q = gm_FindCharacterByMaster(b);
        txt_Format(text, 0x1bba, b);
        gm_GetGebaeudeHauptraum(b);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), q != 0 ? *(int *)(q + 2) : -1, "_NACHRICHTEN_HS_81", 0);
    }
    sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
    he_Unk53e1f0((int *)ev);
}
