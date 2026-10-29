/* 0x5941c0, the pickpocket window (676 B, ROUGH SKELETON from the disassembly; an unnamed Ghidra start): the
   window "locations\diebesgilde\taschendiebstahl" (0x4741f0 / 0x476f80 / 0x475600 / 0x475f70 (1)), the text
   0x515960 (0x19ee, the record's +0x27, the record) and two controls 0x475b80; the click search 0x53e400 /
   0x53e300, the messages 0x521150 / 0x4c4b70 / 0x4c2d20 / 0x5176d0, the carriers 0x524260, the pump 0x5309a0
   (this, 0x677c7) and the exit 0x4758f0 */

int ai_Unk5941c0(int param_1, int param_2, int rec)
{
    char buf[0x504];
    int win;
    int ctl1;
    int ctl2;
    int id;
    int b;
    int i;

    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\taschendiebstahl");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    id = io_PrintText(0x19ee, *(unsigned short *)(rec + 0x27), rec);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    /* ROUGH: the loop reads the click 0xf8a668 / 0x5fe710, searches the building / character (0x53e400,
       0x53e300), posts the messages (0x521150, 0x4c4b70, 0x4c2d20, 0x5176d0) and the carrier marks
       0x524260, then pumps 0x5309a0 (this, 0x677c7) */
    while (sv_RunFrame(ai_Unk5941c0, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        b = he_FindEvent(3, 0, 0x48, 2, *(unsigned short *)(rec + 0x27), 3, U_I(rec + 1));
        while (b != 0)
            b = he_NextEvent();
        sv_MessageBox();
        sv_MoveToGroup(param_1, U_I((unsigned)&sv_PanelShown), b, 0);
    }
    d2_CloseView(win);
    return 0;
}
