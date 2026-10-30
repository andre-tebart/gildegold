/* 0x591ad0, finds the player's building in the jail (278 B): 0x53e400 (1, 0, 0x3d) walks the buildings; the one
   whose +0xac is the player's building word is taken, else the character query 0x501190 (1, 5, 6) / 0x501030
   (the one with +0x65); 0x4ffe90 (the record +0x5d, 1, 0, 0xba / 0x65) gives the jail record and 0x521960
   (the record, +2, the text 0x631090) reports it; the errors 0x62b1d8 / 0x62b198 go to 0x4222b0 */

void ai_Unk591ad0(void)
{
    int b;
    int r;
    int it;

    b = he_FindEvent(1, 0, 0x3d);
    while (b != 0) {
        if (*(int *)(b + 0xac) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87))
            break;
        b = he_NextEvent();
    }
    if (b == 0) {
        it = gm_BuildQuery(1, 5, 6);
        if (it != 0) {
            r = gm_Query(*(int *)(it + 0x5d), 1, 0, 0xba);
            if (r != 0 && U_I((unsigned)&sv_PanelShown) != r)
                sv_SelectPanelObjectK(it, U_I(r + 2), (void *)(unsigned)&ch_EmptyName);
            return;
        }
        err_Message("lc_PlayerInJail(): Could not find the jail building");
        return;
    }
    it = gm_BuildQuery(1, 5, 4);
    while (it != 0) {
        if (*(int *)(it + 0x65) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87))
            break;
        it = gm_BuildNext();
    }
    if (it == 0) {
        err_Message("lc_PlayerInJail(): Could not find the character");
        return;
    }
    r = gm_Query(*(int *)(it + 0x5d), 1, 0, 0x65);
    if (r == 0 || U_I((unsigned)&sv_PanelShown) == r)
        return;
    sv_SelectPanelObjectK(it, U_I(r + 2), (void *)(unsigned)&ch_EmptyName);
}
