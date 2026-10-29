/* 0x583700, the graveyard detail panel (1123 B, ROUGH: the collection, the item widgets and the three detail
   texts are written, the layout numbers are from the disassembly): w = the window list (0 the window, 2 the
   screen part, 4 the font, 7 the selected entry record, 0xa the item buttons, 0xb the item records, 0xc the
   entries of 3 dwords each). The selected entry's sub-entries (0x44 bytes each, whose +0 != -1) collected into
   w + 0xc and the rest zeroed to -1; the scroll position 0xf8f368 + w[2] * 0x3c4 saved; the sections 2 / 3 / 5
   with the text 0x602568 and the scroll area 0x52e860 (0, 0, w[4], w[2], the height); per item: the button
   0x4731d0 (the centre, the row, 0x6c7) and its child window 0x4731d0 (the entry + 0x3c), the window table
   0xf8f080 +0x4c = 1 / +0x1c0 = 3, the name / second line 0x5830f0 / 0x583150 as two windows 0x473650 and the
   five value labels 0x473650 (0x28) / 0x52eaa0 (0xa0, 0x4df); the position restored; the detail 5 with
   0x602568 and the character's name 0x770b80 + 0x6f25b8 * 0x21c through 0x5176d0 (0x62a564) and 0x515960
   (0x62a54c, 0x4101), "AD: %i" 0x62a544 (0x4102, the word at the entry + 0xa + 0x578) and "%i ..." 0x62a53c
   (0x4103, a 0x5de518 conversion) */

void ai_Unk583700(int *w)
{
    char buf1[0x100];
    char buf2[0x100];
    char a[4];
    char b[4];
    char c[4];
    int *e;
    int *p;
    int n;
    int k;
    int i;
    int id;
    int x;
    int saved;

    n = 0;
    e = (int *)w[7];
    saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4);
    if (*(int *)((unsigned)&gv_EntryCounts + *e * 4) > 0) {
        p = w + 0xc;
        for (i = 0; i < *(int *)((unsigned)&gv_EntryCounts + *e * 4); i++) {
            if (*(int *)(e[4] + i * 0x44) != -1) {
                p[-1] = e[4] + i * 0x44;
                n++;
                p[-2] = -1;
                p[0] = *(int *)(e[4] + i * 0x44);
                p += 3;
            }
        }
    }
    if (n < 8) {
        p = w + n * 3 + 0xc;
        for (i = 0; i < 8 - n; i++) {
            p[-2] = -1;
            p[-1] = 0;
            p[0] = -1;
            p += 3;
        }
    }
    d2_SetForm(w[0], 2);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 3);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 5);
    io_PrintText((char *)(unsigned)"$C");
    sv_CreateFormSlider(0, 0, w[4], w[2], *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4), b, a, c);
    d2_SetForm(w[0], 2);
    for (k = 0; k < n; k++) {
        x = *(short *)((unsigned)&ai_Unkf8f128 + w[2] * 0x3c4) - *(short *)((unsigned)&d2_Pics + 0x2399c);
        id = d2_WinOpenPic(x / 2, (*(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4) * k) + 2, 0x6c7, w[2]);
        w[0xb + k * 3 - 1] = id;
        id = d2_WinOpenPic(x / 2 + 2, (*(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4) * k) + 4, *(int *)(w[0xb + k * 3] + 0x3c), w[2]);
        w[0xb + k * 3 - 1] = id;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + w[0xb + k * 3 - 1] * 0x2ec) = 3;
        ai_Unk5830f0((char *)w[0xb + k * 3], buf1);
        ai_Unk583150((char *)w[0xb + k * 3], buf2);
        x = *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4) * k;
        id = d2_CreateWinText(0, x + 0x38, buf1, w[2]);
        *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + id * 0x2ec) = 1;
        *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + w[2] * 0x3c4);
        d2_SetObjFont(id, 0x54);
        id = d2_CreateWinText(0, x + 0x45, buf2, w[2]);
        *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + id * 0x2ec) = 1;
        *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + w[2] * 0x3c4);
        d2_SetObjFont(id, 0x54);
        x += 0x57;
        for (i = 0; i < 5; i++) {
            txt_Format(buf1, (char *)(unsigned)&full_data_62a56c, i + 0x16b7);
            id = d2_CreateWinText(0x28, x, buf1, w[2]);
            *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = 300;
            d2_SetObjFont(id, 0x54);
            sv_Unk52eaa0(0xa0, x, *(unsigned char *)(w[0xb + k * 3] + 0x24 + i), 0x4df);
            x += 0xd;
        }
    }
    U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4) = saved;
    d2_SetForm(w[0], 5);
    io_PrintText((char *)(unsigned)"$C");
    txt_Format(buf1, (char *)(unsigned)&full_data_62a564, *(int *)((unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
    io_PrintText((char *)(unsigned)&full_data_62a54c, 0x4101, buf1);
    txt_Format(buf1, (char *)(unsigned)&full_data_62a544, *(unsigned short *)((char *)e + 10) + 0x578);
    io_PrintText((char *)(unsigned)&full_data_62a54c, 0x4102, buf1);
    txt_Format(buf1, (char *)(unsigned)&full_data_62a53c, (int)ai_Unk5de518());
    io_PrintText((char *)(unsigned)&full_data_62a54c, 0x4103, buf1);
}
