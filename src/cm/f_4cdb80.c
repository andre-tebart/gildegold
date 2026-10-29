/* 0x4cdb80, a character with the flag 0x20000000: with chance (byte +0x1c5) / 10 it is hit (1), else the flag is
   cleared (0) */

char ai_Unk4cdb80(char *c)
{
    char buf[0x800];
    char b = 1;
    char neg;
    char *ch, *g;
    int *r;
    int id;

    if (c != 0 && (*(int *)(c + 0x1c8) & 0x20000000) != 0) {
        neg = c[0x1c5];
        if (util_Random(10) < (unsigned char)neg) {
            neg = -neg;
            ai_Unk4cdb20(c);
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldValues(1, 1, (int)(c - cm_FieldObject) + 0x1c5, &neg);
            cm_SendFieldDeltas();
            g = *(char **)(c + 0x16c);
            if (g != 0 && *(unsigned short *)(g + 0x27) != 0xffff) {
                id = *(unsigned short *)(g + 0x27);
                ch = ai_Chars[id];
                if (ch[2] == 6 || ch[2] == 7) {
                    r = (int *)ai_Unk4cda50(c);
                    if (*r != 0) {
                        txt_Format(buf, 0x1bca, *r * 2 + 0x9a8, *(unsigned short *)c, g);
                        he_ShowNews(*(int *)(ch + 4), -1, buf, 0, 0x5f2, 0);
                    }
                }
            }
            return 1;
        }
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldValues(1, 1, (int)(c - cm_FieldObject) + 0x1c5, &b);
        cm_SendFieldDeltas();
        cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0, 0x20000000);
    }
    return 0;
}
