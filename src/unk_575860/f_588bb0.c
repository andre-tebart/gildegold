/* 0x588bb0, the drag window of a named record (169 B, ROUGH): s = the name. The line: strcpy (0x180 bytes), the
   0x5ea986 uppercase, the 0x5de652 line (0x62a9cc, the buffer + 8) at +0x88; 0x42e770 (the line + 0x94) == -1 ->
   0. The window 0x62a9b4 with 0x475600, the section 1, the text 0x62a9a8 (the value), the section 2 and the text
   (the value + 1); the window */

int ai_Unk588bb0(char *s)
{
    char buf[0x180];
    int v;
    int win;

    strcpy(buf, s);
    _strupr(buf);
    sprintf(buf + 0x88, (char *)(unsigned)&full_data_62a9cc, buf + 8);
    v = txt_FindKey(buf + 0x94);
    if (v == -1 || txt_Ptrs[v] == 0)
        return 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a9b4);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    io_PrintText((unsigned)&full_data_62a9a8, v);
    d2_SetForm(0, 2);
    io_PrintText(v + 1);
    return win;
}
