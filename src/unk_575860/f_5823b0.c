/* 0x5823b0, the unit purchase window (429 B): arg 1 = the record (unused), arg 2 = the entry list; its +0x20 is
   the price; the window "Locations_Domizil_kabinett" (0x62a418) with 0x476f80 / 0x475600, the sound 0x52ea30,
   the texts 0x602568 / 0x407d / 0x4171 (the price) and the entries 0x407e / +1 in 0x475b80 (window, 2, text); the
   pump loop 0x5309a0 (this handler, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry:
   0x520f90 (the character 0x770b80 + 0x6f25b8 * 0x21c, the price) == 1 -> 0x4c5f60 (the character, -the price),
   0x4c3ba0 (the list, the list + 2), 0x4c3c00 (4, 1, 0x1c, &0) / (4, 1, 0x20, &0), 0x4c49a0 () and the wait on
   0x4c3ae0 (pump 0x20007), 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window),
   0x52d7a0 () */

void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldDelta(int size, int n, short field, void *v); void cm_SendFieldDeltas(void);

void ai_Unk5823b0(char *rec, char *list)
{
    int win;
    int b1;
    int b2;
    int price;
    int i;
    int v;

    price = U_I(list + 0x20);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a418);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(0x407d, 0x4171, price);
    d2_SetForm(win, 2);
    i = io_PrintText(0x407e);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, price) == 1) {
                    v = 0;
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -price);
                    cm_BeginFieldBlock(list, U_I(list + 2));
                    cm_AddFieldDelta(4, 1, 0x1c, &v);
                    cm_AddFieldDelta(4, 1, 0x20, &v);
                    i = ((int (*)())cm_SendFieldDeltas)();
                    if (i != -1) {
                        while (cm_GetSlotState(i) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5823b0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
