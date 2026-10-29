/* 0x5a0590, the office application list "locations\rathaus\amtsbewerbung" (576 B): below 2 (the player's
   byte 0x770b8d) the text 0x14409cc goes to 0x521150; with the player's office byte 0x770ce8 set the
   application 0x5a0220 opens directly; 0x4e7320 (the player, 6, list, &count) gives the towns, 0x4e7150
   (the player, 6, list) their count (texts 0x14409d8 / 0x14409c8 when empty), the window, the text 0x1a01
   and per town a button 0x515960 (0x1a04, town + 0x253) 0x475b80; a click on a town opens 0x5a0220 (the
   player, town + 3) and the text 0x14409f4 on success; the pump 0x5309a0 (0x5a0590, 0x677c7) */

void ai_Unk5a0590(void)
{
    int list[18];
    int ctls[6];
    int count;
    int n;
    int i;
    int win;
    int r;
    char *per;
    int changed;

    per = (char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c;
    if (*(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x4) + U_W((unsigned)&ai_PlayerId) * 0x21c) < 2) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_14409cc), 0);
        return;
    }
    if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x164) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0) {
        ai_Unk5a0220((int)per, *(unsigned char *)(((unsigned)&ai_CharInts + 0x164) + U_W((unsigned)&ai_PlayerId) * 0x21c));
        sv_CameraFollow();
        return;
    }
    n = amt_GetOfficeCandidatesFull(per, 6, list, &count);
    if (n == 0) {
        if (count != 0) {
            sv_MessageBox((char *)U_I((unsigned)&full_data_14409d8), 0);
            return;
        }
        sv_MessageBox((char *)U_I((unsigned)&full_data_14409c8), 0);
        return;
    }
    win = d2_OpenForm(0, 0, "locations\\rathaus\\amtsbewerbung");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    changed = 1;
    n = -1;
    for (;;) {
        count = amt_GetOfficeCandidates(per, 6, list);
        if (count != n)
            changed = 1;
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            io_PrintText(0x1a01);
            for (i = 0; i < count; i++) {
                ctls[i] = d2_GetWinObj(win, 1, io_PrintText(0x1a04, list[i * 3] + 0x253));
            }
            for (i = count; i < 6; i++)
                ctls[i] = -1;
            changed = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && count > 0) {
            for (i = 0; i < count; i++) {
                if (ctls[i] == U_I((unsigned)&ai_Unk5fe710)) {
                    if (i >= 0 && i < count && ai_Unk5a0220((int)per, list[i * 3]) == 1) {
                        sv_MessageBox((char *)U_I((unsigned)&full_data_14409f4), 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                    break;
                }
            }
        }
        if (sv_RunFrame(ai_Unk5a0590, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
