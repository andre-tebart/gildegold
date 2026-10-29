/* 0x4f27d0, the message handler of 0x4f2750: stores the value at +5 in the table record (+0x18) of the record p[1] (byte) and tells the character p[0] (0x5a9110 with the text 0x4f2fe0) unless it is the player's own */

int unk_Unk4f27d0(int *p)
{
    int rec[9];
    char buf[0x100];
    char *c;

    if (gv_GetTableRecord((char)p[1], rec) == 0)
        return 0;
    *(int *)(unk_Tab611258 + 0x18 + (char)p[1] * 0x24) = *(int *)((char *)p + 5);
    if (*(int *)(ai_Chars[ai_PlayerId] + 4) != p[0]) {
        c = gm_FindChar(p[0]);
        if (c != 0) {
            gv_TableRecordText(buf, (char)p[1]);
            ai_Unk5a9110(c, buf);
        }
    }
    return 1;
}
