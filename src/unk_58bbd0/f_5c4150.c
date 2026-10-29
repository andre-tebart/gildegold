/* 0x5c4150, the family tree window "Special_Stammbaum" 0x62d300 (4542 B, ROUGH SKELETON): the gate
   0x5763c0 (1) and the event 0x576040 (6, 0, 0, 0); the character 0x505df0 (param_1); the window
   0x4741f0 (0, 0, 0x62d300), 0x476f80, 0x475600, page 2 with the caption param_3 (0x9f when 0),
   0x515960 and page 1; the frame loop: the screen width / 2 from 0xf8f128 + screen * 0x3c4 and the
   rect of 0x6ed918 + 0x1ec2c, the cleared local tables (the 8 ids, the 6 + 2 portraits, the 5
   siblings), the parents 0x505df0 (record +0x128 / +0x12c), the partner (record +0x130), the six
   children (+0xd0, the kind +2 < 10, the first two at 0x770d0c + id * 0x21c), the grandparents, the
   boxes 0x4764f0 and the labels 0x5176d0 (0x62d1fc, the id) / 0x5bd0c0 (text, x, y, 0x50, 0x56); the
   portrait ids 0x4731d0 (0x5db / 0x4fb / 0x98 / 0x96, the kind record) with the control record
   0xf8f080 + id * 0x2ec (+0x1c0 = 3, +0x2e8 = 0x770b80 + id * 0x21c, +0x4c the visible flag from
   param_2 & 0x10 / 0xc / 8); a click (0x5fe710) resolves 0x505df0 (0x770b84 + id * 0x21c) and runs
   0x5c5310 (the character, param_2 & 0x10 ? 0 : the own flag) between 0x475590 (win, 0 / 1); the pump
   0x5309a0 (this, param_4), the exit 0x4758f0 and the event 0x576040 (6, 0, 1, 0). ROUGH: the eight
   positions of the generation blocks are comments, the calls are grouped per block */

void d2_PlotLine();

void ai_Unk5c4150(int param_1, unsigned char param_2, int param_3, int param_4)
{
    char *ch;
    int win;
    int draw;
    int i;
    unsigned short ids[6];
    unsigned short parents[2];
    unsigned short children[4];
    unsigned short grand[5];
    int ctrl;
    int x;
    int y;

    if (ai_Unk5763c0(1) == 0)
        return;
    ai_Unk576040(6, 0, 0, 0);
    ch = (char *)gm_FindChar(param_1);
    if (ch == 0)
        return;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d300);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    if (param_3 == 0)
        param_3 = 0x9f;
    io_PrintText(param_3);
    d2_SetForm(win, 1);
    draw = 1;
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (draw) {
            for (i = 0; i < 6; i++)
                ids[i] = 0xffff;
            for (i = 0; i < 2; i++)
                parents[i] = 0xffff;
            for (i = 0; i < 4; i++)
                children[i] = 0xffff;
            for (i = 0; i < 5; i++)
                grand[i] = 0xffff;
            draw = 0;
            /* ROUGH: the two parents 0x505df0 (*(int *)(ch + 0x128) / *(int *)(ch + 0x12c)), the partner
               (0x130) and the six children of ch + 0xd0 (kind +2 < 10), then the grandparents; each
               portrait is 0x4731d0 (the y of its row, 0x5db / the picture 0x4fb or 0x98, 0x770d0c +
               id * 0x21c, screen) with the record 0xf8f080 + id * 0x2ec (+0x1c0 = 3, +0x2e8) */
            d2_SetForm(win, 1);
            d2_ClearPaintbox();
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            /* ROUGH: the generation rows: 0x4731d0 (x, 10 / 0xc / 0x96 / 0x98 / 0x122 / 0x124, the box
               text 0x5db / 0x4fb / the picture, screen); the partner boxes; the five siblings at
               0x4764f0 (x, y, x2, y2, 0x10, 0x10, 0x10) and the labels 0x5176d0 (0x62d1fc, the id) +
               0x5bd0c0 (text, x, y + 3, 0x50, 0x56); the own portrait 0x4731d0 (x - 10 - width / 2,
               y - 5, ch + 0x2a, screen) */
            for (i = 0; i < 6; i++) {
                if (ids[i] != 0xffff) {
                    /* ROUGH: the visible flag of the portrait control */
                    if ((param_2 & 1) != 0 && (param_2 & 0xc) == 0 && (param_2 & 0x10) == 0)
                        U_I(U_I((unsigned)&d2_Objs) + 0x4c + ctrl * 0x2ec) = 1;
                    else
                        U_I(U_I((unsigned)&d2_Objs) + 0x4c + ctrl * 0x2ec) = 0;
                    txt_Format((char *)0x300, (char *)(unsigned)&full_data_62d1fc, ids[i]);   /* ROUGH: the real buffer */
                    ai_Unk5bd0c0((char *)0x300, x, y + 3, 0x50, 0x56);
                }
            }
            /* ROUGH: the same for the two parents (the flag = (param_2 & 0xc) == 0) and the five
               siblings (the flag with the kind < 0xb test of 0x770b8a + id * 0x21c) */
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 && (param_2 & 8) == 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            /* ROUGH: the click search over the portraits (0x505df0 (0x770b84 + id * 0x21c) the
               character), then 0x475590 (win, 0), 0x5c5310 (the character, param_2 & 0x10 ? 0 : the own
               flag) and 0x475590 (win, 1) */
        }
        if (sv_RunFrame(ai_Unk5c4150, param_4) == 0)
            break;
    }
    d2_CloseView(win);
    ai_Unk576040(6, 0, 1, 0);
}
