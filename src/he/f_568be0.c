/* 0x568be0, the run handler of a building event with a running income (a Ghidra miss; ROUGH skeleton, 1248 bytes, jump table 0x5690a4): the building 0x4fea50 (+0xb8); the owner record ebp (+0x27, not the local player); every player record
   whose +0x17c is this event is released (0x524260 (0, 0, id, 0)) when it ends: -1 / -2 / no building free; state 0 goes to 1; state 1 (while the counters +0x14 / +0x18 are non-zero) each minute step (0x509400 (+0x60, now, 0)) adds to the
   byte +0xc8, at 60 it re-rates the eight slots (+0x8c: 0x4c4b00 (owner +4, id, 2 - level, 0), the rate +0xc4 = (0x50c980 (id, 8, +0x8c) - 0.5) * 0.25) and while the due clock (+0x52, 0x4fe210) is past pays the amount from the stock
   +0xb0 to the party (0x4c4880 (+0xc0, +0xbc, id, min (+0x36 of the record, +0xe of the group), money 0x6f1bc5, 0), 0x4ffe90 (+0x14 of the building, 0, 1, id) group); an exhausted stock ends with the news 0x1bb7 (0x628a28) and
   frees; state 2 releases the players and frees when both counters are zero */

void he_Unk568be0(int ev)
{
    int buf[0x20], b2[0x20];
    int obj, ebp, rec, i, v, g, n, bld;
    float f1, f2;
    char text[0x100];

    ebp = 0;
    gm_FindById(buf, 0, 0, *(int *)(ev + 0xb8));
    obj = buf[4];
    if (obj == 0) {
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, 0, *(unsigned short *)rec, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(unsigned short *)(obj + 0x27) != 0xffff) {
        rec = (unsigned)&ai_Chars + *(unsigned short *)(obj + 0x27) * 0x21c;
        if (rec != U_I((unsigned)&ai_PlayerChar))
            ebp = rec;
    }
    switch (*(int *)(ev + 0x70)) {
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) == 0 && *(int *)(ev + 0x18) == 0) {
            *(int *)(ev + 0x70) = 2;
            return;
        }
        gm_FindById(b2, 0, 0, *(int *)(ev + 0xb8));
        v = gm_BuildingSkill(b2[4], 1, 1);
        f1 = (float)((double)v * 0.0 + 1.0);
        if (*(unsigned char *)(ev + 0xc8) >= 0x3c) {
            if (ebp != 0) {
                for (i = 0; i < 8; i++) {
                    if (*(int *)(ev + 0x8c + i * 4) != -1 && *(int *)(b2[4] + 0x41) != 2)
                        cm_SendRelations(*(int *)(ebp + 4), *(int *)(ev + 0x8c + i * 4), 2 - *(int *)(b2[4] + 0x41), 0);
                }
                *(float *)(ev + 0xc4) = (float)((gm_Unk50c980(*(unsigned short *)ebp, 8, ev + 0x8c) - 0.5) * 0.25);
            }
            *(char *)(ev + 0xc8) = 0;
        }
        n = gm_Unk509400(ev + 0x60, (unsigned)&ai_Time, 0);
        if (n > 0)
            *(unsigned char *)(ev + 0xc8) += n;
        f2 = f1 + *(float *)(ev + 0xc4);
        while (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) < 0) {
            n = gm_Unk509400(ev + 0x60, ev + 0x52, 0);
            v = (int)((float)n * 0.0f * f2);
            *(int *)(ev + 0xb0) = (int)((float)*(int *)(ev + 0xb0) - (float)v);
            if (*(int *)(ev + 0xb0) <= 0) {
                gm_FindById(b2, 0, 0, *(int *)(ev + 0xbc));
                g = gm_Query(*(int *)(b2[4] + 0x14), 0, 1, *(unsigned short *)(ev + 0xac));
                gm_FindById(b2, 0, 0, *(int *)(ev + 0xc0));
                if (g != 0) {
                    n = *(unsigned short *)(U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41 + 0x36);
                    if (n >= *(int *)(g + 0xe))
                        n = *(int *)(g + 0xe);
                    cm_SendExSellObject(*(int *)(ev + 0xc0), *(int *)(ev + 0xbc), n, *(unsigned short *)(ev + 0xac), U_I((unsigned)&ai_OwnPlayer), 0);
                    *(int *)(ev + 0xb0) += *(int *)(U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41 + 0x22);
                    *(float *)(ev + 0xb4) = (float)*(int *)(ev + 0xb0);
                }
                if (g == 0) {
                    txt_Format(text, 0x1bb7, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b2[4]);
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f1, *(int *)(b2[4] + 1), -1, 0, (unsigned)&full_data_628a28);
                    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
                        if (*(int *)(rec + 0x17c) == ev)
                            sv_MoveToGroup(b2[4], *(unsigned short *)rec, 0, 0);
                    }
                    he_Unk53e1f0((int *)ev);
                    return;
                }
            }
            *(HETIME *)(ev + 0x60) = *(HETIME *)(ebp != 0 ? ebp : ev + 0x52);
            gm_AddTime(ev + 0x52, 0, 0, 5);
        }
        return;
    case 2:
        if (*(int *)(ev + 0x14) > 0 || *(int *)(ev + 0x18) > 0) {
            *(int *)(ev + 0x70) = 1;
            return;
        }
        if (obj == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(obj, *(unsigned short *)rec, 0, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    case -2:
    case -1:
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, 0, *(unsigned short *)rec, 0);
        }
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
}
