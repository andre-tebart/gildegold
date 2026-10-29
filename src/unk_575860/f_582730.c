/* 0x582730, the four contact buttons (399 B): arg 1 = the record (its +0x65 the entries' state record), arg 2 =
   the window list (0 the window, 4 the parent, 8 .. 0xb the buttons, 0xc the selected button). 0x475f70 (the
   window, 6) and the text 0x602568; the four buttons 0x4731d0 (i * 0x6c + 6, 0, the text 0x5eecb0[i], the parent)
   into the list 8 .. 0xb with the window table 0xf8f080 +0x4c = 1, +0x1c0 = 3 and the icon 0x91aaf0 + the table
   0x5eece0[i] * 0x41 at +0x2e8; the buttons enabled by the state bytes +0x19 / 0x1a / 0x1b; the selected 0xc:
   3 / 2 / 1 with its byte 1 -> that button marked at +0x2c, else the first button marked and 0xc = 0; the
   selected one marked at +0x28 */

void ai_Unk582730(char *rec, int *w)
{
    char *r;
    int i;
    int id;
    int sel;

    r = (char *)U_I(rec + 0x65);
    d2_SetForm(w[0], 6);
    io_PrintText((char *)(unsigned)"$C");
    for (i = 0; i < 4; i++) {
        id = d2_WinOpenPic(i * 0x6c + 6, 0, (char *)U_I(((unsigned)&ai_Unk5eeab8 + 0x1f8) + i * 4), w[4]);
        w[8 + i] = id;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + id * 0x2ec) = 3;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2e8 + id * 0x2ec) = U_I((unsigned)&ai_Goods) + U_I(((unsigned)&ai_Unk5eeab8 + 0x228) + i * 4) * 0x41;
    }
    d2_SetObjEnabled(w[9], *(unsigned char *)(r + 0x19));
    d2_SetObjEnabled(w[10], *(unsigned char *)(r + 0x1a));
    d2_SetObjEnabled(w[0xb], *(unsigned char *)(r + 0x1b));
    sel = w[0xc];
    if (sel == 3 && *(char *)(r + 0x1b) == 1) {
        id = w[0xb];
    } else if (sel == 2 && *(char *)(r + 0x1a) == 1) {
        id = w[10];
    } else if (sel == 1 && *(char *)(r + 0x19) == 1) {
        id = w[9];
    } else {
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + w[8] * 0x2ec) = 1;
        w[0xc] = 0;
        goto done;
    }
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + id * 0x2ec) = 1;
done:
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + w[w[0xc] + 8] * 0x2ec) = 1;
}
