/* 0x5639c0, the run handler "Angriff" (the attack of a gang on a building; ROUGH skeleton, 2336 bytes): the attackers (group 0x524260, 0x4c5230, 0x4c52e0 "Angriff") walk to the building (0x524770), the victims of the query list (gm_Query / gm_QueryNext, util_Random) are picked, the fight is started (0x4c4fa0, 0x4d3d10) and the news 0x5176d0 / 0x53dee0 shown; the record is freed at the end */

void he_Unk5639c0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p;

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
    ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Angriff");
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
