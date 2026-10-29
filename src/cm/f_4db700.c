/* 0x4db700 */

/* 0x4db700 .. 0x4dd95d (one function; Ghidra's 0x4dc7c0 lies inside it), the trial scene (the run function of the scene kind 2, called with the party m) of a session request m (+0x30 count, +0x34 .. +0x48 the characters: accused, accuser, judge, two
   assessors ..): "Gericht.ed3" / "ADDON\\PROZESS_NEU.sbf", the scenes "cutscenes\\prozess\\prozess-*.esc" (create, judge, assessors, intro,
   accuser, accused), the texts 0x1491 .. 0x1557 through 0x515960 and the voices "_PROZESS_*" (0x4e32d0 / 0x4e32b0); the accusation, the
   charge (witnesses), the question of guilt, the torture sub-scenes "cutscenes\\prozess\\folter*" (the instrument names 0x60dad0 ..
   0x60db84 are the .esc names), the vote ("prozess-abstimmen-1 .. 3"), the verdict (guilty / not guilty, penalties 0x4f4420 / 0x4f48f0 /
   0x4f4460) and the cleanup of all sound slots. The stack frame is 0x760 bytes. ROUGH: the structure and the calls (read from the call
   digest, arguments in call order but mostly 0), not the arguments of 0x4f2720 / 0x4f3580 / 0x4f3b40 and the mood floats (0x3fb33333 ..) */
char ai_Unk60de4c = 50;  /* 0x60de4c, scene slots of the trial */
char ai_Unk60dd68 = 49;  /* 0x60dd68 */
char ai_Unk60dd24 = 48;  /* 0x60dd24 */
/* ai_Unk92caf0: view of ai_Unk92cae0 (tools/data_views.py) */
#define G26_BOX(sw) ai_Unk4e2bd0(ai_Unk60d368, (sw)); d2_SetForm(ai_Unk60d368, 0)
#define G26_WAITV(h) if ((h) != 0) { while (sb_IsPlaying(h) != 0) ai_Unk4e3420(1); } ai_Unk4e3420(1)

