/* 0x4ef930, a random used slot (first word not -1; 0x44 bytes each at the table +0x20 of the record, 0x4ef3c0 of them, from a random start, cyclic) of the object o of the building kind 0x1d with the kind byte 1 .. 3; 0 when none */

int unk_Unk4ef930(unsigned char *o)
{
    char *rec;
    char *b;
    int n, k;
    unsigned i;

    rec = *(char **)(o + 0x65);
    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] == 0x1d && (unsigned char)b[0x247] != 0 && (unsigned char)b[0x247] < 4) {
        n = unk_Unk4ef3c0(o);
        i = util_RandomU(n) & 0xffff;
        if (n != 0) {
            k = n;
            do {
                if (*(int *)(*(char **)(rec + 0x20) + i * 0x44) != -1)
                    return (int)(*(char **)(rec + 0x20) + i * 0x44);
                k--;
                i = (int)(i + 1) % n;
            } while (k != 0);
        }
    }
    return 0;
}
