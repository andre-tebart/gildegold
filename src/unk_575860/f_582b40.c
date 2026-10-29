/* 0x582b40, the contact detail panel (1449 B, ROUGH SKELETON: the entry collection, the item widgets and the
   header texts are written, the detail texts of the lower half are comments): rec = the record, w = the window
   list (0 the window, 3 the screen part, 4 / 5 / 6 the fonts, 0x4d the selected entry, 0x4e the detail window,
   0x50 the item records of 3 dwords each, 0x51 their widgets), clicked = the newly clicked entry, flag = 0 / 1.
   The selected entry w[w[0x4d] * 4 + 0xe] and its record; the sub-entries (+4, 0x44 bytes each, whose +0 != -1
   are collected); the list zeroed to -1 beyond; w[0x4e] = -1; the scroll position 0xf8f368 + w[3] * 0x3c4
   saved; the entries w[0] 3 / 5 / 7 with the text 0x602568 and the scroll area 0x52e860 (0, 0, w[6], w[3], the
   screen height 0xf8f12a + w[3] * 0x3c4); per item: the button 0x4731d0 (the centre, the row, 0x6c7) and its
   +0x3c child window 0x4731d0, the window table 0xf8f080 +0x4c = (flag == 0), +0x1c0 = 3, the two texts
   0x5830f0 / 0x583150 and the five value labels 0x473650 (0x28, 0x6002 / 0x16b7 + i ... 0x62a56c, width 300,
   0x4760b0 (0x54)) and 0x52eaa0 (0xa0, the row + i * 0xd, the item byte +0x24 + i, 0x4df); the position
   0xf8f368 restored, the detail 7 with the text 0x602568; the detail text: the plain name 0x5eecd0[*e] through
   0x5176d0 (0x4100) into 0x624204, or the character 0x505df0 (e[1]) and the three strings 0x62a54c (0x4101 the
   name 0x62a564, 0x4102 "AD: %i" 0x62a544, 0x4103 "%i ..." 0x62a53c with a __ftol value); with flag 1 the
   detail window 0x46bf10 (the centre, 0xbf, 0x144ad84) with 0x46c260 (0xb4); the position restored and
   0x46f840 (w[3]) */

void ai_Unk5830f0(); void ai_Unk583150();

void ai_Unk582b40(char *rec, int *w, int clicked, int flag)
{
    char buf1[0x100];
    char buf2[0x100];
    int *e;
    int n;
    int i;
    int k;
    int saved;

    e = (int *)w[w[0x4d] * 4 + 0xe];
    saved = 0;
    n = 0;
    if (*(int *)((unsigned)&gv_EntryCounts + *e * 4) > 0) {
        int *p = w + 0x51;
        for (i = 0; i < *(int *)((unsigned)&gv_EntryCounts + *e * 4); i++) {
            if (*(int *)(e[4] + i * 0x44) != -1) {
                p[-1] = e[4] + i * 0x44;
                p[-2] = -1;
                p[0] = *(int *)(e[4] + i * 0x44);
                n++;
                p += 3;
            }
        }
    }
    if (n < 8) {
        int *p = w + n * 3 + 0x51;
        for (i = 0; i < 8 - n; i++) {
            p[-2] = -1;
            p[-1] = 0;
            p[0] = -1;
            p += 3;
        }
    }
    w[0x4e] = -1;
    if (clicked == w[0x4d])
        saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[3] * 0x3c4);
    d2_SetForm(w[0], 3);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 5);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 7);
    io_PrintText((char *)(unsigned)"$C");
    sv_CreateFormSlider(0, 0, w[6], w[3], *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[3] * 0x3c4), buf2, buf1, &clicked);
    d2_SetForm(w[0], 3);
    /* ROUGH: the per-item widgets (0x4731d0 / 0x473650 / 0x5830f0 / 0x583150 / 0x52eaa0, see above) and the
       detail texts are not written yet */
    U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[3] * 0x3c4) = saved;
    d2_ScrollWindow(w[3]);
}
