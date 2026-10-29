/* 0x5bdb60, the entry rows of a kind 8 window (1648 B, ROUGH): the switch on param_2 (1 .. 8) sets the column
   origins of the row records of param_5 (0xe dwords each: the x / y at +0x10 / +0x14, the other fields as in
   0x5bd5e0), then per row: the text control 0x4731d0 (0x5da), the icon +0x54, the two texts 0x5176d0 /
   0x5bd0c0 (0x62d1f4 / 0x62d1ec; a kind 8 row: 0x62d1fc and the page word 0x23 .. 0x27 of the flags +0x1cc),
   the slider 0x46a960 (0x442) with 0x50c550, the 0x5c33c0 check and the window list +0x4c / +0x1c0 / +0x2e8.
   ROUGH: the switch's field assignments are grouped by kind, the text arguments from the disassembly */

int ai_Unk5bdb60(int param_1, int param_2, int param_3, int param_4, int param_5, unsigned short *param_6)
{
    int k;
    int i;
    int x;
    int y;
    int page;
    unsigned int flags;
    char *chr;
    char buf[256];
    int ok;

    ok = 0;
    if (param_2 == 1 || param_2 == 2 || param_2 == 3) {
        U_I(param_5 + 0x80) = 0x65;
        U_I(param_5 + 0x10) = 0xca;
        U_I(param_5 + 0x48) = 0xca;
        U_I(param_5 + 0xb8) = 0x12f;
        U_I(param_5 + 0x14) = 0;
        U_I(param_5 + 0x4c) = 0x80;
        U_I(param_5 + 0x84) = 0x100;
        U_I(param_5 + 0xbc) = 0x100;
        ok = 1;
    } else if (param_2 == 4 || param_2 == 5 || param_2 == 6) {
        U_I(param_5 + 0x10) = 0xca;
        U_I(param_5 + 0xf0) = 0xca;
        U_I(param_5 + 0x48) = 0x65;
        U_I(param_5 + 0x4c) = 0x80;
        U_I(param_5 + 0x84) = 0x80;
        U_I(param_5 + 0x80) = 0x12f;
        U_I(param_5 + 0xb8) = 0;
        U_I(param_5 + 0x128) = 0x194;
        U_I(param_5 + 0x14) = 0;
        U_I(param_5 + 0xf4) = 0x100;
        U_I(param_5 + 0x12c) = 0x100;
        ok = 1;
    } else if (param_2 == 8) {
        U_I(param_5 + 0x10) = 0x12f;
        U_I(param_5 + 0xb8) = 0x12f;
        U_I(param_5 + 0x48) = 0x65;
        U_I(param_5 + 0x14) = 0;
        U_I(param_5 + 0x4c) = 0;
        U_I(param_5 + 0x80) = 0xca;
        U_I(param_5 + 0xf0) = 0x65;
        U_I(param_5 + 0x84) = 0x80;
        U_I(param_5 + 0xf4) = 0x100;
        ok = 1;
    }
    if (ok)
        U_I(param_5 + 0xbc) = 0x100;
    for (k = 0; k < param_4; k++) {
        int *rec = (int *)((char *)param_5 + k * 0x38);
        if (!ok)
            return 0;
        x = rec[4] + 0x65;
        y = rec[5] + 0x23;
        i = x - *(short *)(U_I((unsigned)&d2_Pics) + 0x1ebd8) / 2;
        rec[1] = d2_WinOpenPic(i, y, 0x5da, U_I((unsigned)&d2_LastWindow));
        rec[2] = -1;
        chr = (char *)rec[0];
        if (chr == 0) {
            d2_WinOpenPic((x - *(short *)(U_I((unsigned)&d2_Pics) + 0x1a2ac) / 2) - 3, rec[5] + 0x24, 0x4fb,
                         U_I((unsigned)&d2_LastWindow));
            txt_Format(buf, "%s", rec[3] + 0x253);
            ai_Unk5bd0c0((int)buf, x, rec[5], 0xca, 0x56);
            U_I((unsigned)&d2_Objs + rec[1] * 0x2ec + 0x4c) = 1;
            U_I((unsigned)&d2_Objs + rec[1] * 0x2ec + 0x2e8) = 0;
            d2_SetObjEnabled(rec[1], 0);
        } else {
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
            if (param_2 == 8) {
                flags = U_I(chr + 0x1cc);
                if (flags & 0x1000)
                    page = 0x26;
                else if (flags & 0x2000)
                    page = 0x27;
                else if (flags & 0x800)
                    page = 0x25;
                else if (flags & 0x400)
                    page = 0x24;
                else
                    page = (flags & 0x200) ? 0x23 : 0;
                txt_Format(buf, (char *)(unsigned)&full_data_62d1fc, *(unsigned short *)chr);

                ai_Unk5bd0c0((int)buf, x, rec[5], 0xca, 0x56);
                txt_Format(buf, "%s",
                             (-(*(char *)(chr + 9) != 0) & 0x28) + 0x253 + page);
                ai_Unk5bd0c0((int)buf, x, rec[5] + 0xf, 0xca, 0x56);
            } else {
                txt_Format(buf, (char *)(unsigned)&full_data_62d1f4,
                             (-(*(char *)(chr + 9) != 0) & 10) + 0x114 + *(unsigned char *)(chr + 0xd),
                             *(unsigned short *)chr);
                ai_Unk5bd0c0((int)buf, x, rec[5], 0xca, 0x56);
                txt_Format(buf, (char *)(unsigned)&full_data_62d1ec, *(unsigned short *)chr);
                ai_Unk5bd0c0((int)buf, x, rec[5] + 0xf, 0xca, 0x56);
            }
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