void ai_Unk4db700(char *m)
{
    int a, b, c;
    int ids[6];
    char *ch[6];
    char names[0x200];
    int i, n, h, v, guilty, mode, scene;

    n = *(unsigned char *)(m + 0x30);
    for (i = 0; i < 6; i++)
        ids[i] = *(int *)(m + 0x34 + i * 4);
    a = ids[0];
    b = ids[1];
    c = ids[2];
    gv_GetTableRecord(a, (int *)names);     /* ROUGH: the original pushes (edi, a local buffer) */
    G26_BOX(0);
    ai_Unk4e2e60("Gericht.ed3");
    for (i = 0; i < 5; i++) {
        ch[i] = (char *)gm_FindChar(ids[i]);
        if (ch[i] == 0)
            goto done;
    }
    ch[5] = (char *)gm_FindChar(ids[5]);
    if (ch[5] == 0 && (unsigned char)m[0x30] > 2 && *(int *)(m + 0x48) != -1)
        goto done;
    if (ai_Unk6f0d8c == 0)
        msx_PlayAmbient(0x40, "Athmos\\Athmo_sitzung.mp3");
    scene = ai_Unk4e52f0(2);
    ai_Unk4e3620("cd2\\ImKerker.mp3");
    ai_Unk4e3270(0, "ADDON\\PROZESS_NEU.sbf");
    ai_Unk4947a0(a, b);
    sprintf(names, "%s ", (char *)unk_Unk4fb390(*(unsigned short *)ch[0]) + 4);
    for (i = 1; i < 4; i++)
        sprintf(names + strlen(names), "%s ", (char *)unk_Unk4fb390(*(unsigned short *)ch[i]) + 4);
    sprintf(names + strlen(names), "%s", (char *)unk_Unk4fb390(*(unsigned short *)ch[4]) + 4);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-create2.esc", names);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter.esc", &ai_Unk60de4c);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer1.esc", &ai_Unk60de4c);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer2.esc", &ai_Unk60de4c);
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\prozess\\prozess-intro.esc", &ch_EmptyName));

    /* the judge opens the session */
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
    G26_BOX(1);
    io_PrintText(0x1491);
    h = ai_Unk4e32b0(5, "_PROZESS_NEU", 0, 0);
    G26_WAITV(h);
    G26_BOX(0);
    io_PrintText(0x1492);
    ai_Unk4e3420(4);

    /* the accuser states the charge */
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-klaeger.esc", &ai_Unk60dd68);
    G26_BOX(0);
    io_PrintText(0x1499);
    h = ai_Unk4e32b0(0, "_PROZESS_NEU", 0, 0);
    G26_WAITV(h);
    ai_Unk4e2c70(0);

    /* the witnesses */
    for (i = 0; i < n; i++) {
        if (ids[i] == -1)
            continue;
        gv_GetTableRecord(ids[i], i);
        v = unk_Unk4f3580(ids[i], i);
        if (v == -1)
            continue;
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-klaeger.esc", &ai_Unk60dd68);
        G26_BOX(0);
        io_PrintText(0x14b0);
        h = ai_Unk4e32b0(i, "_PROZESS_GESETZ", 0, 0);
        G26_WAITV(h);
        ai_Unk4e52f0(0x1e);
        ai_Unk4e2c70(0);
    }
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
    io_PrintText(0x14ba);
    h = ai_Unk4e32b0(0, "_PROZESS_NEU", 0, 0);
    G26_WAITV(h);
    G26_BOX(0);
    io_PrintText("$C");

    /* the accused answers: guilty / not guilty */
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-angeklagter.esc", &ai_Unk60dd24);
    ai_Unk4e2bd0(ai_Unk60d368, 0);
    ai_Unk4e3830(m, 0);
    for (i = 0; i < n && *(int *)(ai_Unk92caf0 + i * 0x18) != ids[0]; i++)
        ;
    guilty = *(int *)(ai_Unk92caf0 + i * 0x18 + 0x84);
    mode = *(int *)(ai_Unk92caf0 + i * 0x18 + 0x88);
    G26_BOX(1);
    io_PrintText("$C");
    if (guilty == 1) {
        gm_Liking(guilty, mode, 0);
        ai_Unk4e3270(0, "PROZESS_3_SCHULDIG.sbf");
        ai_Unk4e32d0(0, 0, "_PROZESS_3_SCHULDIG", 0, 0);
        io_PrintText(0x14bd, 0, 0x14be);
    } else {
        ai_Unk4e3270(0, "PROZESS_3_NICHT_SCHULDIG.sbf");
        ai_Unk4e32d0(0, 0, "_PROZESS_3_NICHT_SCHULDIG", 0, 0);
        io_PrintText(0x14bd, 0, 0x14c4);
    }
    ai_Unk4e3420(2);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer1.esc", &ai_Unk60dd68);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer2.esc", &ai_Unk60dd68);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ai_Unk60dd68);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter.esc", &ch_EmptyName);

    /* the torture (the instrument names 0x60dad0 .., "cutscenes\\prozess\\folter\\<name>.esc") */
    if (ai_Unk4e52f0(2) == 1) {
        ai_Unk494830(a, b);
        G26_BOX(1);
        io_PrintText("$C", 0x14ca);
        ai_Unk4e3420(4);
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-folter-1.esc", &ch_EmptyName);
        G26_BOX(0);
        ai_Unk4e3830(m, 0);
        ai_Unk4e2c70(0);
        for (i = 0; i < n && *(int *)(ai_Unk92caf0 + i * 0x18) != ids[0]; i++)
            ;
        G26_BOX(1);
        io_PrintText("$C");
        if (*(int *)(ai_Unk92caf0 + i * 0x18 + 0x88) == 1) {
            ai_Unk4e52f0(6);
            ai_Unk4e3270(0, "PROZESS_4_FOLTER_1.sbf");
            ai_Unk4e3270(0, "PROZESS_4_FOLTER_2.sbf");
            ai_Unk4e32d0(0x32, 0, "_PROZESS_4_FOLTER_1", 0, 0);
            ai_Unk4e32d0(0x12c, 0, "_PROZESS_4_FOLTER_2", 0, 0);
            io_PrintText(0x14d9);
            ai_Unk4e3420(1);
            ai_Unk4e2c30("cutscenes\\prozess\\prozess-folter-3.esc", &ch_EmptyName);
        } else {
            ai_Unk4e2c30("cutscenes\\prozess\\prozess-folter-2.esc", &ch_EmptyName);
            io_PrintText(0x14d8);
        }
        ai_Unk4e3420(4);
        ai_Unk4e2e60("ob_GEFAENGNIS_FOLTERKAMMER.ed3");
        ai_Unk4e2c30("cutscenes\\prozess\\folter-create.esc", &ch_EmptyName);
        G26_BOX(1);
        io_PrintText(0x14e4);
        h = ai_Unk4e32b0(4, "_PROZESS_NEU", 0, 0);
        G26_WAITV(h);
        ai_Unk4e3420(5);
        ai_Unk4e52f0(5);
        io_PrintText(0x14ec);
        ai_Unk4e3420(5);
        ai_Unk494300(0);
        ai_Unk4e2c70(0);
        ai_Unk4e32d0(0x1f4, 0, "_PROZESS_5_FOLTER2_UNSCHULDSBETEUERUNG", 0, 0);
        ai_Unk4e32d0(0, 0, "_PROZESS_5_FOLTER2_GESTEHEN", 0, 0);
        ai_Unk4e32d0(0x1f4, 0, "_PROZESS_5_FOLTER2_WEITER", 0, 0);
        io_PrintText(0x1510);
        ai_Unk4e3420(4);
        ai_Unk4e2c30("cutscenes\\prozess\\folterexit.esc", &ch_EmptyName);
        if (ai_Unk6f0d8c == 0)
            d3if_SelectScene(scene, 0);
        ai_Unk4e3270(0, "PROZESS_6_FOLTERERGEBNIS.sbf");
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
        ai_Unk4e32d0(0, "_PROZESS_6_FOLTERERGEBNIS", 0, 0);
        io_PrintText(0x1520, 0, 0x1522);
        ai_Unk4e3420(4);
    }
    ai_Unk4e3270(0, "PROZESS_6_ANKUENDIGUNG.sbf");
    ai_Unk4e32d0(0, 0, "_PROZESS_6_ANKUENDIGUNG", 0, 0);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
    io_PrintText(0x1523);
    ai_Unk4e3420(4);

    /* the vote */
    ai_Unk4e3270(0, "PROZESS_6_ABSTIMMUNG_ERGEBNISSE.sbf");
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-3.esc", &ch_EmptyName);
    G26_BOX(1);
    io_PrintText(0x1528);
    ai_Unk4e3420(2);
    ai_Unk4e52f0(100, 3);
    ai_Unk4e32d0(0, "_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", 0, 0);
    ai_Unk4e3420(2);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-1.esc", &ch_EmptyName);
    ai_Unk4e3420(2);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-2.esc", &ch_EmptyName);
    ai_Unk4e3420(2);

    /* the verdict */
    ai_Unk4e3270(0, "PROZESS_8.sbf");
    if (guilty < 2) {
        ai_Unk4e3270(0, "PROZESS_7_SCHULDIG.sbf");
        ai_Unk4e3270(0, "PROZESS_7_STRAFEN.sbf");
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-3.esc", &ch_EmptyName);
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
        ai_Unk4e2bd0(ai_Unk60d368, 0);
        ai_Unk4e3830(m, 0);
        if (mode >= 4 && ch[0][2] == 6 && ai_Unk4e3290(0) != 0)
            ai_Unk4b44e0(0x13, ch[0]);
        G26_BOX(1);
        io_PrintText(0x152f);
        io_PrintText(0x1530);
        ai_Unk4e32d0(0x64, 0, "_PROZESS_7_SCHULDIG", 0, 0);
        ai_Unk4e32d0(0xfa, 2, "_PROZESS_7_SCHULDIG", 0, 0);
        ai_Unk4e3420(3);
        unk_Unk4f4420(mode, 0);
        unk_Unk4f48f0(0, mode, 0);
        ai_Unk4e32d0(0x64, mode, "_PROZESS_7_STRAFEN", 0, 0);
        ai_Unk4e32d0(0xc8, 0, "_PROZESS_8", 0, 0);
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0])
            unk_Unk4f4460(0, 0, 0, mode, 0);
        io_PrintText(0x1531);
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
        }
        ai_Unk4e3420(6);
        io_PrintText("$C", 0x1557);
        ai_Unk4e3420(4);
    } else {
        ai_Unk4e3270(0, "PROZESS_7_NICHT_SCHULDIG.sbf");
        ai_Unk4e32d0(0x64, 0, "_PROZESS_7_NICHT_SCHULDIG", 0, 0);
        ai_Unk4e32d0(0xc8, 0, "_PROZESS_8", 0, 0);
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-3.esc", &ch_EmptyName);
        G26_BOX(1);
        io_PrintText("$C", 0x152e);
        ai_Unk4e3420(4);
        io_PrintText("$C", 0x1557);
        ai_Unk4e3420(4);
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
        }
    }
    /* the player's reputation: opinion changes of +-26 (0x4c4b00) when 0x4fca40 / 0x5ba6b0 (0x22e) allow it */
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        if (gm_AddHealth(0, 0) != 0 && ai_InventoryHas(0x22e, 0) != 0) {
            cm_SendRelations(1, -26, 0, 0);
            cm_SendRelations(1, 26, 0, 0);
            cm_SendRelations(1, -26, 0, 0);
            cm_SendRelations(1, 26, 0, 0);
            cm_SendRelations(1, -26, 0, 0);
            cm_SendRelations(1, 26, 0, 0);
        }
    }
    ai_Unk4e3060();
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\prozess\\prozess-ende.esc", &ch_EmptyName));
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\prozess\\prozess-exit.esc", &ch_EmptyName));
done:
    if (ai_Unk6f0d8c == 0)
        msx_StopAmbient(0);
    ai_Unk4e3640();
    snd_ClearQueue();
    /* the sound slots made by 0x4e3270 are freed one by one (0x4e3290) */
    ai_Unk4e3290(0);
}
