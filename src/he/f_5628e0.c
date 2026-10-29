/* 0x5628e0, the run handler "GebSpionage" (spying on a building; ROUGH skeleton, 2064 bytes): like the espionage walk 0x55a030 the spies (group 0x524260 (building, .., id, ev), 0x4c5230, 0x4c52e0 "GebSpionage") walk to the building (0x524770), look around (gm_Unk504e00, 0x5090b0, _ftol rolls), the owner is told (the news "_NACHRICHTEN_HS_78", 0x501ce0 shows the building) and the group released; the record is freed at the end */

void he_Unk5628e0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p;
    char text[0x200];

    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (b == 0 || p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    ai_Unk4c5230(*(int *)(p + 4));
    ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "GebSpionage");
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    gm_GetGebaeudeHauptraum(b);
    txt_Format(text, 0x1bba, b);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_78", 0);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
