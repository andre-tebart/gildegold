/* 0x560900, the run handler of the prison visit / arrest event (dummy "dummy_EINGANG_ZELLE", the news "_NACHRICHTEN_HS_71" / "_26" / "_72" / "_73"; ROUGH skeleton, 2096 bytes): the guard and the prisoner (+0xac, group 0x524260, 0x4c5230 / 0x4c52e0 send them, 0x524770 arrival), the cell entrance and the sentence by a roll (util_RandomF, gm_BuildingSkill, 0x5ba6b0 skill), the news to the player (0x53dee0 / 0x53dda0) and the release; the end frees the record */

void he_Unk560900(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b;
    char text[0x200];

    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (p == 0 || b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    gm_GetGebaeudeHauptraum(b);
    ai_Unk4c5230(*(int *)(p + 4));
    ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "dummy_EINGANG_ZELLE");
    txt_Format(text, 0x1bba, b);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_71", 0);
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
