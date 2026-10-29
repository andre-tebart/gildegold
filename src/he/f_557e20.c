/* 0x557e20, the run handler of the herb gathering trip ("Kr\344uter" animation; ROUGH skeleton, 7 states by the jump table 0x5584a8; a season check 0x4fe350 against the float tables 0x617830 / 0x617840): the +0xac group persons (ids from +0x8c) are sent to the building +0x10 / the field positions (0x501190, 0x458a20, the 3D distance test against the range), gather ("Kr\344uter" 0x4c52e0, 0x4c5230), arrive (0x524770) and are given the harvest (the amount from 0x5092e0 and two random rolls 0x42ec00, 0x50ab80 / 0x50a810 for the owner), paid out by 0x4c4880 (.., -1) and the news 0x17fd / 0x1802 .. 0x1804 is shown to the owner (0x53dee0); the end frees the record and the persons (0x524260) */

int gm_ClockSeason();
int gm_FreeCapacity();
int sv_Unk524770();

void he_Unk557e20(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, i, p, n = 0, o;
    char text[0x400];
    float pos[3], pos2[3];

    if (gm_ClockSeason((unsigned)&ai_Time) < 0 || st < -2 || st > 4) {
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
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0)
            return;
        o = *(int *)(b + 0x61);
        d3_PointToWorld(o, o + 0x4c, pos);
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0) {
                ai_Unk4c5230(*(int *)(p + 4));
                ai_Unk4c52e0(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Kr\344uter");
                n++;
            }
        }
        *(int *)(ev + 0x70) = 1;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    default:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0 && sv_Unk524770(p, 0) != 0) {
                n++;
                txt_Format(text, 0x17fd, p);
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
            }
        }
        (void)pos2;
        he_Unk53e1f0((int *)ev);
        return;
    }
}
