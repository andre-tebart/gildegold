/* 0x571640, the run of the event 0x8d (a work on the building +0x10 by the character +0x8c): outside the night window (the hour 0x4fe350 against the minute curves 0x617840 / 0x617830) a state -1 / -2 works; the player +8 0xffff or +0xc -1 frees. The character 0x505df0 (+0x8c) missing -> state -1, else registered (0x524260); the building 0x501190 (1, 1, +0x10) missing -> state -1; the work 0x4ef570 (building +0x65, +0xb0, +0xb4) missing -> state -1. State 0: the elapsed 0x4fe280 (+0xbc), the value (4 - x) * y * 0x5eb91c (times 0x5eb79c for the kinds 0x1b / 0x1c with +0x1b4 bit 0) added to +0xac, due now + 5 hours, +0xb8 -= elapsed, the start clock +0xbc = now, +0xb8 <= 0 -> state -1; n = (int)+0xac + HM_B (rec, 0xc), n >= 0xff -> state -1. With a state -1 / -2: n = (int)+0xac, 0x4f0c20 (the character, +0xb0, +0xb4, n), the text 0x411e / 0x4126 and the message 0x53dee0 (when the player is kind 6 / 7 and the character no +0x1b4 bit 0), the release 0x524260, the free. ROUGH */

void he_Unk571640(int ev)
{
    char text[0x200];
    char name[0x200];
    int hour, minute, n, e;
    float x;
    char *per, *bld, *r;

    hour = gm_ClockSeason(&ai_Time);
    minute = *(unsigned short *)((char *)&ai_Time + 4);
    if ((float)minute < *(float *)((unsigned)&ai_Unk617840 + hour * 4) || (float)minute < *(float *)((unsigned)&ai_Unk617830 + hour * 4)) {
        if (HM_I(ev, 0x70) != -1 && HM_I(ev, 0x70) != -2) {
            *(HETIME *)(ev + 0xbc) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            return;
        }
    }
    if (*(unsigned short *)(ev + 8) == 0xffff || HM_I(ev, 0xc) == -1)
        goto free_one;
    per = (char *)gm_FindChar(HM_I(ev, 0x8c));
    if (per == 0)
        HM_I(ev, 0x70) = -1;
    else
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0)
        HM_I(ev, 0x70) = -1;
    r = (char *)unk_Unk4ef570(HM_I(bld, 0x65), HM_I(ev, 0xb0), HM_B(ev, 0xb4));
    if (r == 0)
        HM_I(ev, 0x70) = -1;
    if (HM_I(ev, 0x70) == 0) {
        e = gm_MinutesBetween(ev + 0xbc, &ai_Time);
        x = (float)(4 - HM_I(ev, 0xb0)) * (float)HM_I(ev, 0xb4) * (0.100000001f);
        if ((HM_B(per, 0x165) == 0x1b || HM_B(per, 0x165) == 0x1c) && (HM_B(per, 0x1b4) & 1))
            x *= 0.25f;
        *(float *)(ev + 0xac) += x;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        HM_I(ev, 0xb8) -= e;
        *(HETIME *)(ev + 0xbc) = ai_Time;
        if (HM_I(ev, 0xb8) <= 0)
            HM_I(ev, 0x70) = -1;
        n = (int)*(float *)(ev + 0xac) + HM_B(r, 0xc);
        if (n >= 0xff)
            HM_I(ev, 0x70) = -1;
    }
    n = HM_I(ev, 0x70);
    if (n < -2 || n >= 0)
        return;
    n = (int)*(float *)(ev + 0xac);
    if (per != 0 && n > 0)
        unk_Unk4f0c20(per, HM_I(ev, 0xb0), HM_B(ev, 0xb4), n);
    if (per != 0) {
        r = (char *)gm_FindCharacterByMaster(per);
        txt_Format(name, 0x411e, 0x4126, *(unsigned short *)per);
        if ((HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 6 || HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 7) &&
            (r == 0 || !(HM_B(r, 0x1b4) & 1)))
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, name, 0, -1, *(int *)(per + 1), 0x5ec, text, 0);
    }
    if (per != 0)
        sv_MoveToGroup(0, 0, *(unsigned short *)per, 0);
free_one:
    he_Unk53e1f0((int *)ev);
}
