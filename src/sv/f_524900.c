/* 0x524900, collects the image numbers for the character c into out[]: from the table 0x75ec98 (records of 8 words: [-6] the kind, [0] the first image, [1] the index of a short in the character's +0x17c record + 0xdf)
   and the image of 0x4cda50 (+ 0xdf); returns the count */

int sv_Unk524900(char *c, int *out)
{
    unsigned char *r;
    int n, i, *p;
    int *q;

    r = *(unsigned char **)(c + 0x17c);
    n = 0;
    if (r != 0 && *r != 0) {
        i = 0;
        p = (int *)(unsigned)&full_data_75ec98;
        for (; (int)p < (int)((unsigned)&full_data_75ec98 + 0x600); p += 8, i++) {
            if (p[-6] == *r) {
                out[n++] = *p;
                if (p[1] != -1) {
                    out[n++] = *(unsigned short *)(r + ((int *)(unsigned)&full_data_75ec9c)[i * 8]) + 0xdf;
                    break;
                }
            }
        }
    }
    q = (int *)ai_Unk4cda50(c);
    if (q != 0 && *q != 0)
        out[n++] = *q + 0xdf;
    return n;
}
