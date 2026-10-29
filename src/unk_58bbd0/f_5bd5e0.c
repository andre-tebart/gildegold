/* 0x5bd5e0, the entry rows of a window (1408 B, ROUGH): the layout switch on param_4 (1: one 0x80 column,
   2 / 3: one, 4 / 5 / 6: two 0x40 columns, 7 / 8: three 0x80 columns) fills the 5-int layout local, then every
   row record of param_5 (0xe dwords: +0 the character, +4 the row control, +8 the slider, +0xc unused,
   +0x10 / +0x14 the position, +0x18 the icon, +0x1c / +0x20 / +0x24 / +0x28 the texts) gets the controls
   0x4731d0 (0x5da; the icon +0x54 for the kinds 5 / 6 / 7, the slider 0x46a960 (0x442) and its 0x50c550
   percent) and the two caption texts of the kind, the 0x5c33c0 check and the window list fields +0x4c /
   0x1c0 / 0x2e8; the page byte of the kind 6 gets 0x6d3. ROUGH: the text arguments of 0x5176d0 are taken
   from the disassembly order */

int ai_Unk5bd5e0(int param_1, int param_2, int param_3, int param_4, int *param_5, unsigned short *param_6)
{
    int lay[5];
    int ncol;
    int xstep;
    int g;
    int j;
    int i;
    int k;
    int x;
    int y;
    int ctrl;
    char *chr;
    char buf[256];

    lay[0] = -1;
    lay[1] = -1;
    lay[2] = -1;
    lay[3] = 0;
    ncol = 0;
    xstep = 0;
    switch (param_4) {
    case 1: ncol = 1; xstep = 0x80; lay[0] = 1; break;
    case 2: ncol = 1; xstep = 0x80; lay[0] = 2; break;
    case 3: ncol = 1; xstep = 0x80; lay[0] = 3; break;
    case 4: ncol = 2; xstep = 0x40; lay[0] = 2; lay[1] = 2; break;
    case 5: ncol = 2; xstep = 0x40; lay[0] = 3; lay[1] = 2; break;
    case 6: ncol = 2; xstep = 0x40; lay[0] = 3; lay[1] = 3; break;
    case 7: ncol = 3; lay[2] = 2; lay[1] = 2; lay[0] = 3; break;
    case 8: ncol = 3; lay[2] = 3; lay[1] = 2; lay[0] = 3; break;
    }
    i = 0;
    if (ncol != 0) {
        lay[4] = xstep;
        for (g = 0; g < ncol; g++) {
            int cnt = lay[g];
            int off = 0;
            for (j = 0; j < cnt && i < param_4; j++) {
                *(int *)((char *)param_5 + i * 0x38 + 0x14) = lay[4];
                if (cnt == 3)
                    *(int *)((char *)param_5 + i * 0x38 + 0x10) = off;
                else if (cnt == 2)
                    *(int *)((char *)param_5 + i * 0x38 + 0x10) = off + 0x65;
                else
                    *(int *)((char *)param_5 + i * 0x38 + 0x10) = 0xca;
                off += 0xca;
                i++;
            }
            lay[4] += 0x80;
        }
    }
    for (k = 0; k < param_4; k++) {
        int *rec = (int *)((char *)param_5 + k * 0x38);
        x = rec[4] + 0x65;
        y = rec[5] + 0x23;
        i = x - *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) / 2;
        ctrl = d2_WinOpenPic(i, y, 0x5da, U_I((unsigned)&d2_LastWindow));
        rec[1] = ctrl;
        chr = (char *)rec[0];
        if (chr != 0) {
            char kind = *(char *)(chr + 2);
            if (kind == 6 || kind == 7 || kind == 5)
                d2_WinOpenPic(i - 0x20, y, U_I(chr + 0x54), U_I((unsigned)&d2_LastWindow));
            rec[6] = d2_WinOpenPic((x - *(short *)(U_I((unsigned)&d2_Pics) + 0x50 + U_I(chr + 0x18c) * 0x54) / 2) - 3,
                                  rec[5] + 0x24, U_I(chr + 0x18c), U_I((unsigned)&d2_LastWindow));
            d2_WinOpenPic(i + 3, rec[5] + 0x26, rec[7], U_I((unsigned)&d2_LastWindow));
            if (rec[8] != 0)
                d2_WinOpenPic(i + 3, rec[5] + 0x26 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) - 0x13, rec[8],
                             U_I((unsigned)&d2_LastWindow));
            if (rec[9] != 0)
                d2_WinOpenPic(i + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) - 0x10, rec[5] + 0x26, rec[9],
                             U_I((unsigned)&d2_LastWindow));
            d2_WinOpenPic(i + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) - 0x10,
                         rec[5] + 0x26 + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) - 0x13, rec[10], U_I((unsigned)&d2_LastWindow));
            txt_Format(buf, (char *)(unsigned)&full_data_62d1f4,
                         (-(*(char *)(chr + 9) != 0) & 10) + 0x114 + *(unsigned char *)(chr + 0xd),
                         *(unsigned short *)chr);
            ai_Unk5bd0c0((int)buf, x, rec[5], 0xca, 0x56);
            if (param_2 == 7) {
                txt_Format(buf, "%s",
                             (-(*(char *)(chr + 9) != 0) & 0x28) + 0x253 + *(unsigned char *)(chr + 0x169));
            } else {
                txt_Format(buf, (char *)(unsigned)&full_data_62d1ec, *(unsigned short *)chr);
            }
            ai_Unk5bd0c0((int)buf, x, rec[5] + 0xf, 0xca, 0x56);
            if ((unsigned short *)chr != param_6 && *(char *)(chr + 2) != 6) {
                rec[2] = d2_WinOpenSlider(rec[4] + 0x42, *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebda) + 5 + y, 0x40, 0,
                                      100, 0x75, 0x442, U_I((unsigned)&d2_LastWindow));
                if (*(char *)(chr + 2) == 7)
                    d2_SetObjValue(rec[2], 0, 100, 0);
                else
                    d2_SetObjValue(rec[2], 0, 100, (int)gm_Liking(*param_6, *(unsigned short *)chr, 1));
                d2_SetObjEnabled(rec[2], 1);
                U_I((unsigned)&d2_Objs + rec[2] * 0x2ec + 0x4c) = 1;
            }
            if (param_6 != 0 && *(char *)(param_6 + 1) == 6) {
                if (U_I(chr + 0x210) == U_I((char *)param_6 + 4))
                    d2_WinOpenPic(i + *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8), y, 0x6d3, U_I((unsigned)&d2_LastWindow));
            }
            if (param_3 == 0 ||
                ai_Unk5c33c0(U_W((unsigned)&ai_PlayerId), *(unsigned short *)chr, (unsigned int *)param_3, 0) != 0) {
                U_I((unsigned)&d2_Objs + rec[1] * 0x2ec + 0x4c) = 1;
                U_I((unsigned)&d2_Objs + rec[1] * 0x2ec + 0x1c0) = 3;
                U_I((unsigned)&d2_Objs + rec[1] * 0x2ec + 0x2e8) = (int)chr;
            }
        }
    }
    return 0;
}
