/* 0x4d8e20, plays one council session ("sitzung_*" cutscenes; a: the 0x114-byte party record, +0x30 count, +0x34 ids, copied to the
   stack and back): for each of the 6 kinds (6 .. 1: the six sitting groups, "Sitzung_Wohnsitz" .. "Sitzung_zunft" .ed3, "sitzungen_flug_*"
   .esc) it collects the participants (0x4e6fc0), shows the heads of candidates (kind 2) and voters (kind 3) in the window 0x6ed938,
   and walks the vote: the votes per participant, the winner (a tie is drawn with 0x4e52f0), the texts 0x12c4 .. 0x12e5, then the
   dismissal ("_ABSETZEN_*") for the office of the player. ROUGH draft from the disassembly: the control flow of the two vote loops
   and the exact argument orders are not checked. */

void ai_Unk4d8e20(char *a)
{
    char flug[6][0x20] = {"sitzungen_flug_wohnsitz.esc", "sitzungen_flug_kirche.esc", "sitzungen_flug_rat.esc", "sitzungen_flug_diebe.esc",
                          "sitzungen_flug_wache.esc", "sitzungen_flug_zunft.esc"};
    char ed3[6][0x30] = {"Sitzung_Wohnsitz", "Sitzung_Kirche.ed3", "Sitzung_Rat.ed3", "Sitzung_Diebe.ed3", "Sitzung_Wache.ed3", "Sitzung_zunft.ed3"};
    int kinds[6] = {6, 5, 4, 3, 2, 1};
    int copy[0x45];
    G22ENT ent[16];
    char name[0x100];
    int cand[8], votes[8], voted[8];
    int *p;
    int ids[8], idx[8];
    int kind, k, n, nc, nv, nid, i, j, w, tie, best, win, win2, view1, view2, view3, track, ob, cur;
    int sound;

    memcpy(copy, a, 0x114);
    track = ai_Unk4e3270((unsigned)&full_data_60d5b4, 0);
    cur = ai_Unk4e3270((unsigned)&full_data_60d5a4, 0);
    ai_Unk4e3620(0);
    nid = 0;
    for (i = 0; i < (unsigned char)a[0x30]; i++) {
        p = (int *)gm_FindChar(*(int *)(a + 0x34 + i * 4));
        if (p != 0)
            ids[nid++] = (int)p;
    }
    for (i = nid; i < 8; i++)
        ids[i] = 0;
    for (k = 0; k < 6; k++) {
        kind = kinds[k];
        n = amt_GetColumnOffices(kind, 6, ent);
        nc = 0;
        for (i = 0; i < n; i++) {
            p = (int *)gm_FindChar(ent[i].id);
            if (p != 0 && *((char *)p + 0x1b1) == 0) {
                idx[nc] = i;
                cand[nc++] = (int)p;
            }
        }
        sv_Unk52fd80(&ch_EmptyName);
        if (amt_IsColumnDecided(kind, 0) == 0) {
            if (ai_CharInts[ai_PlayerId * 0x21c / 0x21c * 0 + ai_PlayerId][0] == *(int *)(a + 0xc))
                amt_ElectOffices(kind, n, ent);
            goto next;
        }
        for (i = n; i < 6; i++)
            cand[i] = 0;
        for (i = 0; i < nc; i++) {
            *(int *)(a + 0x34 + i * 4) = *(int *)(cand[i] + 4);
        }
        *(unsigned char *)(a + 0x30) = (unsigned char)nc;
        sv_Unk52fd80(&ch_EmptyName);
        view1 = d2_OpenForm(0, 0, "cutscenes\\sitzung_main");
        d2_SetViewEnabled(view1, 0);
        d2_PlaceMainWindow(view1, 3);
        nv = 0;
        for (i = 0; i < n; i++) {
            if (ent[i].kind == 2)
                nv++;
        }
        if (nv != 0) {
            ai_Unk4e2e60(ed3[k]);
            sound = msx_PlayAmbient("Athmos\\Athmo_sitzung.mp3", 0x40);
            for (i = 0; i < nc; i++) {
                if (unk_Unk4fb390(*(unsigned short *)cand[i]) != 0)
                    sprintf(name, "%i %s", idx[i], unk_Unk4fb390(*(unsigned short *)cand[i]) + 4);
                else
                    sprintf(name, "%i stallbursche_MANN", idx[i]);
                ai_Unk4e2c30("cutscenes\\sitzungen\\sitzungen_create.esc", name);
            }
            for (i = 0; i < nc; i++) {
                sprintf(name, "%i", idx[i]);
                ai_Unk4e2c30("cutscenes\\sitzungen\\sitzungen_create_2.esc", name);
            }
            txt_Format(name, "cutscenes\\sitzungen\\%s", flug[k]);
            ai_Unk4e2c70(ai_Unk4e2c30(name, &ch_EmptyName));
            sv_Unk52fd80(&ch_EmptyName);
        }
        for (j = 0; j < n; j++) {
            if (nv == 0)
                break;
            if (ent[j].kind != 2 || ent[j].flag == 0)
                continue;
            nv--;
            /* the head of a candidate: the views "sitzung_bottom" / "sitzung_top_absetzen", the number of supporters (0x4e75c0) in a row
               of heads (0x5be1d0 / 0x5be3b0), the texts 0x12c4 / 0x12c5 and the pause 0x9c4 */
            view2 = d2_OpenForm(0, 0, "cutscenes\\sitzung_bottom");
            d2_PlaceMainWindow(view2, 3);
            view3 = d2_OpenForm(0, 0, "cutscenes\\sitzung_top_absetzen");
            d2_PlaceMainWindow(view3, 3);
            d2_SetForm(view2, 1);
            ai_Unk5be1d0(ent);
            ai_Unk5be3b0(0x28, 0, d2_LastWindow, 0x54, ent, 1);
            d2_SetForm(view2, 2);
            ai_Unk4d8cb0(d2_LastWindow, 1, 0);
            ai_Unk4e32d0(-8, ids[0], "_INTRO", nc - 1, 0x32);
            io_PrintText(0x12c4);
            io_PrintText(0x12c5, *(unsigned short *)ent[j].id, j + 0x12bb, j + 0x12a9);
            ai_Unk4e3570(0x9c4);
            /* each voter shows its head and votes (texts 0x12c7 .. 0x12d1; kinds 0 / 1 / 2: yes / no / abstain, picked with 0x4e52f0),
               4d8cb0(.., 0, 0 / 1 / 2) adds a picture to the stack of its answer */
            win = -1;
            best = -1;
            tie = 0;
            for (i = 0; i < nc; i++) {
                if (votes[i] > best) {
                    tie = 0;
                    best = votes[i];
                    win = i;
                } else if (votes[i] == best)
                    tie = 1;
            }
            if (tie != 0 && win != -1)
                win = ai_Unk4e52f0(nc);
            if (win != -1) {
                ai_Unk4e32d0(-8, ids[0], "_ERGEBNIS", 100, 3);
                io_PrintText(0x12e1);
                ai_Unk4e3570(0x3e8);
                cm_SendRelations(ent[j].id, ent[win].id, 0, 1);
            }
            d2_SetForm(view1, 0);
            io_PrintText((unsigned)"$C");
            d2_SetViewEnabled(view1, 0);
            d2_CloseView(view3);
            d2_CloseView(view2);
            ai_Unk4e3570(0x3e8);
            sv_Unk52fd80(&ch_EmptyName);
        }
        if (nv != 0) {
            ai_Unk4e3060();
            ai_Unk4e3570(0x3e8);
            ai_Unk4e2d40(ai_Unk4e2c30("cutscenes\\sitzungen\\sitzungen_exit.esc", &ch_EmptyName));
            ai_Unk4e30c0();
        }
next:
        d2_CloseView(view1);
    }
    if (ai_CharInts[ai_PlayerId][0] == *(int *)(a + 0xc))
        amt_UpdateOffices();
    ai_Unk4e3290(cur);
    ai_Unk4e3290(track);
    if (sound != 0)
        msx_StopAmbient(sound);
    ai_Unk4e3640();
    memcpy(a, copy, 0x114);
    ai_Unk576040(0x30, 0, 0, 0);
}
