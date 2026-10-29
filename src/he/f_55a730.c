/* 0x55a730, the run handler of the brawl event "PRUEGEL" (2112 bytes; ROUGH skeleton like 0x558f10): the two brawlers (+0xac / +0xb0 and the job handles 0x4c3ae0 / 0x4c3b10) walk to the building (0x4c52e0, the detection 0x553ae0, 0x4f2e30 relation, the guards 0x53e400, 0x4c4ed0), the fight is decided by the skills (0x494d90, 0x5ba6b0, 0x50cc50 twice) and "_NACHRICHTEN_HS_17" tells the winner / loser (0x53dda0 / 0x53dee0), the end releases the group (0x524260) and frees the record */

void he_Unk55a730(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b, h;
    char text[0x200];

    if (st < -2 || st > 6) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || st < 0) {
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    h = *(int *)(ev + 0xcc);
    if (h != -1 && cm_GetSlotState(h) == 0)
        return;
    ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "PRUEGEL");
    txt_Format(text, 0x1bba, b);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, 0);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0x70) = st + 1;
}
