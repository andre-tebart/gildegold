/* 0x4e1a80, an auction (the message m: +0x7c the building / place, +0x30 count, +0x34 the bidders): for the building kinds 0xb / 0xc / 0xd (forest
   "WALDSTUECK", quarry "STEINBRUCH", mine "MINE") the scene "Versteigerung.ed3" with "ub_STAFELEI" (texture set by the kind), the auctioneer
   "Auktionator" (dummy_AUKTIONATOR) and up to 8 persons on the places "dummy_RP_%02i" (the bidders' own models, else a random
   "buerger_MANN" / "buerger2_MANN" / "handwerker3_MANN"); the voices "_VERSTEIGERUNG_INTRO_%s", "_VERSTEIGERUNG_GEBOTE" (bids, texts 0x1aa2 ..
   0x1aa5: intro, the bid, the winner, nobody) and "_VERSTEIGERUNG_KOMMENTARE"; the winner pays (0x4c3c00 fields 0x69 / 0x6d) and the
   opinion of the others changes (0x4c4830). ROUGH: the bid loop (slots 0x92cb74, the bid steps) is only sketched */

void ai_Unk4e1a80(char *m)
{
    char buf[0x100];
    char who[0x40];
    int ids[8];
    int who_ok[8];
    char *b, *kind;
    int i, n, bid, best, round, pl, h, v, scene;

    b = (char *)gm_BuildQuery(1, 1, *(int *)(m + 0x7c));
    best = -1;
    scene = ai_Unk4e52f0(3);
    ai_Unk4e3620(0);
    if (b == 0)
        goto done;
    kind = (char *)ai_BTypes + *(unsigned char *)b * 0x24d;
    if (kind[0] == 0xb)
        sprintf(who, "%s", "WALDSTUECK");
    else if (kind[0] == 0xd)
        sprintf(who, "%s", "MINE");
    else if (kind[0] == 0xc)
        sprintf(who, "%s", "STEINBRUCH");
    else
        goto done;
    n = *(unsigned char *)(m + 0x30);
    for (i = 0; i < n; i++) {
        who_ok[i] = 0;
        ids[i] = *(int *)(m + 0x34 + i * 4);
        who_ok[i] = gm_FindChar(ids[i]) != 0 ? 1 : 0;
    }
    for (; i < 8; i++) {
        ids[i] = -1;
        who_ok[i] = 0;
    }
    ai_Unk4e3270(0, "VERSTEIGERUNG_INTRO.sbf");
    ai_Unk4e3270(0, "VERSTEIGERUNG_GEBOTE.sbf");
    ai_Unk4e3270(0, "VERSTEIGERUNG_KOMMENTARE.sbf");
    ai_Unk4e2e60("Versteigerung.ed3");
    if (d3_FindObject("ub_STAFELEI", 0, 0x40, 0) != 0) {
        if (kind[0] == 0xb)
            v = 0;
        else if (kind[0] == 0xc)
            v = 1;
        else
            v = 2;
        d3if_SelectTextureSet(v, 0, 1, 2);
    }
    /* the auctioneer */
    sprintf(buf, "%s", v == 0 ? "buerger3_DICKER" : v == 1 ? "wirt2_DICKER" : "buergerin2_FRAU");
    sprintf(buf, "%s", "W1");
    h = d3_FindObject("dummy_AUKTIONATOR", 0, 0x100, 0);
    if (h != 0)
        d3_PointToWorld(0, 0, h);
    pl = ai_Unk4e3230(0, 0);
    if (pl != 0)
        ch_PlaceAtObject(pl, h);
    sprintf(who + strlen(who), "%s", "Auktionator");
    /* the persons on the places dummy_RP_00 .. */
    for (i = 0; i < n; i++) {
        sprintf(buf, "dummy_RP_%02i", i);
        h = d3_FindObject(buf, 0, 0x100, 0);
        if (h != 0)
            d3_PointToWorld(0, 0, h);
        if (who_ok[i] != 0 && unk_Unk4fb390(ids[i]) != 0)
            sprintf(buf, "%s", (char *)unk_Unk4fb390(ids[i]) + 4);
        else if (ai_Unk4e52f0(3) == 1)
            strcpy(buf, "buerger_MANN");
        else if (ai_Unk4e52f0(3) == 2)
            strcpy(buf, "buerger2_MANN");
        else
            strcpy(buf, "handwerker3_MANN");
        pl = ai_Unk4e3230(0, 0);
        if (pl != 0)
            ch_PlaceAtObject(pl, h);
    }
    ai_Unk4e2c30("cutscenes\\auktion\\behavior_auktionator.esc", &ch_EmptyName);
    ai_Unk4e2c30("cutscenes\\auktion\\Enterfly_auktion.esc", &ch_EmptyName);
    sprintf(buf, "_VERSTEIGERUNG_INTRO_%s_00", who);
    ai_Unk4e32d0(0, -1, buf, 0, -7);
    sprintf(buf, "_VERSTEIGERUNG_INTRO_%s", who);
    ai_Unk4e32d0(0x1f4, ai_Unk4e52f0(3), buf, 0, 0);
    v = d2_OpenForm("cutscenes\\pacht_main", 0, 0);
    if (ai_Unk6f0d8c == 0)
        d2_SetViewEnabled(v, 0);
    ((void (*)(int, int))d2_PlaceMainWindow)(3, v);
    d2_SetForm(0, v);
    io_PrintText(0x1aa2, 0, 0);
    ai_Unk4e2c70(0);
    /* the bids: up to 6 rounds, the bidders raise by turns (the slots 0x92cb74 hold the bids) */
    for (round = 0; round <= 5; round++) {
        if (ai_Unk4e2c70(0) != 0)
            break;
        ai_Unk4e3830(m, 0);
        if (n > 6 && ai_Unk4e52f0(3) != 0) {
            ai_Unk4e52f0(0x200);
            ai_Unk4e52f0(6, 0);
            ai_Unk4e32d0(ai_Unk4e52f0(4), "_VERSTEIGERUNG_KOMMENTARE", 0, 0);
        }
        for (i = 0; i < n; i++)
            if (*(int *)(ai_Unk92caf0 + i * 0x18 + 0x84) != 0 && best < *(int *)(ai_Unk92caf0 + i * 0x18 + 0x84))
                best = i;
        if (best < 2 || n <= 6)
            ai_Unk4e32d0(ai_Unk4e52f0(2), "_VERSTEIGERUNG_GEBOTE", 0, 0);
        else
            ai_Unk4e32d0(ai_Unk4e52f0(4), "_VERSTEIGERUNG_GEBOTE", 0, 0);
        d2_SetForm(0, v);
        io_PrintText(0x1aa3, 0, 5, 0, best, 0);
        ai_Unk4e3420(5);
    }
    if (best > -1) {
        bid = ai_Time;
        ai_Unk4e32d0(6, "_VERSTEIGERUNG_GEBOTE", 0, 0);
        d2_SetForm(0, v);
        io_PrintText(0x1aa4, bid, 0);
        if (ids[best] == ai_CharInts[ai_PlayerId][0]) {
            cm_BeginFieldBlock(0, best);
            cm_AddFieldDelta(0, 0x69, 1, 4);
            cm_AddFieldDelta(0, 0x6d, 1, 4);
            cm_SendFieldDeltas();
            cm_SendOwnership(ai_PlayerCharC, 0, 0);
            gm_FirstBuilding(1);
            gm_Query(0x115, 0, 6, 2, 0);
            cm_SendMoveGoodsUnchecked(util_Random(5) % 5, 0, 0, 0);
        }
    } else {
        ai_Unk4e32d0(7, "_VERSTEIGERUNG_GEBOTE", 0, 0);
        d2_SetForm(0, v);
        io_PrintText(0x1aa5, 0);
        if (ids[0] == ai_CharInts[ai_PlayerId][0])
            cm_SendOwnership(ai_PlayerCharC, 0, 0);
    }
    ai_Unk4e32d0(0x1f4, 8, "_VERSTEIGERUNG_GEBOTE", 0, 0);
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\auktion\\Auktion_Outro.esc", &ch_EmptyName));
    d2_CloseView(v);
    ai_Unk4e3060();
    ai_Unk4e2c70(ai_Unk4e2d90(0));
    for (i = 0; i < n; i++)
        if (ai_Unk4e3230(0, 0) != 0)
            ai_Unk4e3250(0);
    ai_Unk4e30c0();
    snd_ClearQueue();
    ai_Unk4e3290(0);
done:
    ai_Unk4e3640();
}
