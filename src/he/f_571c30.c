/* 0x571c30, the run of the event 0x8f: only the states -2 .. 0 (below -> nothing, -1 / -2 -> free). State 0: the character +0xac missing, the byte +0x80 [+0xb0] >= 1000.0f (0x5eceb4) or +0xb8 not due (0x4fe210) -> the release part; the building 0x501190 (1, 1, +0x10) missing -> free; the value 0x5092e0 (+0xc) and 0x509400 (+0x60, now, building) -> the byte +0x80 [+0xb0] = a share of it (the constants 0x5eec10, 0x5ee928, 0x5ece60, 0x5eb8a8); 0x4c3ba0 (character, +4), 0x4c3d00 (1, 1, the byte record, &out), 0x4c49a0, the due clock +0x52 and +0x60 = now, due now + 0x1e hours. The release part: the character 0x505df0 (+0xac) and the building 0x501190 (1, 1, +0x10) -> free when missing; the leader 0x50a810 (building) with +0x1b4 bit 0 -> release; the group 0x4ffe90 (+0x5d, 1, 1, +0xb4) or 0x501ce0 (building) -> the text 0x1bc8 (and the strings 0x631090 / 0x6274c8) to the player, the message 0x53dee0; the character released (0x524260) and the event freed. ROUGH */

void he_Unk571c30(int ev)
{
    char text[0x200];
    float f;
    int state = HM_I(ev, 0x70);
    int v, n;
    char *per, *bld, *r;
    int grp;

    if (state < -2)
        return;
    if (state < 0)
        goto free_ev;
    if (state != 0)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xac));
    if (per == 0)
        goto release;
    v = HM_B(per + HM_I(ev, 0xb0), 0x80);
    if ((float)v >= (252.0f))
        goto release;
    if (gm_CompareTime(&ai_Time, ev + 0xb8) >= 0)
        goto release;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0)
        goto free_ev;
    n = gm_BuildingSkill(bld, 0xc, 1);
    f = (float)n;
    v = gm_Unk509400(ev + 0x60, &ai_Time, bld);
    HM_B(per + HM_I(ev, 0xb0), 0x80) = (unsigned char)(int)((double)v * *(double *)((unsigned)&ai_Unk5eeab8 + 0x158) * *(double *)((unsigned)&ai_Unk5ee160 + 0x7c8) * ((double)f * *(double *)(unsigned)&ai_Unk5ece60 + *(double *)(unsigned)&full_rdata_5eb8a8));
    cm_BeginFieldBlock(per, *(int *)(per + 4));
    cm_AddFieldValues(1, 1, per + (*(unsigned short *)(ev + 0xb0) - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x80, &n);
    cm_SendFieldDeltas();
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    return;
release:
    per = (char *)gm_FindChar(HM_I(ev, 0xac));
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0 || per == 0)
        goto free_ev;
    r = (char *)gm_FindCharacterByMaster(bld);
    if (r != 0 && (HM_B(r, 0x1b4) & 1))
        goto released;
    grp = gm_Query(HM_I(bld, 0x5d), 1, 1, HM_I(ev, 0xb4));
    if (grp == 0) {
        grp = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(bld);
        if (grp == 0)
            goto released;
    }
    txt_Format(text, 0x1bc8, *(unsigned short *)per, bld);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), HM_I(ev, 0x10), text, 0, 0x5f2, *(int *)(bld + 1), *(int *)(grp + 2), (char *)(unsigned)&ch_EmptyName, (char *)((unsigned)&ai_Unk625820 + 0x1ca8));
released:
    sv_MoveToGroup(bld, 0, *(unsigned short *)per, 0);
    he_Unk53e1f0((int *)ev);
    return;
free_ev:
    he_Unk53e1f0((int *)ev);
}
