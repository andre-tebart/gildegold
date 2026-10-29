/* 0x502eb0, sorts the characters that work in the building b into the lists of an output record: out[0] = their number, out[1 ..] (0x18 words, -1 filled) the workers of the first job class (type bytes +0x22f), out[0xd ..] (0xc) of the second (+0x230), out[0x19 ..] (0xc) the others (not the building's own two special jobs +0x223 / +0x224), out[0x25] = the one of the special jobs. ROUGH */

void gm_Unk502eb0(unsigned char *b, int *out)
{
    int i, n, j;
    char *k;
    char *c;
    unsigned *p1, *p2, *p3;

    k = ai_BTypes + *b * 0x24d;
    p1 = (unsigned *)(out + 1);
    out[0] = 0;
    for (j = 0; j < 0x18; j++)
        p1[j] = 0xffff;
    p3 = (unsigned *)(out + 0x19);
    for (j = 0; j < 0xc; j++)
        p3[j] = 0xffff;
    out[0x25] = 0xffff;
    p2 = (unsigned *)(out + 0xd);
    n = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(unsigned char **)(c + 0x170) == b) {
            if (k[0x22f] == c[0x165]) {
                *p1++ = *(unsigned short *)c;
            } else if (k[0x230] == c[0x165]) {
                *p2++ = *(unsigned short *)c;
            } else if (k[0x223] == 0 || k[0x224] == 0 || (k[0x223] != c[0x164] && k[0x224] != c[0x164])) {
                *p3++ = *(unsigned short *)c;
            } else {
                out[0x25] = i;
            }
            n++;
        }
    }
    out[0] = n;
}
