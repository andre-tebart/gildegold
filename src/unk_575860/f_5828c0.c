/* 0x5828c0, the contact list builder (635 B): rec = the record, w = the window list (0 the window, 2 the screen
   part, 4 the parent, 5 the font, 0xc the selected button, 0xd the entries of 4 dwords each, 0x4d the selected
   entry), sel = the newly clicked button (the current selection's entry 0xf8f368 + w[2] * 0x3c4 saved); 0x475f70
   (the window, 2) and (4) with the text 0x602568; the scroll area 0x52e860 (0, 0, w[5], w[2], 0x35, two locals,
   &sel); the entries 0x4ef7c0 (0, rec, the previous, 0, w[0xc]) each: +4 the entry, +8 its id, +0xc the byte
   0x4ef6b0 (the record + 0x65, the entry), the button 0x4731d0 (1, n * 0x35 + 2, the text 0x5eecb0 / 0x5eecc0 by
   the button, w[2]); the marker 0x4731d0 (0xf, n * 0x35 + 9, 0x7de) when 0x573200 (rec, the entry, the byte); the
   window table 0xf8f080 +0x4c = 1 and +0x1c0 = 3; the selected entry 0x4d clamped to 0 and marked at +0x2c / +0x28;
   the rest of the 0x10 entries filled (+0xd = -1, +0xe = 0, +0xf = 0xff, +0x10 = 0xff); the screen 0xf8f368 /
   0xf8f384 (w[2] * 0x3c4) restored and 0x46f840 (w[2]) */

int sv_CreateFormSlider(); unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l); int gv_EntryIndex(char *r, int *e); int he_Unk573200();
void d2_ScrollWindow();

void ai_Unk5828c0(char *rec, int *w, int sel)
{
    char a[4];
    char b[4];
    int *e;
    int *p;
    int saved;
    int n;
    int i;
    int id;

    saved = 0;
    if (sel == w[0xc])
        saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4);
    d2_SetForm(w[0], 2);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 4);
    io_PrintText((char *)(unsigned)"$C");
    sv_CreateFormSlider(0, 0, w[5], w[2], 0x35, a, b, &sel);
    e = (int *)gv_FindEntry(0, rec, 0, 0, w[0xc]);
    n = 0;
    if (e) {
        p = w + 0xd;
        do {
            p[1] = (int)e;
            p[2] = *e;
            *(char *)(p + 3) = gv_EntryIndex(U_I(rec + 0x65), e);
            if (e[1] == -1)
                id = d2_WinOpenPic(1, n * 0x35 + 2, (char *)U_I(((unsigned)&ai_Unk5eeab8 + 0x1f8) + w[0xc] * 4), w[2]);
            else
                id = d2_WinOpenPic(1, n * 0x35 + 2, (char *)U_I(((unsigned)&ai_Unk5eeab8 + 0x208) + w[0xc] * 4), w[2]);
            *p = id;
            if (he_Unk573200(rec, *e, (char)p[3]) != 0)
                d2_WinOpenPic(0xf, n * 0x35 + 9, 0x7de, w[2]);
            *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + *p * 0x2ec) = 1;
            *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *p * 0x2ec) = 3;
            e = (int *)gv_FindEntry(0, rec, e, 0, w[0xc]);
            n++;
            p += 4;
        } while (e);
    }
    if (n <= w[0x4d])
        w[0x4d] = 0;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + w[0x4d] * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + w[w[0x4d] * 4 + 0xd] * 0x2ec) = 1;
    if (n < 0x10) {
        p = w + n * 4 + 0xf;
        for (i = 0; i < 0x10 - n; i++) {
            p[-2] = -1;
            p[-1] = 0;
            p[0] = 0xff;
            *(unsigned char *)(p + 1) = 0xff;
            p += 4;
        }
    }
    U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4) = saved;
    U_I(((unsigned)&ai_Unkf8f13c + 0x248) + w[2] * 0x3c4) = 0x35;
    d2_ScrollWindow(w[2]);
}
