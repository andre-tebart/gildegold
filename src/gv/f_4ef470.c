/* 0x4ef470, as 0x4ef3c0 with the sizes 8 / 12, 12 / 18 + 2, 16 / 24 + 4 + 2 */

int unk_Unk4ef470(unsigned char *o)
{
    char *rec;
    char k, f;
    int n;

    rec = *(char **)(o + 0x65);
    f = rec[0x19];
    k = ai_BTypes[o[0] * 0x24d + 0x247];
    if (k == 1) {
        n = 8;
        if (f == 1)
            return 0xc;
    } else {
        if (k == 2) {
            n = 0xc;
            if (f == 1)
                n = 0x12;
            f = rec[0x1a];
        } else {
            n = 0x10;
            if (f == 1)
                n = 0x18;
            if (rec[0x1a] == 1)
                n = n + 4;
            f = rec[0x1b];
        }
        if (f == 1)
            n = n + 2;
    }
    return n;
}
