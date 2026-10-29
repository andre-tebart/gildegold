/* 0x571000, the run of the event 0x8c (a work on the building +0x10 by up to four characters +0x8c ..): outside the night window (the hour 0x4fe350 against the minute curves 0x617840 / 0x617830) a state -1 / -2 works; the player +8 0xffff or +0xc -1 releases all and frees. The four characters counted (cnt; alive without the kinds 0x1b / 0x1c with +0x1b4 bit 0), none -> state -1; the building 0x501190 (1, 1, +0x10) and the work group 0x4ffe90 (+0x5d, 2, 0, 0x2a, 6), missing -> state -1. State 0: the elapsed 0x4fe280 (+0xb8) and the value 0x5092e0 (group, 0x12 / 0x1e, 1) * 0x5eb7f0 (minus 0.5 for kind 3 without the +0x5b bit 0x20), (that + 1.0f) * elapsed * 0x5eec50 added alive times to +0xb0; +0xb4 -= alive; the start clock +0xb8 = now; due now + 5 hours; +0xb4 < 0 -> state -1. With a state -1 / -2: n = (int)+0xb0 (at least 1), -1 when 0x5241b0 (ev) <= 0 (the -1 state only); with a group: 0x50a810, 0x50ab80, 0x4c4880 and the texts 0x421e / 0x4222 / 0x4128 / 0x412c, 0x4223 / 0x4224 / 0x412d / 0x412e and 0x628f04 / 0x628ee8, the message 0x53dee0. Finally the four released (0x524260) and freed. ROUGH: the text building around the 0x53dee0 calls is a skeleton */

void he_Unk571000(int ev)
{
    char text[0x200];
    char name[0x200];
    int hour, minute, i, n, cnt = 0, alive = 0, e;
    float x;
    char *per, *bld, *grp, *r;

    hour = gm_ClockSeason(&ai_Time);
    minute = *(unsigned short *)((char *)&ai_Time + 4);
    if ((float)minute < *(float *)((unsigned)&ai_Unk617840 + hour * 4) || (float)minute < *(float *)((unsigned)&ai_Unk617830 + hour * 4)) {
        if (HM_I(ev, 0x70) != -1 && HM_I(ev, 0x70) != -2) {
            gm_AddTime(ev + 0x52, 0, 5, 0);
            return;
        }
    }
    if (*(unsigned short *)(ev + 8) == 0xffff || HM_I(ev, 0xc) == -1)
        goto free_all;
    for (i = 0; i < 4; i++) {
        r = (char *)gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (r != 0) {
            cnt++;
            alive++;
            if ((HM_B(r, 0x165) == 0x1b || HM_B(r, 0x165) == 0x1c) && (HM_B(r, 0x1b4) & 1))
                alive--;
        }
    }
    if (cnt == 0)
        HM_I(ev, 0x70) = -1;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    grp = 0;
    if (bld != 0) {
        grp = (char *)gm_Query(HM_I(bld, 0x5d), 2, 0, 0x2a, 6);
        if (grp == 0)
            HM_I(ev, 0x70) = -1;
    } else
        HM_I(ev, 0x70) = -1;
    if (HM_I(ev, 0x70) == 0) {
        e = gm_MinutesBetween(ev + 0xb8, &ai_Time);
        if (HM_B(bld, 0) == 0x1d)
            x = (float)gm_BuildingSkill(grp, 0x12, 1) * (0.00999999978f);
        else {
            x = (float)gm_BuildingSkill(grp, 0x1e, 1) * (0.00999999978f);
            if (e == 3 && !(HM_B(grp, 0x5b) & 0x20))
                x -= 0.5f;
        }
        x = (x + 1.0f) * e * *(float *)((unsigned)&ai_Unk5eeab8 + 0x198);
        for (i = alive; i > 0; i--)
            *(float *)(ev + 0xb0) += x;
        HM_I(ev, 0xb4) -= alive;
        *(HETIME *)(ev + 0xb8) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        if (HM_I(ev, 0xb4) < 0)
            HM_I(ev, 0x70) = -1;
    }
    n = HM_I(ev, 0x70);
    if (n != -1 && n != -2)
        return;
    n = (int)*(float *)(ev + 0xb0);
    if (n <= 1)
        n = 1;
    if (HM_I(ev, 0x70) == -1 && sv_UsedGroupSlots(ev) <= 0)
        n = -1;
    if (cnt != 0 && grp != 0 && n > 0) {
        per = (char *)gm_FindCharacterByMaster(grp);
        r = (char *)gm_FreeCapacity(grp, *(unsigned short *)(ev + 0xac), n);
        if (r != 0) {
            cm_SendExSellObject(*(int *)(grp + 2), -1, *(unsigned short *)(ev + 0xac), r, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            if (per[0] == 0x1f) {
                txt_Format(text, 0x421e, 0x4222, 0x22c);
                txt_Format(name, 0x4223, 0x4224, r, HM_I(ev, 0xac) * 2 + 0x9a8);
            } else {
                txt_Format(text, 0x4128, 0x412c, 0x228);
                txt_Format(name, 0x412d, 0x412e, r, HM_I(ev, 0xac) * 2 + 0x9a8);
            }
            if (HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 6 || HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 7) {
                if (per == 0 || !(HM_B(per, 0x1b4) & 1))
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, -1, HM_I(ev, 0xac), 0x5ec, name, 0);
            }
        }
    }
free_all:
    for (i = 0; i < 4; i++) {
        r = (char *)gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (r != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)r, 0);
    }
    he_Unk53e1f0((int *)ev);
}
