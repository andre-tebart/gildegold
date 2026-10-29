/* 0x5c82f0, the player comparison panel (837 B): the ranking 0x506180 (up to 8 records of 0x24 bytes, the
   character index first, the score at +0x20), the two captions "panel_pcompare4plus6" 0x62d468 (count < 7) /
   "panel_pcompare8" 0x62d458; the window 0x4741f0 / 0x476f80 / 0x475600, the page 0x515960 (0x1f35) and the
   rows: the portrait 0x4731d0 (0x28, x, the character's +0x50, screen) into the control record 0xf8f080 +
   id * 0x2ec (+0x4c = 1, +0x1c0 = 3, +0x2e8 = 0x770b80 + idx * 0x21c), the name 0x5176d0 (0x62a564, idx) /
   0x5bd0c0 (0x36, x + 0x24, 300, 0x56), the slider 0x46a960 (0x5a, x + 9, 300, 0, 100, the colour
   pos[clamp(char +0x50 - 0x59a, 7)], 0x42, screen) set by 0x475cb0 (0, 100, win) and the score 0x5176d0
   (0x5fe7d0, record +0x20) / 0x5bd0c0 (500, x + 2, 200, 0x55). The row x: count <= 4 -> 0x50 * i + 0x14,
   else 0x3a * i + 2. The pump 0x5309a0 and the end 0x4758f0 / 0x576040 (10, 0, 1, 0). */

void gm_Unk506180(int *count, unsigned short *out);   /* 0x506180 (defined in f3gm.c) */

void ai_Unk5c82f0(void)
{
    int pos[8];
    unsigned short list[144];
    char buf[0x100];
    int count;
    int win;
    int i;
    int x;
    int y;
    int id;
    int ctrl;
    unsigned int idx;
    unsigned int n;

    pos[0] = 0x8f;
    pos[1] = 0x95;
    pos[2] = 0x9b;
    pos[3] = 0xa1;
    pos[4] = 0xa7;
    pos[5] = 0xad;
    pos[6] = 0xb3;
    pos[7] = 0xb9;
    gm_Unk506180(&count, list);
    if (count == 0 || ai_Unk5763c0(4) == 0)
        return;
    ai_Unk576040(10, 0, 0, 0);
    if (count < 7)
        win = d2_OpenForm((char *)(unsigned)&full_data_62d468, 0, 0);
    else
        win = d2_OpenForm((char *)(unsigned)&full_data_62d458, 0, 0);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1f35);
    d2_SetForm(win, 2);
    for (i = 0; i < count; i++) {
        idx = list[i * 0x12];
        x = (count > 4 ? 0x3a : 0x50) * i + (count > 4 ? 2 : 0x14);
        id = d2_WinOpenPic(0x28, x, *(int *)((unsigned)&ai_CharInts + idx * 0x21c + 0x50), *(int *)(unsigned)&d2_LastWindow);
        id *= 0x2ec;
        *(int *)((unsigned)&d2_Objs + id + 0x4c) = 1;
        *(int *)((unsigned)&d2_Objs + id + 0x1c0) = 3;
        *(int *)((unsigned)&d2_Objs + id + 0x2e8) = (unsigned)&ai_Chars + idx * 0x21c;
        txt_Format(buf, (char *)(unsigned)&full_data_62a564, idx);
        ai_Unk5bd0c0(buf, 0x36, x + 0x24, 300, 0x56);
        n = *(int *)((unsigned)&ai_CharInts + idx * 0x21c + 0x50) - 0x59a;
        if (n > 7)
            n = 7;
        ctrl = d2_WinOpenSlider(0x5a, x + 9, 300, 0, 100, pos[n], 0x42, *(int *)(unsigned)&d2_LastWindow);
        d2_SetObjValue(ctrl, 0, 100, win);
        txt_Format(buf, (char *)(unsigned)"%i", *(int *)(list + i * 0x12 + 0x10));
        ai_Unk5bd0c0(buf, 500, x + 2, 200, 0x55);
    }
    *(int *)(unsigned)&d2_ClickedId = -1;
    do {
        if (*(int *)((unsigned)&dm_L + 0x20))
            *(int *)(unsigned)&sv_PopRequest = 1;
    } while (sv_RunFrame(ai_Unk5c82f0, 0x657c7) != 0);
    d2_CloseView(win);
    ai_Unk576040(10, 0, 1, 0);
}
