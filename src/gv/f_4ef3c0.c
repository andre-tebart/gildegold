/* 0x4ef3c0, the number of slots of the object o by its record (+0x65, flags +0x19 / +0x1a / +0x1b) and the kind byte +0x247 of its building (1: 8, 16 with +0x19; 2: 12 / 24 with +0x19, + 8 with +0x1a; else 16 / 32 + 16 with +0x1a, + 8 with +0x1b) */

int unk_Unk4ef3c0(unsigned char *o)
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
            return 0x10;
    } else {
        if (k == 2) {
            n = 0xc;
            if (f == 1)
                n = 0x18;
            f = rec[0x1a];
        } else {
            n = 0x10;
            if (f == 1)
                n = 0x20;
            if (rec[0x1a] == 1)
                n = n + 0x10;
            f = rec[0x1b];
        }
        if (f == 1)
            n = n + 8;
    }
    return n;
}
