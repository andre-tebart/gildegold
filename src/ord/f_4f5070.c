/* 0x4f5070, the first of a long run of small order handlers (c the character, m the message, kind 1 / 3; 1 = not handled, 2 = handled, text into m + 0x14): characters without flag 0x40 of +0x1c8 whose float +0x24 is below -20 or whose int +0x2c has bits above 0xf
   go to 0x5c8bc0 (kind 3) or get the text 0x1d3a (kind 1, a player) */

int unk_Unk4f5070(char *c, int *m)
{
    int k;

    k = m[0];
    if ((k == 3 || k == 1) && (c[0x1c8] & 0x40) == 0 && ((double)*(int *)(c + 0x24) < -20.0 || (*(unsigned *)(c + 0x2c) & 0xfffffff0) != 0)) {
        if (k == 3)
            return ai_Unk5c8bc0(c, m);
        if (k == 1 && c[2] == 6)
            txt_Format(m + 5, "%s", 0x1d3a);
        return 2;
    }
    return 1;
}
