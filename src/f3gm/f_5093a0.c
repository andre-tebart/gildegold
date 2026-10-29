/* 0x5093a0, the same sum over the building kind p without asking for the instance */

int gm_Unk5093a0(int kind, unsigned k)
{
    char *p;
    int i, s;
    unsigned short *w;

    i = 0;
    p = ai_BTypes + kind * 0x24d;
    s = 0;
    w = (unsigned short *)(p + 0x23);
    do {
        if ((*w & 0x7fff) != 0 && (unsigned char)p[s + 0x1a3] == k)
            i += (unsigned char)p[s + 0x1e3];
        s++;
        w++;
    } while (s < 0x40);
    return i;
}
