/* 0x588950, the drag window of a goods item (598 B, ROUGH: a start Ghidra missed - 0x588970 is the body, the
   guard falls through): idx = the item index, item = 0x91aaf0 + idx * 0x41; the kind 0x1d -> -1. The window
   "ToolTip\ToolTip_Objekte_Upgrade" (0x62a984) with 0x475600, the position from the window record and the screen
   width 0xfb9072 in 0x46cda0; the sections 2 / 1 / 3, 0x4731d0 (0, 0, idx + 0xdf), the texts idx * 2 + 0x9a9 /
   idx * 2 + 0x9a8, 0x21 (the byte +0x40) or 0x23 (0x5087f0 (idx, 0x64) * (0x6f23fc * 0.25 + 0.5) -> int); the
   text 0x20 (the dword +0x22). With 0x6f2308 / 0x6f230c / 0x75ec60 (the first non-0) and the item kind not 2 / 6:
   the building record of that object (0x7e3f30 + kind * 0x24d), its 0x40 words at +0x23 (& 0x7fff) against the
   item's word, the bytes +0x1a3 / +0x1e3, the text 0x24 and 0xf9e / 0xfe0 / the word + 0xf98 in 0x515960
   (0x62a978 / 0x62a96c / 0x62a960); the window */

int ai_Unk588950(int idx)
{
    char *item;
    char *o;
    int win;
    int i;
    int v;

    item = (char *)U_I((unsigned)&ai_Goods) + idx * 0x41;
    if (*(unsigned char *)item == 0x1d)
        return -1;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a984);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, (char *)(idx + 0xdf), U_I((unsigned)&d2_LastWindow));
    io_PrintText(idx * 2 + 0x9a9);
    d2_SetForm(0, 1);
    io_PrintText(idx * 2 + 0x9a8);
    d2_SetForm(0, 3);
    if (*(unsigned char *)(item + 0x40) != 0)
        io_PrintText(0x21, *(unsigned char *)(item + 0x40));
    else
        io_PrintText(0x23, (int)(gm_ItemPrice(idx, 0x64) * (float)(U_I((unsigned)&ai_Difficulty) * 0.25 + 0.5)));
    io_PrintText(0x20, *(int *)(item + 0x22));
    v = U_I((unsigned)&ai_PlayerBuilding);
    if (v == 0)
        v = U_I((unsigned)&ai_FallbackBuilding);
    if (v == 0)
        v = U_I((unsigned)&ai_Unk75ec60);
    if (v == 0)
        return win;
    if (*(unsigned char *)item == 2 || *(unsigned char *)item == 6)
        return win;
    o = (char *)U_I((unsigned)&ai_BTypes) + *(unsigned char *)v * 0x24d;
    for (i = 0; i < 0x40; i++)
        if ((*(unsigned short *)(o + 0x23 + i * 2) & 0x7fff) == *(unsigned short *)item)
            break;
    if (i == 0x40)
        return win;
    io_PrintText(0x24);
    v = *(unsigned char *)(o + 0x1a3 + i);
    i = *(unsigned char *)(o + 0x1e3 + i);
    if (v == 0 || v == -1 || v == 0xff)
        return win;
    if (v == 3)
        io_PrintText((char *)(unsigned)&full_data_62a978, i, 0xf9e);
    else if (v == 0x24)
        io_PrintText((char *)(unsigned)&full_data_62a96c, i, 0xfe0);
    else
        io_PrintText((char *)(unsigned)&full_data_62a960, i, v * 2 + 0xf98);
    return win;
}
