/* 0x5973f0, the wine-cellar window "locations\wohnsitz\weinkeller" (272 B): 0x4741f0 / 0x475600, the texts
   0x515960 (0x42cf / 0x42d1), the pump 0x5309a0 (this, 0x677c7); the click 0 (0xf8a668) resolves the
   character's drink (0x506480 with the record 0x770b80 + player * 0x21c and the local choice): failure gives
   the text 0x143a1c0[0x42d2 + (choice != 0)] through 0x521150, success 0x4fca40 (the word 0x770b80 + player *
   0x21c, -0x7d0); the click 1 or the state 0x14681c1 sets 0x6f22a0; the exit 0x4758f0 */

int gm_Unk506480(); float gm_AddHealth(unsigned id, int d); int ai_Unk578170(); int ai_Unk5b4590();

void ai_Unk5973f0(void)
{
    int win;
    unsigned int player;
    int r;
    int sel;

    player = U_W((unsigned)&ai_PlayerId);
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\weinkeller");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x42cf);
    d2_SetForm(win, 2);
    io_PrintText(0x42d1);
    while (sv_RunFrame(ai_Unk5973f0, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) == 0 && *(char *)((unsigned)&kb_Down + 0x1) == 0) {
            if (U_I((unsigned)&d2_ClickedId) == 0) {
                sel = U_I((unsigned)&d2_ClickedId);
                r = gm_Unk506480((void *)((unsigned)&ai_Chars + player * 0x21c), &sel);
                if (r == 0)
                    sv_MessageBox(U_I((unsigned)&d2_Strings + (0x42d2 + (sel != 0)) * 4), 0);
                else
                    gm_AddHealth(U_W((unsigned)&ai_Chars + player * 0x21c), 0xfffff830);
            } else if (U_I((unsigned)&d2_ClickedId) == 1) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    d2_CloseView(win);
}
