/* 0x5aa9a0, refreshes the worker list 0x5aa700 (480 B): per entry of 0x6f40a4 the text 0x1a3f (a free one,
   0x50a0f0) or 0x1a3e (the master 0x501190 (1, 1, +0xb4)) into the control 0x6f40bc; a missing icon control
   0x6f40b0 is created (picture 5, the object the name, 0x1440ab4 the caption); on a click (0xf8a668 == 5) the
   clicked id 0x5fe710 is released with 0x521960 */

void ai_Unk5aa9a0(int param_1, char *param_2)
{
    char text[512];
    char *name;
    int rec;
    int ctrl;
    int i;

    for (i = 0; i < U_I((unsigned)&full_data_6f40a0); i++) {
        rec = U_I((unsigned)&full_data_6f40a4 + i * 4);
        ctrl = *(int *)(rec + 0x24);
        name = 0;
        if (ctrl == 0) {
            name = (char *)gm_Unk50a0f0(rec);
            if (*name == 'D')
                name = (char *)U_I((unsigned)&ai_MarketBuilding);
            txt_Format(text, 0x1a3f, name);
        } else {
            name = (char *)gm_BuildQuery(1, 1, *(int *)(ctrl + 0xb4));
            if (name != 0)
                txt_Format(text, 0x1a3e, name);
        }
        if (U_I((unsigned)&full_data_6f40bc + i * 4) != -1) {
            d2_SetObjText(U_I((unsigned)&full_data_6f40bc + i * 4), text);
            if (name != 0 && U_I((unsigned)&full_data_6f40b0 + i * 4) == -1) {
                ctrl = d2_WinOpenPic(0, i * 0x78 + 0x5a, 5, U_I((unsigned)&d2_LastWindow));
                U_I((unsigned)&full_data_6f40b0 + i * 4) = ctrl;
                U_I((unsigned)&d2_Objs + 0x4c + ctrl * 0x2ec) = 1;
                U_I((unsigned)&d2_Objs + 0x2e8 + ctrl * 0x2ec) = (int)name;
                d2_SetObjFont(ctrl, 0x56);
                U_I((unsigned)&d2_Objs + 0x1c0 + ctrl * 0x2ec) = 3;
            }
            if (U_I((unsigned)&full_data_6f40b0 + i * 4) != -1)
                d2_SetCaption(U_I((unsigned)&full_data_6f40b0 + i * 4), (char *)U_I((unsigned)&full_data_1440ab4));
        }
    }
    if (*(unsigned char *)(param_1 + 0x270) == 2 && U_I((unsigned)&d2_ClickedId) == 5) {
        for (i = 0; i < 3; i++) {
            ctrl = U_I((unsigned)&full_data_6f40b0 + i * 4);
            if (ctrl != -1 && U_I((unsigned)&ai_Unk5fe710) == ctrl)
                sv_SelectPanelObject(U_I((unsigned)&d2_Objs + 0x2e8 + ctrl * 0x2ec), -1, (char *)(unsigned)&ch_EmptyName);
        }
    }
}
