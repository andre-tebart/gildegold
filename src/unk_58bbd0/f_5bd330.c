/* 0x5bd330, the row list of a window (400 B): the rows 0x50a150 (*param_1, 0, ids) capped at param_2
   (8 -> 9) and the screen half width 0x6ed918 + 0x269d8; per row the text control 0x4731d0 (0x75a), the
   slider 0x472aa0 (0x72, 0x10), the character id, the kind byte 0x56 of the screen record at 0xf8f39c and
   the high word 0x10 of 0xf8f130 (both + ctrl * 0x3c4) and the owner 0x502020 of the character +0xac with
   the text 0x515960 (0x1f41, the owner, the +0xb4, the +0xc0, the +0xc4) */

int ai_Unk5bd330(unsigned short *param_1, int param_2, int param_3, int param_4, int param_5)
{
    int ids[9];
    int n;
    int half;
    int i;
    int ctrl;
    int slider;
    int own;
    char *chr;

    if (param_2 > 8)
        param_2 = 9;
    n = gm_Unk50a150(*param_1, 0, ids);
    half = *(short *)(U_I((unsigned)&d2_Pics) + 0x269d8);
    if (n > param_2)
        n = param_2;
    for (i = 0; i < n; i++) {
        d2_SetForm(param_4, param_5);
        ctrl = d2_WinOpenPic((*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) - half) >> 1, i * 0x80, 0x75a, U_I((unsigned)&d2_LastWindow));
        slider = d2_CreateChildWindow((*(short *)(U_I((unsigned)&d2_LastWindowPtr) + 8) - half) >> 1, i * 0x80 + 4, half, 0x72, 0x10,
                              U_I((unsigned)&d2_LastWindow));
        *(int *)(param_3 + i * 0xc + 0) = slider;
        *(int *)(param_3 + i * 0xc + 4) = ctrl;
        *(int *)(param_3 + i * 0xc + 8) = ids[i];
        *(unsigned short *)((unsigned)&ai_Unkf8f39c + slider * 0x3c4) = 0x56;
        *(unsigned short *)(((unsigned)&ai_Unkf8f128 + 0x8) + slider * 0x3c4) = 0x10;
        chr = (char *)ids[i];
        own = gm_FindBuildingById(U_I(chr + 0xac));
        if (own != 0)
            io_PrintText(0x1f41, own, U_I(chr + 0xb4), U_I(chr + 0xc0), (int)(chr + 0xc4));
    }
    return 0;
}
