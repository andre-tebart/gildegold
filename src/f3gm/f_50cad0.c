/* 0x50cad0, changes the state byte (+0x5c) of the building b by d (kept in 0 .. 100) and sends it; when a building of a kind that matters drops to 0, every person is told (HS_66 for the owner, HS_67 for the others) */

void gm_Unk50cad0(unsigned char *b, int d)
{
    unsigned char k;
    char v;
    int *q;
    char buf1[0x400], buf2[0x400];

    v = d;
    k = b[0x5c];
    if (d + (char)k < 0)
        v = -k;
    if (100 < d + (char)k)
        v = 100 - k;
    cm_BeginFieldBlock(b, *(int *)(b + 1));
    cm_AddFieldValues(1, 1, 0x5c, &v);
    cm_SendFieldDeltas();
    if (*b != 0 && (int)v + (char)b[0x5c] < 1 && (k = ai_BTypes[*b * 0x24d]) != 0xf && k != 0x1c && k != 0x1b && k != 10 && k != 6 && k < 0x17 && k != 0x10) {
        q = (int *)(ai_Chars[0] + 4);
        do {
            if (*((char *)q - 2) == 6 || *((char *)q - 2) == 7) {
                if (*(short *)(q - 1) == *(short *)(b + 0x27)) {
                    txt_Format(buf1, 0x1c53, b);
                    he_ShowNews(*q, -1, buf1, 0, 0x5fe, "_NACHRICHTEN_HS_66");
                } else {
                    txt_Format(buf2, 0x1c52, *(short *)(b + 0x27));
                    he_ShowNews(*q, -1, buf2, 0, 0x5fe, "_NACHRICHTEN_HS_67");
                }
            }
            q += 0x87;
        } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x2));
        ai_Unk4c5510(*(int *)(b + 1));
    }
}
