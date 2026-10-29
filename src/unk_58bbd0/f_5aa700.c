/* 0x5aa700, the worker list of a Domizil window (672 B): up to three entries of the array 0x6f40a4 (0x509060
   fills it, the count 0x6f40a0), each a picture 0x5db, a picture 0x639 + the kind of the entry, the text
   0x4b1 + its word and a small icon control (picture 5) whose object is the master name of the entry
   (0x501190 (1, 1, +0xb4)) or the name 0x50a0f0 of a free one; the controls 0x6f40b0 / 0x6f40bc start at -1 */

void ai_Unk5aa700(int param_1, char *param_2)
{
    int pics[4];
    char text[512];
    char *name;
    int ctrl;
    int rec;
    int n;
    int i;

    U_I((unsigned)&full_data_6f40a0) = 0;
    for (i = 0; i < 3; i++) {
        U_I((unsigned)&full_data_6f40b0 + i * 4) = -1;
        U_I((unsigned)&full_data_6f40bc + i * 4) = -1;
    }
    d2_SetForm(*(int *)(param_1 + 0x234), 1);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(0x1a1f);
    d2_SetForm(*(int *)(param_1 + 0x234), 3);
    n = gm_Unk509060(param_2, (char *)(unsigned)&full_data_6f40a4);
    if (n > 3)
        n = 3;
    U_I((unsigned)&full_data_6f40a0) = n;
    for (i = 0; i < n; i++) {
        pics[0] = unk_Unk4fb770((char *)(unsigned)&full_data_624ec4);
        pics[1] = unk_Unk4fb770((char *)(unsigned)&full_data_624fcc);
        pics[2] = unk_Unk4fb770((char *)(unsigned)&full_data_62cbb4);
        pics[3] = unk_Unk4fb770((char *)(unsigned)&full_data_60d38c);
        rec = U_I((unsigned)&full_data_6f40a4 + i * 4);
        d2_WinOpenPic(0xe, i * 0x6e + 8, 0x5db, U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(0x10, i * 0x6e + 10, pics[U_I(rec + 2) & 3] + 0x639, U_I((unsigned)&d2_LastWindow));
        d2_WinOpenPic(0x50, i * 0x78, U_W(rec) + 0x4b1, U_I((unsigned)&d2_LastWindow));
        name = 0;
        ctrl = *(int *)(rec + 0x24);
        if (ctrl == 0) {
            name = (char *)gm_Unk50a0f0(rec);
            if (*name == 'D')
                name = (char *)U_I((unsigned)&ai_MarketBuilding);
            txt_Format(text, 0x1a3e, name);
        } else {
            name = (char *)gm_BuildQuery(1, 1, *(int *)(ctrl + 0xb4));
            if (name != 0)
                txt_Format(text, 0x1a3e, name);
        }
        ctrl = d2_CreateWinText(0, i * 0x78 + 0x48, text, U_I((unsigned)&d2_LastWindow));
        U_I((unsigned)&full_data_6f40bc + i * 4) = ctrl;
        d2_SetObjFont(ctrl, 0x56);
        if (name != 0) {
            ctrl = d2_WinOpenPic(0, i * 0x78 + 0x5a, 5, U_I((unsigned)&d2_LastWindow));
            U_I((unsigned)&full_data_6f40b0 + i * 4) = ctrl;
            U_I((unsigned)&d2_Objs + 0x4c + ctrl * 0x2ec) = 1;
            U_I((unsigned)&d2_Objs + 0x2e8 + ctrl * 0x2ec) = (int)name;
            d2_SetObjFont(ctrl, 0x56);
            d2_SetCaption(ctrl, (char *)U_I((unsigned)&full_data_1440ab4));
            U_I((unsigned)&d2_Objs + 0x1c0 + ctrl * 0x2ec) = 3;
        }
    }
}
