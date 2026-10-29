/* 0x583c50, the four graveyard buttons (396 B, the 0x582730 shape): rec = the record (its +0x65 the state
   record), win / section = the window and its section, btns = the four button ids out, selp = the selected one.
   0x475f70 (win, section) and the text 0x602568; the four buttons 0x4731d0 (i * 0x6c + 6, 0, the text
   0x5eecb0[i], 0x6ed938) with the window table 0xf8f080 +0x4c = 1, +0x1c0 = 3 and the icon 0x91aaf0 + the table
   0x5eece0[i] * 0x41 at +0x2e8; the buttons 1 .. 3 enabled by the state bytes +0x19 / 0x1a / 0x1b; the selected
   *selp 3 / 2 / 1 with its byte 1 -> that button marked at +0x2c, else the first marked and *selp = 0; the
   selected one marked at +0x28 */

void ai_Unk583c50(char *rec, int win, int section, int *btns, int *selp)
{
    char *r;
    int i;
    int id;
    int sel;

    r = (char *)U_I(rec + 0x65);
    d2_SetForm(win, section);
    io_PrintText((char *)(unsigned)"$C");
    for (i = 0; i < 4; i++) {
        id = d2_WinOpenPic(i * 0x6c + 6, 0, (char *)U_I(((unsigned)&ai_Unk5eeab8 + 0x1f8) + i * 4), U_I((unsigned)&d2_LastWindow));
        btns[i] = id;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + btns[i] * 0x2ec) = 3;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2e8 + btns[i] * 0x2ec) = U_I((unsigned)&ai_Goods) + U_I(((unsigned)&ai_Unk5eeab8 + 0x228) + i * 4) * 0x41;
    }
    d2_SetObjEnabled(btns[1], *(unsigned char *)(r + 0x19));
    d2_SetObjEnabled(btns[2], *(unsigned char *)(r + 0x1a));
    d2_SetObjEnabled(btns[3], *(unsigned char *)(r + 0x1b));
    sel = *selp;
    if (sel == 3 && *(char *)(r + 0x1b) == 1) {
        id = btns[3];
    } else if (sel == 2 && *(char *)(r + 0x1a) == 1) {
        id = btns[2];
    } else if (sel == 1 && *(char *)(r + 0x19) == 1) {
        id = btns[1];
    } else {
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + btns[0] * 0x2ec) = 1;
        *selp = 0;
        goto done;
    }
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + id * 0x2ec) = 1;
done:
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[*selp] * 0x2ec) = 1;
}
